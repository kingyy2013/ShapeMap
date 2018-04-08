#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QtCore>
#include <QPixmap>


class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QObject *parent = 0);
    bool present(const QVideoFrame &frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type=QAbstractVideoBuffer::NoHandle ) const;
    bool start(const QVideoSurfaceFormat &format);
//    void stop();
signals:
    void SetVideoFramePix(QPixmap);
    void SetVideoFrame(const QVideoFrame&);

private:
      QVideoFrame m_frame;
      QImage::Format m_imageFormat;
      QVideoSurfaceFormat m_videoFormat;

};

#endif // VIDEOSURFACE_H
