#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <QWidget>
#include <QHBoxLayout>
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
    ImageWidget* m_image_widget = nullptr;
    AnnotationWidget* m_annotation_widget = nullptr;
    
    QHBoxLayout* m_layout = nullptr;
};

#endif // CONTAINER_HPP
