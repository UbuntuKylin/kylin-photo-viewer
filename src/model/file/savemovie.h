#ifndef SAVEMOVIE_H
#define SAVEMOVIE_H

#include <QThread>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QSemaphore>
#include "global/variable.h"

using namespace cv;

class SaveMovie : public QThread
{
    Q_OBJECT

signals:
    void saveMovieFinish(const QString &path);

public:
    SaveMovie(QList<Mat> *list, const int &fps, const QString &savepath);
    QSemaphore *m_sema = nullptr;
    QSemaphore *m_sema2 = nullptr;

protected:
    void run();

private:
    QList<Mat> * m_list;
    int m_fps;
    QString m_savepath;
    bool m_special;
    QProcess *m_process = nullptr;
    void processLog();
};

#endif // SAVEMOVIE_H
