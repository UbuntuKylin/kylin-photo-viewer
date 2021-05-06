#ifndef LOADMOVIE_H
#define LOADMOVIE_H

#include <QThread>
#include <QMovie>
#include "global/variable.h"

using namespace cv;

class LoadMovie : public QThread
{
    Q_OBJECT
public:
    LoadMovie(QList<Mat> *list, QMovie *movie);

protected:
    void run();

private:
    QList<Mat> *m_list = nullptr;
    QMovie *m_movie = nullptr;
};

#endif // LOADMOVIE_H
