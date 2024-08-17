#include <QString>
#include <QMap>
#include <QPair>

const QString NONE = "null";
const QString CAPTION = "caption";
const QString LABEL = "label";
const QMap<qint16, QString> label2text({
    QPair<qint16, QString>(false, "False"),
    QPair<qint16, QString>(true, "True")
});
const QMap<QString, qint16> text2label({
    QPair<QString, qint16>("False", false),
    QPair<QString, qint16>("True", true)
});

const QString FILEPATH = "filepath";
const QString IMAGE_1 = "image_1";
const QString IMAGE_2 = "image_2";
const QString ANNOTATIONS = "annotations";
const QString DELETE = "delete";

