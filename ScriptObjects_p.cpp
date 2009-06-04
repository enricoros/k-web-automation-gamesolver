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
    QScriptValue imageValue = Image::factory(context(), engine());
    QPixmap pix = m_capture->capture();
    imageValue.setProperty("width", pix.width());
    imageValue.setProperty("height", pix.height());
    return imageValue;
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


/// ...

} // namespace ScriptObjects
