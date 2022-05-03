######################################################################
# Automatically generated by qmake (3.1) Wed Dec 29 10:27:36 2021
######################################################################

TEMPLATE = app
TARGET = VQALabeling
INCLUDEPATH += includes
QT += widgets
RC_ICONS = media/icons/logo-uit.ico
ICON = media/icons/logo-uit.ico
CONFIG += c++17

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += includes/annotation_box.hpp \
           includes/annotation_widget.hpp \
           includes/container.hpp \
           includes/exception.hpp \
           includes/image_widget.hpp \
           includes/mainwindow.hpp \
           includes/open_folder_dialog.hpp \
           includes/save_json_dialog.hpp \
           includes/save_notification_dialog.hpp \
           includes/selection_box.hpp \
           includes/constants.hpp
SOURCES += main.cpp \
           sources/annotation_box.cpp \
           sources/annotation_widget.cpp \
           sources/container.cpp \
           sources/exception.cpp \
           sources/image_widget.cpp \
           sources/mainwindow.cpp \
           sources/open_folder_dialog.cpp \
           sources/save_json_dialog.cpp \
           sources/save_notificationn_dialog.cpp \
           sources/selection_box.cpp
RESOURCES += resources.qrc