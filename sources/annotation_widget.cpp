#include "annotation_widget.hpp"
#include "annotation_box.hpp"

#include <QList>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QPushButton>
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

//    m_addButton = new QPushButton(m_container);
//    m_addButton->setText("Add Annotation");

    m_layout = new QVBoxLayout(m_container);
    for (auto& ann: m_annotation_boxes)
    {
        m_layout->addWidget(ann);
    }
    // m_layout->addWidget(m_addButton);

    setWidget(m_container);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setWidgetResizable(true);
    setAlignment(Qt::AlignCenter);

    // QObject::connect(m_addButton, &QPushButton::clicked, this, &AnnotationWidget::addAnnotation);
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

//void AnnotationWidget::addAnnotation()
//{
//    QuestionAnswerBox* new_box = new QuestionAnswerBox(m_container);
//    m_layout->removeWidget(m_addButton);
//    m_layout->addWidget(new_box);
//    m_layout->addWidget(m_addButton);
//    m_listOfAnns.append(new_box);
//}
