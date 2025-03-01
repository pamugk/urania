#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mapwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidgetLayout->addWidget(new MapWidget(ui->centralwidget));
}

MainWindow::~MainWindow()
{
    delete ui;
}
