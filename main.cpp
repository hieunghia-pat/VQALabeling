#include "mainwindow.hpp"

#include <QApplication>
#include <QTableView>
#include <QListView>
#include <QIcon>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/media/icons/logo-uit.ico"));

    MainWindow window;
    window.show();

    return app.exec();
}
