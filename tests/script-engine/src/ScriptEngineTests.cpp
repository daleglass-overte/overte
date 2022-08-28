#include <QSignalSpy>
#include <QDebug>

#include "ScriptEngineTests.h"
#include "DependencyManager.h"

#include "ScriptEngines.h"
#include "ScriptEngine.h"

QTEST_MAIN(ScriptEngineTests)



void ScriptEngineTests::initTestCase() {
    DependencyManager::set<ScriptEngines>(ScriptManager::CLIENT_SCRIPT, defaultScriptsOverrideOption);


    auto scriptEngines = DependencyManager::get<ScriptEngines>().data();

    scriptEngines->registerScriptInitializer([this](ScriptManagerPointer manager) {
        qDebug() << "Running script initializer";

    });

}

void ScriptEngineTests::scriptTest() {
    QSharedPointer<ScriptEngines> ac = DependencyManager::get<ScriptEngines>();
    QVERIFY(!ac.isNull());


}
