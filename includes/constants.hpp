#include <QString>
#include <QMap>
#include <QPair>

const QString QUESTION = "question";
const QString ANSWER = "answer";

const QString FILEPATH = "filepath";
const QString FILENAME = "filename";
const QString ANNOTATIONS = "annotations";
const QString DELETE = "delete";

const QMap<qint16, bool> check_state({
    QPair<qint16, bool>(Qt::Checked, true),
    QPair<qint16, bool>(Qt::Unchecked, false)
});

const QString ANNOTATION_FILE = "annotations.json";

const QString None = "None";
