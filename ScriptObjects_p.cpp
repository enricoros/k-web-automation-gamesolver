/***************************************************************************
 * Copyright (c) 2009 Enrico Ros                                           *
 *         2009 Enrico Ros <enrico.ros@email.it>                           *
 *                                                                         *
 * Permission is hereby granted, free of charge, to any person             *
 * obtaining a copy of this software and associated documentation          *
 * files (the "Software"), to deal in the Software without                 *
 * restriction, including without limitation the rights to use,            *
 * copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the               *
 * Software is furnished to do so, subject to the following                *
 * conditions:                                                             *
 *                                                                         *
 * The above copyright notice and this permission notice shall be          *
 * included in all copies or substantial portions of the Software.         *
 *                                                                         *
 ***************************************************************************/

#include "ScriptObjects_p.h"
#include "HueClassifier.h"
#include "ScreenCapture.h"
#include "ScreenCapture.h"
#include <QDialog>
#include <QInputDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QtScript>

namespace ScriptObjects {


/// Capture

Capture::Capture(ScreenCapture * capture)
    : m_capture(capture)
{
}

QScriptValue Capture::capture()
{
    QPixmap cPixmap = m_capture->capture();
    return Image::__factory(cPixmap, context(), engine());
}


/// ColorClassifier

ColorClassifier::ColorClassifier()
    : m_classifier(new HueClassifier())
{
}

ColorClassifier::~ColorClassifier()
{
    delete m_classifier;
}

void ColorClassifier::setDictionary(const QString & name, bool load)
{
#if 0
    m_dictionaryDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + QDir::separator() + name;
#else
    m_dictionaryDir = name;
#endif

    // init the dictionary
    m_classifier->clear();
    QDir().mkpath(m_dictionaryDir);

    // load from disk, if requested
    if (load) {
        QDirIterator dIt(m_dictionaryDir, QStringList() << "*_*.png", QDir::Files);
        if (!dIt.hasNext())
            qWarning() << "ColorClassifier::setDictionary: no files in current directory" << m_dictionaryDir;
        while (dIt.hasNext()) {
            QString fileName = dIt.next();

            // get the number
            QString numberString = fileName.section("_", -1, -1).section(".", 0, 0);
            bool nOk;
            int number = numberString.toInt(&nOk);
            if (numberString.isEmpty() || !nOk) {
                qWarning() << "ColorClassifier::setDictionary: bad filename" << numberString;
                continue;
            }

            // get the image
            QImage image( fileName, "PNG" );
            if (image.isNull() ) {
                qWarning() << "ColorClassifier::setDictionary: error loading image" << fileName;
                continue;
            }

            // train the classifier
            m_classifier->addClass(number, image);
            qWarning() <<  "ColorClassifier::setDictionary: loaded" << fileName;
        }
    }
}

QScriptValue ColorClassifier::classify(const QScriptValue & value, bool train)
{
    // get the pixmap out of the value
    Image * image = qobject_cast<Image *>(value.toQObject());
    if (!image) {
        qWarning("ColorClassifier::classify: classifying the wrong type");
        return engine()->undefinedValue();
    }

    // classify the image
    QImage img = image->pixmap().toImage();
    ClassifyResult cr = m_classifier->classify(img);
    if (train && cr.confidence < 0.2) {
        image->show(true);
        cr.index = QInputDialog::getInt(0, tr("Insert integer value"), QString::number(cr.confidence), 1);
        if (cr.index) {
            m_classifier->addClass(cr.index, img);
            if (!m_dictionaryDir.isEmpty())
                img.save(m_dictionaryDir + QDir::separator() + tr("element_%1.png").arg(QString::number(cr.index)));
        } else
            return engine()->undefinedValue();
    }
    return engine()->newVariant(cr.index);
}


/// Image

QScriptValue Image::__factory(const QPixmap & pixmap, QScriptContext * context, QScriptEngine * engine)
{
    QScriptValue imageValue = Image::__factory(context, engine);
    Image * image = qobject_cast<Image *>(imageValue.toQObject());
    image->setPixmap(pixmap);
    return imageValue;
}

void Image::trim(int left, int top, int right, int bottom)
{
    // sanity check
    if (left < 0 || top < 0 || right > 0 || bottom > 0) {
        qWarning("Image::trim: error, need l>=0, t>=0, r<=0, b<=0");
        return;
    }
    QPixmap pix = m_pixmap.copy(left, top, m_pixmap.width() - left + right, m_pixmap.height() - top + bottom);
    if (pix.isNull()) {
        qWarning("Image::trim: error trimming, zero size image");
        return;
    }
    setPixmap(pix);
}

QScriptValue Image::copy(int left, int top, int width, int height)
{
    QPixmap pix = m_pixmap.copy(left, top, width, height);
    if (pix.isNull()) {
        qWarning("Image::copy: error copying image (wrong rect maybe?)");
        return engine()->undefinedValue();
    }
    return Image::__factory(pix, context(), engine());
}

QScriptValue Image::split(int rows, int cols)
{
    // validation
    if (rows < 1 || cols < 1 || rows > m_pixmap.width() || cols > m_pixmap.height() || m_pixmap.isNull()) {
        qWarning("Image::split: can't split the image, bad parameters (%d %d - %d)", rows, cols, m_pixmap.isNull());
        return engine()->undefinedValue();
    }

    // create array
    QScriptValue arrayValue = engine()->newArray(rows * cols);
    int W = m_pixmap.width();
    int H = m_pixmap.height();
    int index = 0;
    int top = 0;
    for (int r = 0; r < rows; r++) {
        int left = 0;
        int height = ((H * (r + 1)) / rows) - top;
        for (int c = 0; c < cols; c++) {
            // get the pixmap
            int width = (W * (c + 1) / cols) - left;
            QPixmap cPixmap = m_pixmap.copy(left, top, width, height);
            // create a new image
            QScriptValue imageValue = Image::__factory(cPixmap, context(), engine());
            arrayValue.setProperty(index++, imageValue);
            left += width;
        }
        top += height;
    }

    // return the array of Images
    return arrayValue;
}

void Image::save(const QString & fileName)
{
    if (!m_pixmap.save(fileName)) {
        qWarning("Image::save: error saving the image (%dx%d) to the file %s", m_pixmap.width(), m_pixmap.height(), qPrintable(fileName));
        return;
    }
}

void Image::show(bool modal)
{
    QLabel * label = new QLabel();
    label->setAttribute(Qt::WA_DeleteOnClose);
    label->setFixedSize(m_pixmap.size());
    label->setPixmap(m_pixmap);
    if (modal) {
        QDialog dialog;
        QVBoxLayout lay(&dialog);
        lay.addWidget(label);
        dialog.exec();
    } else
        label->show();
}

} // namespace ScriptObjects
