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

#ifndef __ScriptEnvironment_h__
#define __ScriptEnvironment_h__

#include <QObject>
class QScriptEngine;
class QPlainTextEdit;
class ScreenCapture;

/**
    \brief Setup HOST script bindings and execute GameSolver script code.
*/
class ScriptEnvironment : public QObject
{
    Q_OBJECT
    public:
        ScriptEnvironment(ScreenCapture * capture, QObject * parent = 0);
        ~ScriptEnvironment();

        // inject scripts here
        void setScript(const QString & script, const QString & fileName = QString(), int lineNumber = 1);

    private:
        void evaluate(const QString & script, const QString & fileName = QString(), int lineNumber = 1);
        QScriptEngine * m_engine;
        QPlainTextEdit * m_errorWindow;
};

#endif
