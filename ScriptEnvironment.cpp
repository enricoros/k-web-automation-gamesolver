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

#include "ScriptEnvironment.h"
#include "ScriptObjects_p.h"
#include "ScreenCapture.h"
#include <QtScript>
#include <QPlainTextEdit>

static QScriptValue qs_describe(QScriptContext * ctx, QScriptEngine * eng)
{
    QStringList result;
    QScriptValue obj = ctx->argument(0);
    //QString name = obj.toString();
    while (obj.isObject()) {
        QScriptValueIterator it(obj);
        while (it.hasNext()) {
            it.next();
            result.append(it.name());
        }
        obj = obj.prototype();
    }
    qWarning("[%s]", qPrintable(result.join(",")));
    return eng->undefinedValue();
}

static QScriptValue qs_printerr(QScriptContext * ctx, QScriptEngine * eng)
{
    QStringList result;
    for (int i = 0; i < ctx->argumentCount(); i++) {
        QScriptValue obj = ctx->argument(i);
        result.append(obj.toString());
    }
    qWarning("E>> %s", qPrintable(result.join(",")));
    return eng->undefinedValue();
}

ScriptEnvironment::ScriptEnvironment(ScreenCapture * capture, QObject * parent)
    : QObject(parent)
    , m_engine(new QScriptEngine(this))
{
    // this.capture: script wrapper over ScreenCapture
    ScriptObjects::Capture * sCapture = new ScriptObjects::Capture(capture);
    QScriptValue capValue = m_engine->newQObject(sCapture, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassMethods | QScriptEngine::ExcludeSuperClassProperties | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    m_engine->globalObject().setProperty("capture", capValue);

    // register a couple of functions
    m_engine->globalObject().setProperty(QLatin1String("__describe__"), m_engine->newFunction(qs_describe));
    m_engine->globalObject().setProperty(QLatin1String("__printerr__"), m_engine->newFunction(qs_printerr));

    // register prototypes
#define ADD_PROTOTYPE_CONSTRUCTOR(factoryFunction, scriptName) \
    {QScriptValue ffValue = m_engine->newFunction(factoryFunction); \
    m_engine->globalObject().setProperty(scriptName, ffValue, QScriptValue::ReadOnly | QScriptValue::Undeletable);}
    ADD_PROTOTYPE_CONSTRUCTOR(ScriptObjects::ColorClassifier::factory, "Classifier");
    ADD_PROTOTYPE_CONSTRUCTOR(ScriptObjects::Image::factory, "Image");

    // debug scripts
    m_errorWindow = new QPlainTextEdit();
    m_errorWindow->hide();
    m_errorWindow->resize(300, 200);
}

ScriptEnvironment::~ScriptEnvironment()
{
    /// delete scriptCapture;
    delete m_engine;
}

void ScriptEnvironment::setScript(const QString & script, const QString & fileName, int lineNumber)
{
    evaluate(script, fileName, lineNumber);
}

void ScriptEnvironment::evaluate(const QString & script, const QString & fileName, int lineNumber)
{
    QScriptValue retVal = m_engine->evaluate(script, fileName, lineNumber);
    if (m_engine->hasUncaughtException()) {
        m_errorWindow->show();
        m_errorWindow->appendPlainText(tr("error at line %1: %2").arg(m_engine->uncaughtExceptionLineNumber()).arg(m_engine->uncaughtException().toString()));
    } else if (retVal.isError()) {
        m_errorWindow->show();
        m_errorWindow->appendPlainText(tr("error: %1").arg(retVal.toString()));
    } else if (!retVal.isUndefined()) {
        m_errorWindow->show();
        m_errorWindow->appendPlainText(tr("ok, return valure: %1").arg(retVal.toString()));
    } else {
        m_errorWindow->show();
        m_errorWindow->appendPlainText(tr("ok"));
    }
}

