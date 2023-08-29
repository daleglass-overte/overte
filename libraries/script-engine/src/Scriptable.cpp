//
//  Scriptable.cpp
//  libraries/script-engine/src
//
//  Created by Heather Anderson on 5/22/21.
//  Copyright 2021 Vircadia contributors.
//  Copyright 2023 Overte e.V.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//  SPDX-License-Identifier: Apache-2.0
//

#include "Scriptable.h"
#include "ScriptEngine.h"

static thread_local ScriptContext* scriptContextStore;
#define JS_VERIFY(cond, error) { if (!this->jsVerify(cond, error)) { return; } }


ScriptContext* Scriptable::context() {
    return scriptContextStore;
}

void Scriptable::setContext(ScriptContext* context) {
    scriptContextStore = context;
}


bool Scriptable::jsVerify(bool condition, const QString& error) {
    if (condition) {
        return true;
    }
    if (context()) {
        context()->throwError(error);
    } else {
        qCDebug(scriptengine) << "WARNING -- jsVerify failed outside of a valid JS context: " + error;
    }
    return false;
}

ScriptValue Scriptable::jsBindCallback(const ScriptValue& scope, const ScriptValue& callback) {
    ScriptValue handler = ::makeScopedHandlerObject(scope, callback);
    ScriptValue value = handler.property("callback");
    if (!jsVerify(handler.isObject() && value.isFunction(),
                 QString("jsBindCallback -- .callback is not a function (%1)").arg(value.toVariant().typeName()))) {
        return ScriptValue();
    }
    return handler;
}

Promise Scriptable::jsPromiseReady(Promise promise, const ScriptValue& scope, const ScriptValue& callback) {
    auto handler = jsBindCallback(scope, callback);
    if (!jsVerify(handler.isValid(), "jsPromiseReady -- invalid callback handler")) {
        return nullptr;
    }
    Q_ASSERT(engine);
    auto scriptEngine = engine();
    return promise->ready([this, handler, scriptEngine](QString error, QVariantMap result) {
        jsCallback(handler, scriptEngine->newValue(error), result);
    });
}

void Scriptable::jsCallback(const ScriptValue& handler,
                                         const ScriptValue& error, const ScriptValue& result) {
    Q_ASSERT(engine);
    //V8TODO: which kind of script context guard needs to be used here?
    ScriptContextGuard scriptContextGuard(engine()->currentContext());
    auto errorValue = !error.toBool() ? engine()->nullValue() : error;
    JS_VERIFY(handler.isObject() && handler.property("callback").isFunction(),
              QString("jsCallback -- .callback is not a function (%1)")
              .arg(handler.property("callback").toVariant().typeName()));
    ::callScopedHandlerObject(handler, errorValue, result);
}

void Scriptable::jsCallback(const ScriptValue& handler,
                                         const ScriptValue& error, const QVariantMap& result) {
    Q_ASSERT(handler.engine());
    auto engine = handler.engine();

    jsCallback(handler, error, engine->toScriptValue(result));
}
