#include "prewindow.h"

PreWindow::PreWindow()
{

    mVideoShow = new VideoHandler(true, this);
    mVideoShow->hide();
//    mVideoShow->mMediaPlayer.setMuted(true);
    connect(mVideoShow, SIGNAL(NewFrame(QPixmap,int)), this, SLOT(setPixmap(QPixmap)));
}

void PreWindow::SetVideo(QFileInfo fileName)
{
    mVideoShow->show();
    mVideoShow->SetVideo(fileName);
    mFlipped = !(mVideoShow->isGif);

}


//protected member functions
void PreWindow::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    if(pixmap())
    {
        if(mFlipped)
        {
            painter.scale(1, -1);
            painter.translate(0, -this->height());
        }
        QPixmap pixmap_scaled = pixmap()->scaled(size(), Qt::IgnoreAspectRatio);
        painter.drawPixmap(0, 0, pixmap_scaled);
    }
//    painter.drawText(QRect(0,0,width(),height()),Qt::AlignCenter,text());
//    painter.setBackgroundMode(Qt::OpaqueMode);
//    painter.setRenderHint(QPainter::Antialiasing, true);

//    QList<QVector2D>::iterator it;

//    painter.setBrush(QColor(255,0,0));
//    for (it = mMousePos->begin() ; it != mMousePos->end(); it++ )
//        painter.drawEllipse(QRectF((*it).x()*width(), (*it).y()*height(), 5, 5));
}

void PreWindow::SetImage(QPixmap img)
{
    mFlipped = false;
    mVideoShow->Stop();
    mVideoShow->hide();
    setPixmap(img);
}
