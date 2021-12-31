#include "mainwindow.hpp"

#include <QApplication>
#include <QTableView>
#include <QListView>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
