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

#ifndef __ScriptObjects_h__
#define __ScriptObjects_h__

#include <QObject>
#include <QColor>
#include <QtScript>
class HueClassifier;
class ScreenCapture;

namespace ScriptObjects {

/// define Exposed GLOBAL objects

    class Capture : public QObject, public QScriptable {
        Q_OBJECT
        public:
            Capture(ScreenCapture * capture);

        public Q_SLOTS:
            QScriptValue capture();

        private:
            ScreenCapture * m_capture;
    };


/// define Exposed Prototype objects
    #define ADD_QTSCRIPT_FACTORY(classname) \
        public: static QScriptValue factory(QScriptContext * /*context*/, QScriptEngine * engine) { classname * instance = new classname(); QScriptValue instanceValue = engine->newQObject(instance, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassMethods | QScriptEngine::ExcludeSuperClassProperties | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater); return instanceValue; } private:

    class ColorClassifier : public QObject, public QScriptable  {
        Q_OBJECT
        ADD_QTSCRIPT_FACTORY(ColorClassifier)
        public Q_SLOTS:
            void loadDictionary(const QString & directory);
        private:
            ColorClassifier();
            ~ColorClassifier();
            HueClassifier * m_classifier;
    };

    class Image : public QObject {
        Q_OBJECT
        ADD_QTSCRIPT_FACTORY(Image);
        Q_PROPERTY(QString color READ color WRITE setColor)
        private:
            QString color() const { return m_color.name(); }
            void setColor( QString brush ) { m_color = QColor(brush); }
            QColor m_color;
    };

} // namespace ScriptObjects

#endif
