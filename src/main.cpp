#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <KIconTheme>

#include <osmscoutclientqt/OSMScoutQt.h>
#include <osmscout/log/Logger.h>

#define DEBUG_VALUE   4
#define INFO_VALUE    3
#define WARNING_VALUE 2
#define ERROR_VALUE   1

static int LogEnv(const QString &env)
{
  if (env.toUpper() == "DEBUG")
  {
    return DEBUG_VALUE;
  }

  if (env.toUpper() == "INFO")
  {
    return INFO_VALUE;
  }

  if (env.toUpper() == "WARNING")
  {
    return WARNING_VALUE;
  }

  if (env.toUpper() == "ERROR")
  {
    return ERROR_VALUE;
  }

  return WARNING_VALUE;
}

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("Urania"));
    QGuiApplication::setDesktopFileName(QStringLiteral("com.github.pamugk.urania"));

    osmscout::OSMScoutQt::RegisterQmlTypes();

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
    {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    QString logLevelName = QProcessEnvironment::systemEnvironment().value("OSMSCOUT_LOG", "WARNING");

    std::cout << "Setting libosmscout logging to level: " << logLevelName.toStdString() << std::endl;

    int logEnv=LogEnv(logLevelName);

    osmscout::log.Debug(logEnv >= DEBUG_VALUE);
    osmscout::log.Info(logEnv >= INFO_VALUE);
    osmscout::log.Warn(logEnv >= WARNING_VALUE);
    osmscout::log.Error(logEnv >= ERROR_VALUE);

    osmscout::OSMScoutQtBuilder builder = osmscout::OSMScoutQt::NewInstance();

    QStringList mapLookupDirectories;

    mapLookupDirectories << QStringLiteral("/mnt/storage/admin/Maps/russia");

    QDir dir(QStringLiteral("/mnt/storage/admin/Maps/russia"));

    if (dir.cdUp())
    {
        if (dir.cd("world"))
        {
          builder.WithBasemapLookupDirectory(dir.absolutePath());
        }
    }

    QFileInfo stylesheetFile("/home/admin/git/libosmscout/stylesheets/standard.oss");

    builder
        .WithStyleSheetDirectory(stylesheetFile.dir().path())
        .WithStyleSheetFile(stylesheetFile.fileName())
        .WithIconDirectory(QStringLiteral("/home/admin/git/libosmscout/OSMScout2/pics"))
        .WithMapLookupDirectories(mapLookupDirectories)
        .AddOnlineTileProviders("/home/admin/git/libosmscout/OSMScout2/resources/online-tile-providers.json")
        .AddMapProviders("/home/admin/git/libosmscout/OSMScout2/resources/map-providers.json")
        .AddVoiceProviders("/home/admin/git/libosmscout/OSMScout2/resources/voice-providers.json")
        .WithUserAgent("OSMScout2DemoApp", "v1");

    if (!builder.Init())
    {
        qCritical("Cannot initialize map");
        return 1;
    }

    int result = 0;
    {
        QQmlApplicationEngine engine;
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                         &app, []() { QCoreApplication::exit(-1); },
                         Qt::QueuedConnection);
        engine.loadFromModule("com.github.pamugk.urania", "Main");
        result = app.exec();
    }
    osmscout::OSMScoutQt::FreeInstance();

    return result;
}
