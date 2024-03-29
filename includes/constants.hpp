#include <QString>
#include <QMap>
#include <QPair>

const QString QUESTION = "question";
const QString FOREIGN_QUESTION = "foreign_question";
// const QString QUESTION_TYPE = "question-type";

const QString ANSWER = "answer";
const QString FOREIGN_ANSWER = "foreign_answer";
// const QString ANSWER_TYPE = "answer-type";

// const QString TEXT_QA = "text-QA";
// const QString STATE_QA = "state-QA";
// const QString ACTION_QA = "action-QA";
const QString QA_TYPE = "QA-type";

const QString FILEPATH = "filepath";
const QString FILENAME = "filename";
const QString ANNOTATIONS = "annotations";
const QString DELETE = "delete";

const QMap<qint16, QString> QA_types({
    QPair<qint16, QString>(0, "text-QA"),
    QPair<qint16, QString>(1, "state-QA"),
    QPair<qint16, QString>(2, "action-QA")
});

const QMap<qint16, QString> question_type({
    QPair<qint16, QString>(0, "What"),
    QPair<qint16, QString>(1, "Who"),
    QPair<qint16, QString>(2, "Where"),
    QPair<qint16, QString>(3, "When"),
    QPair<qint16, QString>(4, "How"),
    QPair<qint16, QString>(5, "Other")
});

const QMap<qint16, QString> answer_type({
    QPair<qint16, QString>(0, "Word"),
    QPair<qint16, QString>(1, "Phrase"),
    QPair<qint16, QString>(2, "Sentence")
});

const QMap<qint16, QString> check_state({
    QPair<qint16, QString>(true, "true"),
    QPair<qint16, QString>(false, "false")
});

const QString None = "None";