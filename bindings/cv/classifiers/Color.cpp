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

#include "bindings/cv/classifiers/Color.h"
#include "bindings/cv/Image.h"
#include "HueClassifier.h"
#include <QDialog>
#include <QInputDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QtScript>

namespace Bindings { namespace CV { namespace Classifiers {

Color::Color()
    : m_classifier(new HueClassifier())
{
}

Color::~Color()
{
    delete m_classifier;
}

void Color::setDictionary(const QString & name, bool load)
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
            qWarning() << "Color::setDictionary: no files in current directory" << m_dictionaryDir;
        while (dIt.hasNext()) {
            QString fileName = dIt.next();

            // get the number
            QString numberString = fileName.section("_", -1, -1).section(".", 0, 0);
            bool nOk;
            int number = numberString.toInt(&nOk);
            if (numberString.isEmpty() || !nOk) {
                qWarning() << "Color::setDictionary: bad filename" << numberString;
                continue;
            }

            // get the image
            QImage image( fileName, "PNG" );
            if (image.isNull() ) {
                qWarning() << "Color::setDictionary: error loading image" << fileName;
                continue;
            }

            // train the classifier
            m_classifier->addClass(number, image);
            qWarning() <<  "Color::setDictionary: loaded" << fileName;
        }
    }
}

QScriptValue Color::classify(const QScriptValue & value, bool train)
{
    // get the pixmap out of the value
    Bindings::CV::Image * image = qobject_cast<Image *>(value.toQObject());
    if (!image) {
        qWarning("Color::classify: classifying the wrong type");
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

} } } // namespace Bindings::CV::Classifiers
