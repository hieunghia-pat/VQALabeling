#include <QString>
#include <QMap>
#include <QPair>

const QString NONE = "null";
const QString CAPTION = "caption";
const QString CAPTION_TYPE = "caption-type";

const QString FILEPATH = "filepath";
const QString FILENAME = "filename";
const QString ANNOTATIONS = "annotations";
const QString DELETE = "delete";

const QMap<qint16, QString> caption_types({
    QPair<qint16, QString>(0, "multi-sarcasm"),
    QPair<qint16, QString>(1, "image-sarcasm"),
    QPair<qint16, QString>(2, "text-sarcasm"),
    QPair<qint16, QString>(2, "not-sarcasm"),
    QPair<qint16, QString>(2, "image-not-sarcasm"),
    QPair<qint16, QString>(2, "text-not-sarcasm")
});

const QMap<qint16, QString> check_state({
    QPair<qint16, QString>(true, "true"),
    QPair<qint16, QString>(false, "false")
});

