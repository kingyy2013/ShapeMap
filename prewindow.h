#ifndef PREWINDOW_H
#define PREWINDOW_H

#include <QWidget>
#include "videohandler.h"

class PreWindow : public QLabel
{
    Q_OBJECT
public:
    explicit PreWindow();
    VideoHandler *mVideoShow;
    void SetImage(QPixmap img);

signals:
    
public slots:
    void SetVideo(QFileInfo fileName);
protected:
    void paintEvent(QPaintEvent *);
private:
    bool mFlipped;
};

#endif // PREWINDOW_H
