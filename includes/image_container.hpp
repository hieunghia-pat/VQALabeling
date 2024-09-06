#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QPushButton>

#include "annotation_widget.hpp"
#include "image_widget.hpp"

class ImageContainer : public QWidget
{
    Q_OBJECT
public:
    // methods
    ImageContainer(QWidget *parent = nullptr);
    ~ImageContainer();

    // attributes
    ImageWidget* m_image_widget_top = nullptr;
    ImageWidget* m_image_widget_bottom = nullptr;
    AnnotationWidget* m_annotation_widget = nullptr;

    // Layout for the two Image Widget
    QHBoxLayout* m_imageLayout = nullptr;
};

#endif // IMAGE_CONTAINER_H
