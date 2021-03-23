#ifndef SAVEMOVIE_H
#define SAVEMOVIE_H

#include <QThread>
#include <QFileInfo>
#include <QDir>
#include "global/variable.h"

using namespace cv;

class SaveMovie : public QThread
{
public:
    SaveMovie(QList<Mat> *list,const int &fps, const QString &savepath, const QString &type, bool special = false);

protected:
    void run();

private:
    QList<Mat> * _list;
    int _fps;
    QString _savepath;
    QString _type;
    bool _special;
};

#endif // SAVEMOVIE_H
