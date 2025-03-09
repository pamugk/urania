#include <QApplication>
#include <QSettings>

#include <osmscout/log/Logger.h>

enum LogLevel
{
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

static LogLevel parseLogLevel(const QString &logLevel)
{
    auto upperCaseLogLevel = logLevel.toUpper();
    if (upperCaseLogLevel == "DEBUG")
    {
        return LogLevel::DEBUG;
    }

    if (upperCaseLogLevel == "INFO")
    {
        return LogLevel::INFO;
    }

    if (upperCaseLogLevel == "WARNING")
    {
        return LogLevel::WARNING;
    }

    if (upperCaseLogLevel == "ERROR")
    {
        return LogLevel::ERROR;
    }

    return LogLevel::WARNING;
}

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Urania"));
    QApplication::setOrganizationDomain(QStringLiteral("pamugk.github.com"));
    QApplication::setDesktopFileName(QStringLiteral("com.github.pamugk.urania"));

    QSettings appSettings;
    QString logLevelName = appSettings.value(QLatin1StringView("libosmscout/logLevel"),
                                             QStringLiteral("WARNING")
    ).toString();

    qDebug() << "Setting libosmscout logging to level: " << logLevelName << '\n';

    LogLevel logEnv = parseLogLevel(logLevelName);

    osmscout::log.Debug(logEnv >= LogLevel::DEBUG);
    osmscout::log.Info(logEnv >= LogLevel::INFO);
    osmscout::log.Warn(logEnv >= LogLevel::WARNING);
    osmscout::log.Error(logEnv >= LogLevel::ERROR);

    MainWindow window;
    window.show();

    return app.exec();
}
