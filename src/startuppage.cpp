#include "startuppage.h"
#include "ui_startuppage.h"

#include <QFileDialog>
#include <QStandardPaths>

StartupPage::StartupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupPage)
{
    ui->setupUi(this);

    connect(ui->chooseMapFolderButton, &QPushButton::clicked,
            this, [this]()
    {
        auto mapFolderPath = QFileDialog::getExistingDirectory(this,
            tr("Select Map Folder"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        if (!mapFolderPath.isNull())
        {
            selectedMapFolderPath = mapFolderPath;
            ui->mapFolderLineEdit->setText(mapFolderPath);
            ui->okButton->setEnabled(!selectedStyleSheetFilePath.isEmpty());
        }
    });
    connect(ui->chooseStylesheetButton, &QPushButton::clicked,
            this, [this]()
    {
        auto styleSheetFilePath = QFileDialog::getOpenFileName(this,
            tr("Select Style Sheet"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            tr("Libosmscout Style Sheet (*.oss)"));
        if (!styleSheetFilePath.isNull())
        {
            selectedStyleSheetFilePath = styleSheetFilePath;
            ui->stylesheetLineEdit->setText(styleSheetFilePath);
            ui->okButton->setEnabled(!selectedMapFolderPath.isEmpty());
        }
    });
    connect(ui->okButton, &QPushButton::clicked,
            this, [this]()
    {
        emit mapConfigured(selectedMapFolderPath, selectedStyleSheetFilePath);
    });
}

StartupPage::~StartupPage()
{
    delete ui;
}
