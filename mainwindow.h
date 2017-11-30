#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json/json.hpp"

using json = nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnInitizalize_clicked();

    void on_btnGetStoreInfo_clicked();

    void on_btnGetStoreProduct_clicked();

public slots:
    void onStoreProducts(const json &storeProducts);
    void onStoreInformation(const json &storeInformation);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
