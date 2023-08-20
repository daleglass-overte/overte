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
#include "MiniPromises.h"

WebDAVClass::WebDAVClass(ScriptEngine* engine) :
    _engine(engine) {

}

WebDAVClass::~WebDAVClass() {
//    if (_reply) { _reply->deleteLater(); }
}


ScriptValue WebDAVClass::constructor(ScriptContext* context, ScriptEngine* engine) {
    qCDebug(scriptengine)  << "Created WebDAV";
    return engine->newQObject(new WebDAVClass(engine), ScriptEngine::ScriptOwnership);
}

void WebDAVClass::setURL(const QUrl &url) {
    _webdav.setConnectionSettings(url);
    qCDebug(scriptengine)  << "URL set";
}

void WebDAVClass::get(const QString &path, const ScriptValue &callback) {
    auto handler = jsBindCallback(thisObject(), callback);
    QNetworkReply *reply = _webdav.put(path, data.toUtf8());


    Promise deferred = makePromise(__FUNCTION__);
    connect(reply, &QNetworkRequest::finished, reply, [deferred, &path]() {
        deferred->resolve({
            { "path" : path },
            { "error" : reply->error() },
            { "data" : reply->readAll() }
        })

        reply->deleteLater();
    })

    qCDebug(scriptengine)  << "GET: " << path;
}

void WebDAVClass::put(const QString &path, const QString &data, const ScriptValue &callback) {
    QNetworkReply *reply = _webdav.put(path, data.toUtf8());


    Promise deferred = makePromise(__FUNCTION__);
    connect(reply, &QNetworkRequest::finished, reply, [deferred, &path]() {
        deferred->resolve({
            { "path" : path },
            { "error" : reply->error() }
        })

        reply->deleteLater();
    })

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