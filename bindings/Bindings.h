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

#ifndef __Bindings_h__
#define __Bindings_h__

#include <QObject>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptable>

namespace Bindings {

#define BIND_QOBJECT_SCRIPT(instance, engine) \
    engine->newQObject(instance, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassMethods | QScriptEngine::ExcludeSuperClassProperties | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);

#define ADD_QTSCRIPT_FACTORY(classname) \
    public: static QScriptValue __factory(QScriptContext * /*context*/, QScriptEngine * engine) { classname * instance = new classname(); return BIND_QOBJECT_SCRIPT(instance, engine); } private:

}

#endif
