#include "ScriptableClass.h"
#include "ScriptEngine.h"

#define JS_VERIFY(cond, error) { if (!this->jsVerify(cond, error)) { return; } }


ScriptableClass::ScriptableClass(ScriptEnginePointer engine) : _engine(engine) {
    Q_ASSERT(engine != nullptr);
}

bool ScriptableClass::jsVerify(bool condition, const QString& error) {
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

ScriptValue ScriptableClass::jsBindCallback(const ScriptValue& scope, const ScriptValue& callback) {
    ScriptValue handler = ::makeScopedHandlerObject(scope, callback);
    ScriptValue value = handler.property("callback");
    if (!jsVerify(handler.isObject() && value.isFunction(),
                 QString("jsBindCallback -- .callback is not a function (%1)").arg(value.toVariant().typeName()))) {
        return ScriptValue();
    }
    return handler;
}

Promise ScriptableClass::jsPromiseReady(Promise promise, const ScriptValue& scope, const ScriptValue& callback) {
    auto handler = jsBindCallback(scope, callback);
    if (!jsVerify(handler.isValid(), "jsPromiseReady -- invalid callback handler")) {
        return nullptr;
    }

    auto scriptEngine = engine();
    Q_ASSERT(scriptEngine);
    return promise->ready([this, handler, scriptEngine](QString error, QVariantMap result) {
        jsCallback(handler, scriptEngine->newValue(error), result);
    });
}

void ScriptableClass::jsCallback(const ScriptValue& handler,
                                         const ScriptValue& error, const ScriptValue& result) {
    Q_ASSERT(engine());
    //V8TODO: which kind of script context guard needs to be used here?
    ScriptContextGuard scriptContextGuard(engine()->currentContext());
    auto errorValue = !error.toBool() ? engine()->nullValue() : error;
    JS_VERIFY(handler.isObject() && handler.property("callback").isFunction(),
              QString("jsCallback -- .callback is not a function (%1)")
              .arg(handler.property("callback").toVariant().typeName()));
    ::callScopedHandlerObject(handler, errorValue, result);
}

void ScriptableClass::jsCallback(const ScriptValue& handler,
                                         const ScriptValue& error, const QVariantMap& result) {

    auto engine = handler.engine();
    Q_ASSERT(engine);

    jsCallback(handler, error, engine->toScriptValue(result));
}
