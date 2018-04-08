#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <QMediaPlayer>
#include <QtWidgets>
#include <QMovie>
#include "videosurface.h"


class VideoHandler : public QWidget
{
    Q_OBJECT
public:
    explicit VideoHandler(bool isPreview = false, QWidget *parent = 0);
    ~VideoHandler();
    void SetVideo(QFileInfo);
    void SetEnable(bool);
    VideoSurface mVideoSurface;
    QMediaPlayer mMediaPlayer;
    QMovie mMoviePlayer;
    bool isGif;

public slots:
    void play();
    void SetMute(bool value = true){CheckMute->setChecked(isMute = value);mMediaPlayer.setMuted(value);}
    void SetMipmap(bool value = true){CheckMipmap->setChecked(isMipmap = value);}
    void Stop();
    void Play();
    void Pause();
private:
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QLabel *errorLabel;
    QCheckBox *CheckMipmap;
    QCheckBox *CheckMute;
    QLabel *FrameNumber;
    bool isMipmap;
    bool isMute;
signals:
    void NewFrame(QPixmap, int);
private slots:

    void mediaStateChanged();
    void positionChanged(int position);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();
    void SurfaceFrame(QPixmap img);


};

#endif // VIDEOHANDLER_H
