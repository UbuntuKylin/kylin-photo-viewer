#ifndef SAVEMOVIE_H
#define SAVEMOVIE_H

#include <QThread>
#include <QFileInfo>
#include <QDir>
#include "global/variable.h"

using namespace cv;

class SaveMovie : public QThread
{
    Q_OBJECT

signals:
    void saveMovieFinish(const QString &path);

public:
    SaveMovie(QList<Mat> *list,const int &fps, const QString &savepath, const QString &type);

protected:
    void run();

private:
    QList<Mat> * m_list;
    int m_fps;
    QString m_savepath;
    QString m_type;
    bool m_special;
};

#endif // SAVEMOVIE_H
