#include "mappage.h"
#include "ui_mappage.h"

#include "mapwidget.h"

MapPage::MapPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPage)
{
    ui->setupUi(this);

    auto map = new MapWidget(ui->mapContainerWidget);
    connect(ui->zoomInButton, &QPushButton::clicked,
            map, &MapWidget::zoomIn);
    connect(ui->zoomOutButton, &QPushButton::clicked,
            map, &MapWidget::zoomOut);
    map->lower();
    this->map = map;
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
