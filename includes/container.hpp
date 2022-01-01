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
    Container(QWidget *parent = nullptr);

    ImageWidget* imageWidget() const;
    AnnotationWidget* annotationWidget() const;
    QHBoxLayout* layout() const;

    ~Container();

private:
    ImageWidget* m_image_widget = nullptr;
    AnnotationWidget* m_annotation_widget = nullptr;
    QPushButton* m_add_anntation_button = nullptr;
    
    QVBoxLayout* m_annotation_layout = nullptr;
    QHBoxLayout* m_layout = nullptr;
};

#endif // CONTAINER_HPP
