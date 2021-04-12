#include "albumthumbnail.h"

AlbumThumbnail::AlbumThumbnail(const QString &path)
{
    m_path = path;
}

void AlbumThumbnail::run()
{
    ImageAndInfo package;
    MatAndFileinfo maf = File::loadImage(m_path,IMREAD_REDUCED_COLOR_8);
    if (maf.mat.data) {
        QPixmap pix = Processing::converFormat(maf.mat);
        pix = Processing::resizePix(pix,Variable::ALBUM_IMAGE_SIZE);
        package.image = changImageSize(pix);
        qDebug()<<"package.image"<<package.image;
    }
    package.info = maf.info;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit albumFinish(var);
}

QPixmap AlbumThumbnail::changImageSize(const QPixmap &pix)
{
    if (pix.size() == Variable::ALBUM_IMAGE_SIZE) {
        return pix;
    }

    int w = 0;
    int h = 0;

    w = Variable::ALBUM_IMAGE_SIZE.width() - pix.width();
    h = Variable::ALBUM_IMAGE_SIZE.height() - pix.height();

    QPixmap newPix(Variable::ALBUM_IMAGE_SIZE);
    QPainter painter(&newPix);
    painter.drawPixmap(w/2,h/2,pix);
    return newPix;
}
