#include "annotation_widget.hpp"
#include "annotation_box.hpp"

#include <QList>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonValue>

#include <memory>

AnnotationWidget::AnnotationWidget(QWidget* parent)
    : QScrollArea(parent)
{
    m_container = new QGroupBox(this);
    for (qint16 ith = 0; ith < m_defaultNumOfAnns; ith++)
    {
        m_annotation_boxes.append(new AnnotationBox(m_container));
    }

    m_layout = new QVBoxLayout(m_container);
    m_layout->setAlignment(Qt::AlignTop);
    for (auto& ann: m_annotation_boxes)
    {
        ann->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        m_layout->addWidget(ann);
    }

    setWidget(m_container);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setWidgetResizable(true);
    setAlignment(Qt::AlignCenter);
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

void AnnotationWidget::setData(QJsonArray const& data) const
{
    for (qsizetype ith = 0; ith < data.size(); ith++)
    {
        m_annotation_boxes[ith]->setAnnotation(data[ith].toObject());
    }
}

AnnotationWidget::~AnnotationWidget()
{

}

void AnnotationWidget::addAnnotation()
{
    AnnotationBox* new_box = new AnnotationBox();
    new_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_annotation_boxes.append(new_box);
    m_layout->addWidget(new_box);
}
