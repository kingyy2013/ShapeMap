
// (c) Copyright Texas A&M University Department of Computer Scinece&Engineering
// ALL RIGHTS RESERVED
//
///
#include "videoplayer.h"
#include <QApplication>
#include <QDesktopWidget>
#include "ctrlwindow.h"
#include <QAbstractVideoSurface>
#include <QVideoWidget>
#include "VideoSurface.h"

//#include <QMediaContent>

class CtrlWindow;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CtrlWindow window;
    window.show();

//    VideoPlayer player;
//    player.resize(320, 240);
//    player.show();


//    QString fileName = QString("C:/Users/ywang-x/Videos/GeForce 3D Vision/GeForce 3D Vision Experience.wmv");
////    qDebug()<<fileName;
////    QMediaPlayer mediaPlayer;
////    mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
////    qDebug()<<mediaPlayer.isAvailable();


//    QMediaPlayer* player = new QMediaPlayer;

//    player->setMedia(QUrl::fromLocalFile(fileName));


//    glVideoSurface *temp = new glVideoSurface(NULL);
////    temp->resize(320, 240);
//    player->setVideoOutput(temp);
//    player->setPosition(5000);
//    player->play();
////    temp->show();
////    videoSurface->
////    QImage temp;
////    QVideoFrame tf = new QVideoFrame(temp);





    app.exec();


}
