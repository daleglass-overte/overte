//
//  WebDAVClass.cpp
//  libraries/script-engine/src/
//
//  Created by Dale Glass on 06/08/2023
//  Copyright 2023 Overte e.V.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//  SPDX-License-Identifier: Apache-2.0
//

#include "WebDAVClass.h"
#include "ScriptEngine.h"
#include "shared/MiniPromises.h"

WebDAVClass::WebDAVClass(ScriptEnginePointer engine) :
    ScriptableClass(engine) {

}

WebDAVClass::~WebDAVClass() {
//    if (_reply) { _reply->deleteLater(); }
}


ScriptValue WebDAVClass::constructor(ScriptContextPointer context, ScriptEnginePointer engine) {
    qCDebug(scriptengine)  << "Created WebDAV";
    return engine->newQObject(new WebDAVClass(engine), ScriptEngine::ScriptOwnership);
}

void WebDAVClass::setURL(const QUrl &url) {
    _webdav.setConnectionSettings(url);
    qCDebug(scriptengine)  << "URL set";
}

void WebDAVClass::get(const QString &path, const ScriptValue &callback) {
    auto handler = jsBindCallback(thisObject(), callback);
    QNetworkReply *reply = _webdav.get(path);


    Promise deferred = makePromise(__FUNCTION__);
    auto scriptEngine = engine();
    deferred->ready([=](QString error, QVariantMap result) {
        auto url = result.value("path").toString();
        auto reqError = result.value("error").toString();
        auto data = result.value("data").toString();
        jsCallback(handler, scriptEngine->newValue(url), scriptEngine->newValue(data));
    });

    connect(reply, &QNetworkReply::readyRead, reply, [deferred, path, reply]() {
        qCDebug(scriptengine) << "GET callback! readyRead called!";

        auto error = reply->error();
        qCDebug(scriptengine) << "Error is" << error;

        auto data = reply->readAll();
        qCDebug(scriptengine) << "Data is" << data;

        qCDebug(scriptengine) << "Path is" << path;

        deferred->resolve({
            { "path", path },
            { "error", error },
            { "data", data }
        });

        qCDebug(scriptengine) << "GET callback! Path" << path << ", error" << error << ", data" << data;
        reply->deleteLater();
    });

    qCDebug(scriptengine)  << "GET: " << path;
}

void WebDAVClass::put(const QString &path, const QString &data, const ScriptValue &callback) {
    QNetworkReply *reply = _webdav.put(path, data.toUtf8());


    Promise deferred = makePromise(__FUNCTION__);
    connect(reply, &QNetworkReply::finished, reply, [deferred, &path, &reply]() {
        deferred->resolve({
            { "path", path },
            { "error", reply->error() }
        });

        reply->deleteLater();
    });

    qCDebug(scriptengine)  << "PUT: " << path;
}
void WebDAVClass::mkdir(const QString &path, const ScriptValue &callback) {
    qCWarning(scriptengine)  << "Not implemented";
}

void WebDAVClass::copy(const QString &source, const QString &destination, bool overwrite, const ScriptValue &callback) {
    qCWarning(scriptengine)  << "Not implemented";
}

void WebDAVClass::move(const QString &source, const QString &destination, bool overwrite, const ScriptValue &callback) {
    qCWarning(scriptengine)  << "Not implemented";
}

void WebDAVClass::remove(const QString &path, const ScriptValue &callback) {
    qCWarning(scriptengine)  << "Not implemented";
}

void WebDAVClass::listDirectory(const QString &path, const ScriptValue &callback) {
    qCWarning(scriptengine)  << "Not implemented";
}