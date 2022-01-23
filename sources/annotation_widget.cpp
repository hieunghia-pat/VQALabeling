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
        m_annotation_boxes.append(new AnnotationBox(ith, this, this));
    }

    m_layout = new QVBoxLayout(m_container);
    m_layout->setAlignment(Qt::AlignTop);
    for (auto& ann: m_annotation_boxes)
    {
        ann->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        
        QObject::connect(ann->m_questionLineEdit, &QLineEdit::textChanged, this, &AnnotationWidget::haveAdjusted);
        QObject::connect(ann->m_answerLineEdit, &QLineEdit::textChanged, this, &AnnotationWidget::haveAdjusted);
        QObject::connect(ann->m_add_button, &QPushButton::clicked, this, &AnnotationWidget::haveAdjusted);
        QObject::connect(ann->m_del_button, &QPushButton::clicked, this, &AnnotationWidget::haveAdjusted);
        QObject::connect(ann->m_selection_box, &QComboBox::currentIndexChanged, this, &AnnotationWidget::haveAdjusted);
        
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

void AnnotationWidget::setData(QJsonArray const& data)
{
    if (data.size() < m_annotation_boxes.size())
        for (qsizetype ith = data.size(); ith < m_annotation_boxes.size(); ith++)
            deleteAnnotation(ith);

    for (qsizetype ith = 0; ith < data.size(); ith++)
    {
        m_annotation_boxes[ith]->setAnnotation(data[ith].toObject());
    }
}

AnnotationWidget::~AnnotationWidget()
{

}

void AnnotationWidget::addAnnotation(qsizetype ith)
{
    ith += 1;
    AnnotationBox* new_box = new AnnotationBox(ith, this, this);
    m_annotation_boxes.insert(ith, new_box);
    m_annotation_boxes[ith]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_layout->insertWidget(ith, new_box);
}

void AnnotationWidget::deleteAnnotation(qsizetype ith)
{
    if (m_annotation_boxes.size() == total_initial_annotations)
        return;

    AnnotationBox* box = m_annotation_boxes[ith];
    m_layout->removeWidget(box);
    box->setParent(nullptr);
    m_layout->update();

    m_annotation_boxes.remove(ith);
}