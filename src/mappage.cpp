#include "mappage.h"
#include "ui_mappage.h"

#include "mapwidget.h"

MapPage::MapPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPage)
{
    ui->setupUi(this);
    auto map = new MapWidget(ui->mapContainerWidget);
    map->lower();
    this->map = map;
    ui->infoPane->setVisible(false);

    connect(ui->zoomInButton, &QPushButton::clicked,
            map, &MapWidget::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked,
            map, &MapWidget::zoomOut);

    connect(ui->toggleInfoButton, &QPushButton::toggled,
            this, [this](bool checked)
    {
        if (checked)
        {
            ui->toggleInfoButton->setToolTip(QString());
            ui->searchButton->setText(tr("Search"));
            ui->searchButton->setToolTip(QString());
        }
        else
        {
            ui->toggleInfoButton->setToolTip(tr("Expand"));
            ui->searchButton->setText(QString());
            ui->searchButton->setToolTip(tr("Search"));
        }
    });
    connect(ui->searchButton, &QPushButton::toggled,
            this, [this](bool checked)
    {
       if (checked)
       {
           ui->infoPaneStack->setCurrentWidget(ui->infoPaneSearchPage);
           ui->infoPane->setVisible(true);
       }
       else
       {
           ui->infoPane->setVisible(false);
       }
    });
    connect(ui->infoPaneSearchPageCloseButton, &QPushButton::clicked,
            this, [this]()
    {
        ui->searchButton->setChecked(false);
    });
}

MapPage::~MapPage()
{
    delete ui;
}

void MapPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->mapControlsWidget->move(
        ui->mapContainerWidget->width() - ui->mapControlsWidget->width() - 20,
        (ui->mapContainerWidget->height() - ui->mapControlsWidget->height()) / 2
    );
    map->setFixedSize(ui->mapContainerWidget->size());
}
