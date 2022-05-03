#include "mainwindow.hpp"
#include "exception.hpp"

#include <QApplication>
#include <QTableView>
#include <QListView>
#include <QIcon>
#include <QMessageLogContext>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDateTime>

#include <filesystem>
#include <exception>
#include <iostream>

namespace fs = std::filesystem;

fs::path project_path(QDir::currentPath().toStdString());
fs::path log_folder("logs");
fs::path log_file_path;

void messageHandler(QtMsgType type, QMessageLogContext const& context, QString const &msg)
{
    static bool create_new_log_file = true;

    if (!QDir(project_path / log_folder).exists())
        QDir(project_path).mkdir("logs");

    if (create_new_log_file) // the application is first started
    {
        log_file_path = fs::path(QString("VQALabeling[%1].log").arg(QDateTime::currentDateTime().toString()).toStdString());
        create_new_log_file = false;
    }

    QFile log_file(project_path / log_folder / log_file_path);

    if (log_file.exists())
        log_file.open(QIODeviceBase::Append);
    else
        log_file.open(QIODeviceBase::WriteOnly);
    
    QTextStream log_stream(&log_file);
    
    switch (type)
    {
        case QtInfoMsg:

        {
            const char* localMessage = (QString("INFO - %1 - %2").arg(QDateTime::currentDateTime().toString()).arg(msg) + "\n").toLocal8Bit().constData();
            log_stream << localMessage;
            break;
        }
        case QtDebugMsg:
        {
            const char* localMessage = (QString("DEBUG - %1 - %2").arg(QDateTime::currentDateTime().toString()).arg(msg) + "\n").toLocal8Bit().constData();
            log_stream << localMessage;
            break;
        }
    }

    log_file.close();
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/media/icons/logo-uit.ico"));

    MainWindow window;
    window.show();

    return app.exec();
}
