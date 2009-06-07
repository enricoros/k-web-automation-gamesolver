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
#include "bindings/cv/classifiers/Color.h"
#include "bindings/cv/Capture.h"
#include "bindings/cv/Image.h"
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
    // 1. register a couple of global functions
    m_engine->globalObject().setProperty(QLatin1String("__describe__"), m_engine->newFunction(qs_describe));
    m_engine->globalObject().setProperty(QLatin1String("__printerr__"), m_engine->newFunction(qs_printerr));

    // 2. namespaces
#define ADD_PROTOTYPE_CONSTRUCTOR(factoryFunction, scriptName, parentValue) \
    {QScriptValue ffValue = m_engine->newFunction(factoryFunction); \
    parentValue.setProperty(scriptName, ffValue, QScriptValue::ReadOnly | QScriptValue::Undeletable);}

    // CV.
    QScriptValue n_cv = m_engine->newObject();
    m_engine->globalObject().setProperty("CV", n_cv);

    // CV.Capture (script wrapper over ScreenCapture)
    Bindings::CV::Capture * sCapture = new Bindings::CV::Capture(capture);
    QScriptValue capValue = m_engine->newQObject(sCapture, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassMethods | QScriptEngine::ExcludeSuperClassProperties | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    n_cv.setProperty("Capture", capValue);

    // CV.Image [prototype]
    ADD_PROTOTYPE_CONSTRUCTOR(Bindings::CV::Image::__factory, "Image", n_cv);

    // CV.Classifiers.
    QScriptValue n_cv_classifiers = m_engine->newObject();
    n_cv.setProperty("Classifiers", n_cv_classifiers);

    // CV.Classifiers.Color [prototype]
    ADD_PROTOTYPE_CONSTRUCTOR(Bindings::CV::Classifiers::Color::__factory, "Color", n_cv_classifiers);

    // 3. debug scripts
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

