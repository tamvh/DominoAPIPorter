#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dominoapi.h"
#include <QDebug>

DominoAPI dominoApi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnInitizalize_clicked()
{
    dominoApi.initizalize("TestingAndSupport", "supp0rtivemeasures", "http://113.161.67.179:59101");
    QObject::connect(&dominoApi, SIGNAL(eventStoreProducts(json)), this, SLOT(onStoreProducts(json)), Qt::UniqueConnection);
    QObject::connect(&dominoApi, SIGNAL(eventStoreInformation(json)), this, SLOT(onStoreInformation(json)), Qt::UniqueConnection);
}

void MainWindow::on_btnGetStoreInfo_clicked()
{
    dominoApi.getStoreInformation(55555);
}

void MainWindow::on_btnGetStoreProduct_clicked()
{
    dominoApi.getStoreProducts(55555);
}

void MainWindow::onStoreProducts(const json &storeProducts)
{
    qDebug() << "store product list = " << QString::fromStdString(storeProducts.dump());
//    std::cout << "store product list = " << storeProducts.dump() << std::endl;
}

void MainWindow::onStoreInformation(const json &storeInformation)
{
    std::cout << "store information = " << storeInformation.dump() << std::endl;
}
