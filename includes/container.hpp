#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QPushButton>

#include "annotation_widget.hpp"
#include "image_widget.hpp"

class Container : public QWidget
{
    Q_OBJECT
public:
    // methods
    Container(QWidget *parent = nullptr);
    ~Container();

    // attributes
    ImageWidget* m_image_widget_top = nullptr;
    ImageWidget* m_image_widget_bottom = nullptr;
    AnnotationWidget* m_annotation_widget = nullptr;

    // Layout for the two Image Widget
    QVBoxLayout* m_vlayout = nullptr;
    // Layout for the whole container
    QHBoxLayout* m_hlayout = nullptr;
};

#endif // CONTAINER_HPP
