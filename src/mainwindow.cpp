#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <osmscoutclientqt/OSMScoutQt.h>

#include "mappage.h"
#include "startuppage.h"

static bool configureMap(const QString &mapPath, const QString &styleSheetPath);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->exitAction, &QAction::triggered,
            this, []{ QApplication::exit(); });
    connect(ui->aboutQtAction, &QAction::triggered,
            this, []{ QApplication::aboutQt(); });

    osmscout::OSMScoutQt::RegisterQmlTypes();

    QSettings appSettings;
    auto selectedMapFolder = appSettings.value(QLatin1StringView("libosmscout/mapsFolder")).toString();
    auto selecteStyleSheet = appSettings.value(QLatin1StringView("libosmscout/styleSheet")).toString();

    bool mapConfigured = false;
    if (!selectedMapFolder.isEmpty() && !selecteStyleSheet.isEmpty())
    {
        mapConfigured = configureMap(selectedMapFolder, selecteStyleSheet);
    }

    if (mapConfigured)
    {
        setCentralWidget(new MapPage(this));
    }
    else
    {
        auto startUpPage = new StartupPage(this);
        connect(startUpPage, &StartupPage::mapConfigured,
                this, [this](const QString &mapsPath, const QString &styleSheetPath)
        {
            if (configureMap(mapsPath, styleSheetPath))
            {
                QSettings appSettings;
                appSettings.setValue(QLatin1StringView("libosmscout/mapsFolder"), mapsPath);
                appSettings.setValue(QLatin1StringView("libosmscout/styleSheet"), styleSheetPath);
                setCentralWidget(new MapPage(this));
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

bool configureMap(const QString &mapsPath, const QString &styleSheetPath)
{
    QFileInfo stylesheetFile(styleSheetPath);
    QSettings appSettings;
    return osmscout::OSMScoutQt::NewInstance()
        .WithStyleSheetDirectory(stylesheetFile.dir().path())
        .WithStyleSheetFile(stylesheetFile.fileName())
        .WithBasemapLookupDirectory(mapsPath)
        .WithMapLookupDirectories({mapsPath})
        .WithUserAgent(QApplication::applicationName(), QApplication::applicationVersion())
        .Init();
}
