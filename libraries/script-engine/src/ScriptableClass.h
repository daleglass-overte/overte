#pragma once

#include "ScriptContext.h"
#include "ScriptValue.h"
#include "shared/MiniPromises.h"

class ScriptEngine;
using ScriptEnginePointer = std::shared_ptr<ScriptEngine>;
using Promise = MiniPromise::Promise;


/**
 * @brief An engine-independent clone of QScriptable
 *
 * This imitates QScriptable to be compatible with the old engine code. It also provides
 * some functionality for classes that provide a scripting API.
 *
 * A QObject can be exposed to the scripting engine with ScriptEngine::newFunction or
 * ScriptEngine::newObject. Inheriting this class gives the derived class access to the
 * script environment it's called from. This allows implementing callbacks and promises,
 * throwing exceptions, and examining the caller's context.
 *
 * Unlike with QScriptable, utility functions for callbacks and promises are provided.
 *
 * This is expected to be a replacement for Scriptable but meanwhile is being kept separate
 * for testing. Unlike Scriptable it's not static.
 *
 */
class ScriptableClass {
public:

    /**
     * @brief Construct a new Scriptable Class object
     *
     *
     * @param engine Engine the object belongs to.
     */
    ScriptableClass(ScriptEnginePointer engine);


    /**
     * @brief Pointer to the engine this object belongs to
     *
     * An object always belongs to an engine. It doesn't ever change,
     * and is always valid.
     *
     * @return ScriptEnginePointer
     */
    ScriptEnginePointer engine() const {
        return _engine;
    }


    /**
     * @brief Script context we're being called from. If we're not currently
     * being called, then this returns a nullptr.
     *
     * @return ScriptContextPointer
     */
    ScriptContextPointer context() {
        return _context;
    }

    /**
     * @brief Set the current context.
     *
     * Only to be used by the scripting engine.
     * @todo Make this a friend-only function?
     *
     * @param context
     */
    void Scriptable::setContext(ScriptContextPointer context) {
        scriptContextStore = context;
    }

    /**
     * @brief Returns the 'this' object from JS
     *
     * This is obtained from the script context. If there's no context,
     * returns an empty ScriptValue.
     *
     * @return ScriptValue
     */
    ScriptValue thisObject() {
        ScriptContextPointer scriptContext = context();
        return scriptContext ? scriptContext->thisObject() : ScriptValue();
    }


    /**
     * @brief Returns the number of arguments passed to the JS function we were called from
     *
     * This is obtained from the script context. If there's no context, returns 0.
     *
     * @return int
     */
    int argumentCount() const {
        ScriptContextPointer scriptContext = context();
        return scriptContext ? scriptContext->argumentCount() : 0;
    }

    /**
     * @brief Returns the argument at the specified index
     *
     * This is obtained from the script context. If there's no context, an empty ScriptValue.
     *
     * @param index
     * @return ScriptValue
     */
    ScriptValue argument(int index) {
        ScriptContextPointer scriptContext = context();
        return scriptContext ? scriptContext->argument(index) : ScriptValue();
    }


    /**
     * @brief Returns the current JS script context
     *
     * If there's no context, returns a nullptr.
     *
     * @return ScriptContextPointer
     */
    ScriptContextPointer Scriptable::context() const {
        return _context;
    }




protected:
    ScriptValue jsBindCallback(const ScriptValue& scope, const ScriptValue& callback = ScriptValue());
    Promise jsPromiseReady(Promise promise, const ScriptValue& scope, const ScriptValue& callback = ScriptValue());

    /**
     * @brief Call a callback in JS
     *
     * This implements a CPS style callback, see http://fredkschott.com/post/2014/03/understanding-error-first-callbacks-in-node-js/
     *
     * @param handler JS function to call
     * @param error Error, if something went wrong
     * @param result Result for the callback
     */
    void jsCallback(const ScriptValue& handler, const ScriptValue& error, const QVariantMap& result);

    /**
     * @brief Call a callback in JS
     *
     * This implements a CPS style callback, see http://fredkschott.com/post/2014/03/understanding-error-first-callbacks-in-node-js/
     *
     * @param handler JS function to call
     * @param error Error, if something went wrong
     * @param result Result for the callback
     */
    void jsCallback(const ScriptValue& handler, const ScriptValue& error, const ScriptValue& result);

    /**
     * @brief If condition is not true, emit exception in the current JS context
     *
     * This is a helper function for error checking. If condition is false, the error string
     * is thrown as an exception to JS.
     *
     * The return value is the same as condition
     *
     * @param condition Whether our test succeeded
     * @param error Exception to throw if condition is false
     * @return true If condition is true
     * @return false If condition is false
     */
    bool jsVerify(bool condition, const QString& error);
private:
    ScriptEnginePointer _engine{nullptr};
    ScriptContextPointer _context{nullptr};
};