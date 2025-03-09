#include "startuppage.h"
#include "ui_startuppage.h"

#include <QFileDialog>
#include <QStandardPaths>

StartupPage::StartupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupPage)
{
    ui->setupUi(this);

    connect(ui->chooseMapsFolderButton, &QPushButton::clicked,
            this, [this]()
    {
        auto mapsFolderPath = QFileDialog::getExistingDirectory(this,
            tr("Select Maps Folder"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        if (!mapsFolderPath.isNull())
        {
            selectedMapsFolderPath = mapsFolderPath;
            ui->mapsFolderLineEdit->setText(mapsFolderPath);
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
            ui->okButton->setEnabled(!selectedMapsFolderPath.isEmpty());
        }
    });
    connect(ui->okButton, &QPushButton::clicked,
            this, [this]()
    {
        emit mapConfigured(selectedMapsFolderPath, selectedStyleSheetFilePath);
    });
}

StartupPage::~StartupPage()
{
    delete ui;
}
