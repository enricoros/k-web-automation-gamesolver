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

void ColorClassifier::loadDictionary(const QString & directory)
{
#if 0
    QString dirString = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + QDir::separator() + directory;
#else
    QString dirString = directory;
#endif
    QDirIterator dIt(dirString, QStringList() << "*_*.png", QDir::Files);
    if (!dIt.hasNext())
        qWarning() << "ColorClassifier::loadDictionary: no files in current directory" << dirString;
    while (dIt.hasNext()) {
        QString fileName = dIt.next();

        // get the number
        QString numberString = fileName.section("_", -1, -1).section(".", 0, 0);
        bool nOk;
        int number = numberString.toInt(&nOk);
        if (numberString.isEmpty() || !nOk) {
            qWarning() << "ColorClassifier::loadDictionary: bad filename" << numberString;
            continue;
        }

        // get the image
        QImage image( fileName, "PNG" );
        if (image.isNull() ) {
            qWarning() << "ColorClassifier::loadDictionary: error loading image" << fileName;
            continue;
        }

        // train the classifier
        m_classifier->addClass(number, image);
        qWarning() <<  "ColorClassifier::loadDictionary: loaded" << fileName;
    }
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


} // namespace ScriptObjects
