#ifndef ALBUMTHUMBNAIL_H
#define ALBUMTHUMBNAIL_H

#include <QThread>
#include "global/variable.h"
#include "model/processing/processing.h"
#include "model/file/file.h"

class AlbumThumbnail : public QThread
{
    Q_OBJECT

signals:
    void albumFinish(QVariant var);

public:
    AlbumThumbnail(const int &type , const QString &path);
    void run();

private:
    int m_type;
    QString m_path;
};

#endif // ALBUMTHUMBNAIL_H
