#include "annotation_widget.hpp"
#include "annotation_box.hpp"

#include <QList>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonValue>
#include <QDialog>
#include <QKeyEvent>

#include <memory>
#include <iostream>

AnnotationWidget::AnnotationWidget(QWidget* parent)
    : QScrollArea(parent)
{
    m_container = new QGroupBox(this);
    for (qint16 ith = 0; ith < m_defaultNumOfAnns; ith++)
    {
        m_annotation_boxes.append(new AnnotationBox(ith, this, this));
        m_annotation_boxes[ith]->setIndex(ith);
    }

    m_layout = new QVBoxLayout(m_container);
    m_layout->setAlignment(Qt::AlignTop);
    for (auto& ann: m_annotation_boxes)
    {
        ann->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        QObject::connect(ann, &AnnotationBox::contentChanged, this, &AnnotationWidget::haveAdjusted);
        m_layout->addWidget(ann);
    }

    setWidget(m_container);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setWidgetResizable(true);
    setAlignment(Qt::AlignCenter);

    QObject::connect(this, &AnnotationWidget::changedNumberOfAnnotations, this, &AnnotationWidget::reassignIndex);
}

std::shared_ptr<QJsonArray> AnnotationWidget::data() const
{
    std::shared_ptr<QJsonArray> annotations = std::make_shared<QJsonArray>();

    for (auto& annotation_box: m_annotation_boxes)
    {
        annotations->append(*(annotation_box->annotation()));
    }

    return annotations;
}

QVBoxLayout *AnnotationWidget::layout()
{
    return m_layout;
}

void AnnotationWidget::setData(QJsonArray const& data)
{
    while (m_annotation_boxes.size() > data.size())
        deleteAnnotation(m_annotation_boxes.size()-1);

    while (m_annotation_boxes.size() < data.size())
        addAnnotation(m_annotation_boxes.size()-1);

    for (qsizetype ith = 0; ith < data.size(); ith++)
        m_annotation_boxes[ith]->setAnnotation(data[ith].toObject());
}

bool AnnotationWidget::isEmpty()
{
    for (auto& annotation_box: m_annotation_boxes)
        if (!annotation_box->isEmpty())
            return false;
    return true;
}

AnnotationWidget::~AnnotationWidget()
{

}

void AnnotationWidget::addAnnotation(qsizetype ith)
{
    ith += 1;
    AnnotationBox* new_box = new AnnotationBox(ith, this, this);
    new_box->setIndex(ith);
    new_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    QObject::connect(new_box, &AnnotationBox::contentChanged, this, &AnnotationWidget::haveAdjusted);
    m_annotation_boxes.insert(ith, new_box);
    m_layout->insertWidget(ith, new_box);
    
    emit changedNumberOfAnnotations();
}

void AnnotationWidget::deleteAnnotation(qsizetype ith)
{
    if (m_annotation_boxes.size() == total_initial_annotations)
        return;

    AnnotationBox* box = m_annotation_boxes[ith];
    m_layout->removeWidget(box);
    box->setParent(nullptr);
    m_layout->update();

    m_annotation_boxes.removeAt(ith);
    delete box;

    emit changedNumberOfAnnotations();
}

void AnnotationWidget::reassignIndex()
{
    for (qsizetype ith = 0; ith < m_annotation_boxes.size(); ith++)
        m_annotation_boxes[ith]->setIndex(ith);
}

void AnnotationWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
        emit backImage();
    
    if (event->key() == Qt::Key_Right)
        emit nextImage();

    QScrollArea::keyPressEvent(event); // pass the event to the base class
}