#include "albumthumbnail.h"

AlbumThumbnail::AlbumThumbnail(const int &type, const QString &path)
{
    m_type = type;
    m_path = path;
}

void AlbumThumbnail::run()
{
    ImageAndInfo package;
    package.type = m_type;

    MatAndFileinfo maf = File::loadImage(m_path,IMREAD_REDUCED_COLOR_8);
    QPixmap pix = Processing::converFormat(maf.mat);
    package.image = Processing::resizePix(pix,Variable::ALBUM_IMAGE_SIZE);
    package.info = maf.info;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit albumFinish(var);
}
