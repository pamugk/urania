#include <QApplication>
#include <QStandardPaths>

#include <KIconTheme>

#include <osmscout/log/Logger.h>

#include "mapwidget.h"

enum LogLevel
{
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

static LogLevel parseLogLevel(const QString &env)
{
  if (env.toUpper() == "DEBUG")
  {
    return LogLevel::DEBUG;
  }

  if (env.toUpper() == "INFO")
  {
    return LogLevel::INFO;
  }

  if (env.toUpper() == "WARNING")
  {
    return LogLevel::WARNING;
  }

  if (env.toUpper() == "ERROR")
  {
    return LogLevel::ERROR;
  }

  return LogLevel::WARNING;
}

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Urania"));
    QApplication::setDesktopFileName(QStringLiteral("com.github.pamugk.urania"));

    osmscout::OSMScoutQt::RegisterQmlTypes();

    QString logLevelName = QProcessEnvironment::systemEnvironment().value("OSMSCOUT_LOG", "WARNING");

    qDebug() << "Setting libosmscout logging to level: " << logLevelName << '\n';

    LogLevel logEnv = parseLogLevel(logLevelName);

    osmscout::log.Debug(logEnv >= LogLevel::DEBUG);
    osmscout::log.Info(logEnv >= LogLevel::INFO);
    osmscout::log.Warn(logEnv >= LogLevel::WARNING);
    osmscout::log.Error(logEnv >= LogLevel::ERROR);

    osmscout::OSMScoutQtBuilder builder = osmscout::OSMScoutQt::NewInstance();

    auto mapsDir = QDir(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                               QStringLiteral("Карты"),
                                               QStandardPaths::LocateDirectory));
    QDir dbDir(mapsDir.absoluteFilePath("volga-fed-district"));
    QStringList mapLookupDirectories;

    mapLookupDirectories << dbDir.canonicalPath();


    if (dbDir.cdUp())
    {
        if (dbDir.cd("world"))
        {
          builder.WithBasemapLookupDirectory(dbDir.absolutePath());
        }
    }

    auto gitDir = QDir(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                          QStringLiteral("git"),
                                          QStandardPaths::LocateDirectory));
    QFileInfo stylesheetFile(gitDir.absoluteFilePath("libosmscout/stylesheets/standard.oss"));

    builder
        .WithStyleSheetDirectory(stylesheetFile.dir().path())
        .WithStyleSheetFile(stylesheetFile.fileName())
        .WithIconDirectory(gitDir.absoluteFilePath("libosmscout/OSMScout2/pics"))
        .WithMapLookupDirectories(mapLookupDirectories)
        .AddOnlineTileProviders(gitDir.absoluteFilePath("libosmscout/OSMScout2/resources/online-tile-providers.json"))
        .AddMapProviders(gitDir.absoluteFilePath("libosmscout/OSMScout2/resources/map-providers.json"))
        .AddVoiceProviders(gitDir.absoluteFilePath("libosmscout/OSMScout2/resources/voice-providers.json"))
        .WithUserAgent(QApplication::applicationName(), QApplication::applicationVersion());

    if (!builder.Init())
    {
        qCritical("Cannot initialize map");
        return 1;
    }

    MapWidget map;
    map.show();
    return app.exec();
}
