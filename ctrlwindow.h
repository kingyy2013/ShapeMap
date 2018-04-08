#ifndef CTRLWINDOW_H
#define CTRLWINDOW_H

#include <QWidget>
#include <QDir>
#include <QListWidget>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>

#include <QMediaPlayer>
#include <QMediaRecorder>



#include "videohandler.h"
#include "qtglrenderres.h"
#include "qsliderspinbox.h"
#include "VideoSurface.h"
#include "prewindow.h"

class qtglRenderRes;
class CtrlWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlWindow(QWidget *parent = 0);
    QPushButton *mButtonSetFolder;
    QPushButton *mButtonSetBG;
    QPushButton *mButtonSetLense;
    QPushButton *mButtonSetEnv;
//    QPushButton *mButtonSetDIDark;
    QPushButton *mButtonSetDI;
    QComboBox   *mBoxCUrDI;
    QPushButton *mButtonAddDI;

    QPushButton *mButtonSetFG;


    QPushButton *mButtonRefFolder;
    QPushButton *mButtonLoadProj;
    QPushButton *mButtonSaveProj;
    QPushButton *mButtonSave;
    QPushButton *mReloadShader;
    QPushButton *mButtonAbout;


    QSliderSpinBox *mDepthSliderSpinbox;
    QLabel *mDepthValue;
    QSliderSpinBox *mAlphaSliderSpinbox;
    QLabel *mAlphaValue;
    QSliderSpinBox *mFilterSliderSpinbox;
    QLabel *mFilterValue;
    QSliderSpinBox *mAmbSliderSpinbox;
    QLabel *mAmbValue;
    QSliderSpinBox *mLODSliderSpinbox;
    QLabel *mLODValue;
    QSliderSpinBox *mSMQualitySpinbox;
    QLabel *mSMQualityValue;
    QSliderSpinBox *mCartoonShaSpinbox;
    QLabel *mCartoonShaValue;

    QCheckBox *mShowLight;
    QCheckBox *mShowCos;
    QCheckBox *mShowAmb;
    QCheckBox *mShowSha;
    QCheckBox *mShowPoint;
    QCheckBox *mShowMirror;


signals:
    void SetBG(QPixmap);
    void SetLense(QPixmap);
    void SetEnv(QPixmap);
    void SetDIDark(QPixmap);
    void SetDIBright(QPixmap);
    void SetFG(QPixmap);

public slots:
    void SetFolder();
    void BuildFileList();
    void GetBG();
    void GetLense();
    void GetGradLense();
    void GetDI();
//    void GetDIDark();
//    void GetDIBright();
    void GetFG();
    void GetEnv();

    void LoadProject();
    void SaveProject();

    void PlayAllVideo();
    void DeleteAllVideo();
    void PauseAllVideo();
    void StopAllVideo();

    void SetDisplayImage(int);
    void SaveImage();
    void About();

    void SetPreWindow(int);

private:
    VideoSurface *mVideoSurface;
    QMediaPlayer* mMediaPlayer;
    QMediaRecorder* mMediaRecorder;

    VideoHandler *mVideoHandler;

    QDir *mFileDir;
    QStringList *mFileNames;
    QListWidget  *mFileWidget;
    QPixmap GetPixmap(int index,  QFileInfo &image);
    void CopyPasteFile(QString NewDir, QFileInfo FileName);
    QWidget* CreateRenderControlTab();
    QWidget* CreateVideoControlTab();


    PreWindow *mMiniMap;
    qtglRenderRes *mRenderWindow;
    QPixmap applySobelFiltering(QPixmap pixmap);

    bool m_ShaderSaveCheck;
    bool m_ImageSaveCheck;

    QFileInfo m_BGFileName;
    QFileInfo m_SMFileName;
    QFileInfo m_DIFileNames[10];
    QFileInfo m_ENVFileName;
    QFileInfo m_SIFileName;


    VideoHandler *mVideoBG;
    VideoHandler *mVideoSM;
    VideoHandler *mVideoDIDark;
    VideoHandler *mVideoDIBright;
    VideoHandler *mVideoEnv;


    QFileInfo m_FShaderName;
    QFileInfo m_VShaderName;
protected:
    void closeEvent( QCloseEvent * event );
};

#endif // CTRLWINDOW_H
