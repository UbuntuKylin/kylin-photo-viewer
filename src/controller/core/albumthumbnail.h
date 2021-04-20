#ifndef ALBUMTHUMBNAIL_H
#define ALBUMTHUMBNAIL_H

#include <QThread>
#include <QStyleOptionViewItem>
#include <QPainter>
#include "global/variable.h"
#include "model/processing/processing.h"
#include "model/file/file.h"

class AlbumThumbnail : public QThread
{
    Q_OBJECT

signals:
    void albumFinish(QVariant var);

public:
    AlbumThumbnail(const QString &path);
    void run();

private:
    QString m_path;
    QPixmap changImageSize(const QPixmap &pix);
};

#endif // ALBUMTHUMBNAIL_H
