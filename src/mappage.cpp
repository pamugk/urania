#include "mappage.h"
#include "ui_mappage.h"

#include "mapwidget.h"
#include <osmscoutclientqt/SearchLocationModel.h>

MapPage::MapPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPage)
{
    ui->setupUi(this);
    auto map = new MapWidget(ui->mapContainerWidget);
    map->lower();
    this->map = map;
    ui->infoPane->setVisible(false);

    auto searchModel = new osmscout::LocationListModel(ui->searchResultsListView);
    connect(ui->searchLineEdit, &QLineEdit::editingFinished,
            searchModel, [this, searchModel]()
    {
        if (!ui->searchLineEdit->text().isEmpty())
        {
            searchModel->setPattern(ui->searchLineEdit->text());
        }
        else if (!searchModel->isSearching())
        {
            searchModel->removeRows(0, searchModel->rowCount());
            ui->searchResultsStack->setCurrentWidget(ui->searchResultsInitialPage);
        }
    });
    connect(searchModel, &osmscout::LocationListModel::SearchingChanged,
            this, [this, searchModel](bool searching)
    {
        if (searching)
        {
            ui->searchResultsStack->setCurrentWidget(ui->searchResultsLoadingPage);
        }
        else if (searchModel->rowCount() == 0)
        {
            ui->searchResultsStack->setCurrentWidget(ui->searchResultsNothingFoundPage);
        }
        else
        {
            ui->searchResultsListView->scrollToTop();
            ui->searchResultsStack->setCurrentWidget(ui->searchResultsDataPage);
        }
    });
    connect(map, &MapWidget::centerChanged,
            searchModel, [searchModel](const osmscout::GeoCoord &newCenter)
    {
        searchModel->SetLat(newCenter.GetLat());
        searchModel->SetLon(newCenter.GetLon());
    });
    ui->searchResultsListView->setModel(searchModel);

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
