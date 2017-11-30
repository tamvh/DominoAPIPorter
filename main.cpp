#include "mainwindow.h"
#include <QApplication>
#include "json/json.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    nlohmann::json json;
    json["et"] = nullptr;

    nlohmann::json &temp = json["et"];
    temp["abc"] = "123";


    std::cout << json.dump() << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
