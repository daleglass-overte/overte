//
//  WebDAVClass.h
//  libraries/script-engine/src/
//
//  Created by Dale Glass on 06/08/2023
//  Copyright 2023 Overte e.V.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//  SPDX-License-Identifier: Apache-2.0
//

/// @addtogroup ScriptEngine
/// @{

#pragma once

#include <QObject>
#include <QNetworkRequest>
#include "Scriptable.h"
#include "ScriptValue.h"

#include <qwebdavlib/qwebdav.h>
#include <qwebdavlib/qwebdavitem.h>
#include <qwebdavlib/qwebdavdirparser.h>


class WebDAVClass : public QObject {
    Q_OBJECT
public:
    WebDAVClass(ScriptEngine* engine);
    ~WebDAVClass();

    static ScriptValue constructor(ScriptContext* context, ScriptEngine* engine);

public slots:
    void setURL(const QUrl &url);
    void get(const QString &path, const ScriptValue &callback);
    void put(const QString &path, const QString &data, const ScriptValue &callback);
    void mkdir(const QString &path, const ScriptValue &callback);
    void copy(const QString &source, const QString &destination, bool overwrite, const ScriptValue &callback);
    void move(const QString &source, const QString &destination, bool overwrite, const ScriptValue &callback);
    void remove(const QString &path, const ScriptValue &callback);
    void listDirectory(const QString &path, const ScriptValue &callback);

signals:
    void requestComplete();
    void errorChanged(QString error);

private:
    ScriptEngine* _engine { nullptr };
    int64_t _max_
    QWebdav _webdav;
    QWebdavDirParser _dirparser;

};
/// @}