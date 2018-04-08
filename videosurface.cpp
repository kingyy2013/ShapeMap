#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent) :
    QAbstractVideoSurface(parent)
{
    m_imageFormat = QImage::Format_Invalid;
}

bool VideoSurface::present(const QVideoFrame &frame){
    m_frame = frame;
//    qDebug()<<surfaceFormat();
    if(surfaceFormat().pixelFormat() != m_frame.pixelFormat() ||
            surfaceFormat().frameSize() != m_frame.size()) {
//        stop();
//        qDebug() << "surface: return false";
        return false;
    } else {
        if (m_frame.map(QAbstractVideoBuffer::ReadOnly)) {
            QImage image(
                        m_frame.bits(),
                        m_frame.width(),
                        m_frame.height(),
                        m_frame.bytesPerLine(),
                        m_imageFormat);
            emit SetVideoFramePix(QPixmap::fromImage(image));
        }
        return true;
    }
}

bool VideoSurface::start(const QVideoSurfaceFormat &format){
//    qDebug() << "surface: start";
    m_videoFormat = format;
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        m_imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);
//        qDebug() << "surface: return true";
        return true;
    } else {
//        qDebug() << "surface: return false";
        return false;
    }
}


QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

