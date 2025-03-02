#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <osmscoutclientqt/OSMScoutQt.h>

#include <QSettings>

#include "mapwidget.h"
#include "startuppage.h"

static bool configureMap(const QString &mapPath, const QString &styleSheetPath);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    osmscout::OSMScoutQt::RegisterQmlTypes();

    QSettings appSettings;
    auto selectedMapFolder = appSettings.value(QLatin1StringView("libosmscout/mapFolder")).toString();
    auto selecteStyleSheet = appSettings.value(QLatin1StringView("libosmscout/styleSheet")).toString();

    bool mapConfigured = false;
    if (!selectedMapFolder.isEmpty() && !selecteStyleSheet.isEmpty())
    {
        mapConfigured = configureMap(selectedMapFolder, selecteStyleSheet);
    }

    if (mapConfigured)
    {
        setCentralWidget(new MapWidget(this));
    }
    else
    {
        auto startUpPage = new StartupPage(this);
        connect(startUpPage, &StartupPage::mapConfigured,
                this, [this](const QString &mapPath, const QString &styleSheetPath)
        {
            if (configureMap(mapPath, styleSheetPath))
            {
                QSettings appSettings;
                appSettings.setValue(QLatin1StringView("libosmscout/mapFolder"), mapPath);
                appSettings.setValue(QLatin1StringView("libosmscout/styleSheet"), styleSheetPath);
                setCentralWidget(new MapWidget(this));
            }
            else
            {
                // Notify that something went wrong
            }
        });
        setCentralWidget(startUpPage);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool configureMap(const QString &mapPath, const QString &styleSheetPath)
{
    osmscout::OSMScoutQtBuilder builder = osmscout::OSMScoutQt::NewInstance();

    QDir dbDir(mapPath);
    QStringList mapLookupDirectories;

    mapLookupDirectories << dbDir.canonicalPath();

    if (dbDir.cdUp())
    {
        if (dbDir.cd("world"))
        {
          builder.WithBasemapLookupDirectory(dbDir.absolutePath());
        }
    }

    QFileInfo stylesheetFile(styleSheetPath);

    builder
        .WithStyleSheetDirectory(stylesheetFile.dir().path())
        .WithStyleSheetFile(stylesheetFile.fileName())
        //.WithMapLookupDirectories(mapLookupDirectories)
        ;

    return builder.Init();
}
