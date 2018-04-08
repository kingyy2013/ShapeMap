#include "videohandler.h"

VideoHandler::VideoHandler(bool isPreview, QWidget *parent) :
    QWidget(parent)
{
    //set all buttons/sliders
    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));


    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);


    //set layouts
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    FrameNumber = NULL;
    if(!isPreview)
    {
        //create checkbox for Mipmap and Mute;
        FrameNumber = new QLabel("-1");
        controlLayout->addWidget(FrameNumber);
        CheckMipmap = new QCheckBox();
        CheckMute = new QCheckBox();
        controlLayout->addWidget(new QLabel("MipMap"));
        controlLayout->addWidget(CheckMipmap);
        controlLayout->addWidget(new QLabel("Mute"));
        controlLayout->addWidget(CheckMute);
        SetMute();
        connect(CheckMipmap,SIGNAL(clicked(bool)),this, SLOT(SetMipmap(bool)));
        connect(CheckMute,SIGNAL(clicked(bool)),this, SLOT(SetMute(bool)));

        SetMipmap(false);
    }


//    QBoxLayout *layout = new QVBoxLayout;
//    layout->addLayout(controlLayout);
    controlLayout->addWidget(errorLabel);
    setLayout(controlLayout);

    SetEnable(false);
    connect(&mMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged()));
    connect(&mMediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mMediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&mMediaPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));
    connect(&mVideoSurface, SIGNAL(SetVideoFramePix(QPixmap)),this,SLOT(SurfaceFrame(QPixmap)));


    connect(&mMoviePlayer, SIGNAL(stateChanged(QMovie::MovieState)),
            this, SLOT(mediaStateChanged()));
    connect(&mMoviePlayer, SIGNAL(frameChanged(int)), this, SLOT(positionChanged(int)));
}


VideoHandler::~VideoHandler()
{
}


void VideoHandler::SurfaceFrame(QPixmap img)
{
    emit NewFrame(img, ((int)isGif)<<1|((int)isMipmap));
}


void VideoHandler::SetVideo(QFileInfo fileName)
{
    if(fileName.suffix()==QString("gif"))
    {
        mMoviePlayer.setFileName(fileName.absoluteFilePath());
        if(mMoviePlayer.frameCount()>=0)
        {
            positionSlider->setMaximum(mMoviePlayer.frameCount() - 1);
        }
        else
        {
            positionSlider->setMaximum(0);
        }

        if(mMediaPlayer.isAvailable())
            mMediaPlayer.stop();
        if(mMoviePlayer.isValid())
            mMediaPlayer.stop();
        if(mMoviePlayer.isValid())
        {
            mMoviePlayer.setPaused(true);
            isGif = true;
            mMediaPlayer.blockSignals(true);
            mMoviePlayer.blockSignals(false);
            mMoviePlayer.start();
            SetEnable(true);
        }
    }
    else
    {
        if(mMediaPlayer.isAvailable())
            mMediaPlayer.stop();
        if(mMoviePlayer.isValid())
            mMediaPlayer.stop();
        mMediaPlayer.setMedia(QUrl::fromLocalFile(fileName.absoluteFilePath()));
        if(mMediaPlayer.isAvailable())
        {
            isGif = false;
            mMoviePlayer.blockSignals(true);
            mMediaPlayer.blockSignals(false);
            SetEnable(true);

            mMediaPlayer.play();
            mMediaPlayer.setVideoOutput(&mVideoSurface);
        }
    }

}

void VideoHandler::SetEnable(bool value)
{
    playButton->setEnabled(value);
    positionSlider->setEnabled(value);
}

void VideoHandler::play()
{
    if(isGif)
    {
        switch(mMoviePlayer.state())  {
        case QMovie::Running:
            mMoviePlayer.setPaused(true);
            break;
        default:
            mMoviePlayer.setPaused(false);
            break;
        }
    }
    else
    {
        switch(mMediaPlayer.state()) {
        case QMediaPlayer::PlayingState:
            mMediaPlayer.pause();
            break;
        default:
            mMediaPlayer.play();
            break;
        }
    }
}

void VideoHandler::mediaStateChanged()
{
    if(isGif)
    {
        switch(mMoviePlayer.state()) {
        case QMovie::Running:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
    else
    {
        switch(mMediaPlayer.state()) {
        case QMediaPlayer::PlayingState:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

//this is the positionchanged slider for gif
void VideoHandler::positionChanged(int position)
{
//    positionChanged((qint64)position);


    bool hasFrames = (mMoviePlayer.currentFrameNumber() >= 0);
    if (hasFrames) {
        if (mMoviePlayer.frameCount() > 0) {
            positionSlider->setMaximum(mMoviePlayer.frameCount() - 1);
        } else {
            if (mMoviePlayer.currentFrameNumber() > positionSlider->maximum())
                positionSlider->setMaximum(mMoviePlayer.currentFrameNumber());
        }
        positionSlider->setValue(mMoviePlayer.currentFrameNumber());
    } else {
        positionSlider->setMaximum(0);
    }
    if(FrameNumber)
        FrameNumber->setText(QString("%1").arg(position));
    emit NewFrame(mMoviePlayer.currentPixmap(), ((int)isGif)<<1|((int)isMipmap));
}

void VideoHandler::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
    if(FrameNumber)
        FrameNumber->setText(QString("%1").arg(position));
}

void VideoHandler::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void VideoHandler::setPosition(int position)
{
    if(isGif)
    {
        mMoviePlayer.jumpToFrame(position);
    }
    else
        mMediaPlayer.setPosition(position);
    if(FrameNumber)
        FrameNumber->setText(QString("%1").arg(position));
}


void VideoHandler::handleError()
{
    playButton->setEnabled(false);
    errorLabel->setText("Error: " + mMediaPlayer.errorString());
}


void VideoHandler::Play()
{
    if(isGif)
    {
        if(mMoviePlayer.isValid())
            mMoviePlayer.start();
    }
    else
        if(mMediaPlayer.isAvailable())
            mMediaPlayer.play();
}



void VideoHandler::Stop()
{
    if(isGif)
    {
        if(mMoviePlayer.isValid())
        {
            mMoviePlayer.stop();
        }
    }
    else
        if(mMediaPlayer.isAvailable())
        {
            mMediaPlayer.stop();
        }
    positionSlider->setValue(0);
    if(FrameNumber)
        FrameNumber->setText(QString("%1").arg(0));
}




void VideoHandler::Pause()
{
    if(isGif)
    {
        if(mMoviePlayer.isValid())
            mMoviePlayer.setPaused(true);
    }
    else
        if(mMediaPlayer.isAvailable())
            mMediaPlayer.pause();
}

