#include "ctrlwindow.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
//#include <QSpinBox>

CtrlWindow::CtrlWindow(QWidget *parent) :
    QWidget(parent)
{

    mRenderWindow = new qtglRenderRes();

    setWindowTitle("Shape Map Control Panel");
    //set slider
    mFileWidget = new QListWidget();
    connect(mFileWidget, SIGNAL(currentRowChanged(int)), this, SLOT(SetDisplayImage(int)));

    //set mini map labe
     mMiniMap = new PreWindow();


    QGridLayout *mainLayout = new QGridLayout();
    //set all buttons
    //image related
    mButtonSetBG = new QPushButton("Set BG Image");
    connect(mButtonSetBG, SIGNAL(clicked()), this, SLOT(GetBG()));
    mButtonSetLense = new QPushButton("Set Shape Map");
    connect(mButtonSetLense, SIGNAL(clicked()), this, SLOT(GetLense()));
    mButtonSetEnv = new QPushButton("Set Enviroment Map");
    connect(mButtonSetEnv, SIGNAL(clicked()), this, SLOT(GetEnv()));

    mButtonSetDI = new QPushButton("Set Diffusion");
    connect(mButtonSetDI, SIGNAL(clicked()), this, SLOT(GetDI()));
    mBoxCUrDI = new QComboBox();
    mBoxCUrDI->addItems(QString("0|1|2|3|4|5|6|7|8|9").split("|", QString::SkipEmptyParts));
    connect(mBoxCUrDI,SIGNAL(currentIndexChanged(int)), this, SLOT(SetPreWindow(int)));

//    mButtonSetDIBright = new QPushButton("Set Diffusion(Bright)");
//    connect(mButtonSetDIBright, SIGNAL(clicked()), this, SLOT(GetDIBright()));

    mButtonSetFG = new QPushButton("Set Specular Image");
    connect(mButtonSetFG, SIGNAL(clicked()), this, SLOT(GetFG()));

    mButtonSetFolder = new QPushButton("Set Image Folder");
    connect(mButtonSetFolder, SIGNAL(clicked()), this, SLOT(SetFolder()));

    //project related
    mButtonRefFolder = new QPushButton("   Refresh Folder   ");
    connect(mButtonRefFolder, SIGNAL(clicked()), this, SLOT(BuildFileList()));
    mButtonLoadProj = new QPushButton("Load Project");
    connect(mButtonLoadProj, SIGNAL(clicked()), this, SLOT(LoadProject()));
    mButtonSaveProj = new QPushButton("Save Project");
    connect(mButtonSaveProj, SIGNAL(clicked()), this, SLOT(SaveProject()));
    mButtonSave = new QPushButton("Save Image");
    connect(mButtonSave, SIGNAL(clicked()), this, SLOT(SaveImage()));
    mReloadShader = new QPushButton("Reload Shader");
    connect(mReloadShader, SIGNAL(clicked()), mRenderWindow, SLOT(ReloadShader()));
    mButtonAbout = new QPushButton("About");
    connect(mButtonAbout, SIGNAL(clicked()), this, SLOT(About()));

    //set slider
    mDepthSliderSpinbox = new QSliderSpinBox();
    mDepthSliderSpinbox->SetRatio(5000);
    mDepthSliderSpinbox->SetChangeRange(-4999, 4999);
    mDepthSliderSpinbox->SetChangeStep(0.01);

    mDepthValue = new QLabel("Refraction:");

    mAlphaSliderSpinbox = new QSliderSpinBox();
    mAlphaSliderSpinbox->SetRatio(10000);
    mAlphaSliderSpinbox->SetChangeRange(0, 10000);
    mAlphaSliderSpinbox->SetChangeStep(0.01);
    mAlphaValue = new QLabel("Alpha");

    mFilterSliderSpinbox = new QSliderSpinBox();
    mFilterSliderSpinbox->SetRatio(1250);
    mFilterSliderSpinbox->SetChangeRange(0, 10000);
    mFilterSliderSpinbox->SetChangeStep(0.01);
    mFilterValue = new QLabel("Filter Size");


    mAmbSliderSpinbox = new QSliderSpinBox();
    mAmbSliderSpinbox->SetRatio(10000);
    mAmbSliderSpinbox->SetChangeRange(-10000, 10000);
    mAmbSliderSpinbox->SetChangeStep(0.01);
    mAmbValue = new QLabel("Shadow Size");


    mLODSliderSpinbox = new QSliderSpinBox();
    mLODSliderSpinbox->SetRatio(1000);
    mLODSliderSpinbox->SetChangeRange(0, 10000);
    mLODSliderSpinbox->SetChangeStep(0.01);
    mLODValue = new QLabel("Translucency");

    mSMQualitySpinbox = new QSliderSpinBox();
    mSMQualitySpinbox->SetRatio(1000);
    mSMQualitySpinbox->SetChangeRange(0, 1000);
    mSMQualitySpinbox->SetChangeStep(0.01);
    mSMQualityValue = new QLabel("SM Quality");

    mCartoonShaSpinbox = new QSliderSpinBox();
    mCartoonShaSpinbox->SetRatio(1000);
    mCartoonShaSpinbox->SetChangeRange(1, 1000);
    mCartoonShaSpinbox->SetChangeStep(0.01);
    mCartoonShaValue = new QLabel("Cartoon");



    QTabWidget* tabWidget = new QTabWidget;
    tabWidget->addTab(CreateRenderControlTab(), tr("Render"));
    tabWidget->addTab(CreateVideoControlTab(), tr("Video"));
    tabWidget->setTabPosition(QTabWidget::East);


    ///attach to widget
    mainLayout->addWidget(mFileWidget, 0,0,6,1);


    mainLayout->addWidget(mButtonSetBG,1,1,1,2);
    mainLayout->addWidget(mButtonSetLense,2,1,1,2);
    mainLayout->addWidget(mButtonSetEnv,3,1,1,2);
//    mainLayout->addWidget(mButtonSetDIDark,3,1,1,2);
    mainLayout->addWidget(mButtonSetDI,4,1,1,2);
    mainLayout->addWidget(mBoxCUrDI,5,1,1,1);

//    mainLayout->addWidget(mButtonSetFG,6,1,1,2);


    mainLayout->addWidget(mButtonSetFolder,0,1,1,5);

    mainLayout->addWidget(mButtonRefFolder,1,3,1,3);
    mainLayout->addWidget(mButtonLoadProj,2,3,1,3);
    mainLayout->addWidget(mButtonSaveProj,3,3,1,3);

    mainLayout->addWidget(mButtonSave,4,3,1,3);


//    mainLayout->addWidget(mButtonAbout,5,3,1,3);
    mainLayout->addWidget(mReloadShader,5,3,1,3);
    mainLayout->addWidget(tabWidget, 9,0,5,6);

    //set main layout
    setLayout(mainLayout);

    mRenderWindow->show();
//    connect(this,SIGNAL(SetBG(QPixmap)), mRenderWindow, SLOT(SetBGTexture(QPixmap)));
//    connect(this,SIGNAL(SetLense(QPixmap)), mRenderWindow, SLOT(SetLenseTexture(QPixmap)));
//    connect(this,SIGNAL(SetEnv(QPixmap)), mRenderWindow, SLOT(SetEnvTexture(QPixmap)));
//    connect(this,SIGNAL(SetDIDark(QPixmap)), mRenderWindow, SLOT(SetDIDarkTexture(QPixmap)));
//    connect(this,SIGNAL(SetDIBright(QPixmap)), mRenderWindow, SLOT(SetDIBrightTexture(QPixmap)));
//    connect(this,SIGNAL(SetFG(QPixmap)), mRenderWindow, SLOT(SetFGTexture(QPixmap)));
//    connect(this,SIGNAL(SetFG(QPixmap)), mRenderWindow, SLOT(SetFGTexture(QPixmap)));


    //set initial value of slider controllers
    mAlphaSliderSpinbox->setValue(1);
    mDepthSliderSpinbox->setValue(0);
    mFilterSliderSpinbox->setValue(2);
    mAmbSliderSpinbox->setValue(0.25);
    mLODSliderSpinbox->setValue(0);
    mSMQualitySpinbox->setValue(0.5);
    mCartoonShaSpinbox->setValue(0.1);

    mFileDir = NULL;

    //video surface is used to hadle video frames
//    mVideoSurface = new VideoSurface(NULL);
//    mMediaPlayer = new QMediaPlayer();
//    mMediaPlayer->setVideoOutput(mVideoSurface);

//    mMediaPlayer->setMuted(true)

    //adjust positions
    move(100,50);
    adjustSize();
    mRenderWindow->setWindowTitle("Rendered Scene");
    mRenderWindow->move(pos().x()+size().width()+15,pos().y());
    mMiniMap->setWindowTitle("Image Preview");
    mMiniMap->show();
    mMiniMap->move(pos().x(),pos().y()+size().height()+40);
    mMiniMap->setPixmap(QPixmap(size().width(),sizeHint().width()));
    mMiniMap->adjustSize();
    mRenderWindow->sizeHint();
    mRenderWindow->resize(mMiniMap->height()+size().height()+40,mMiniMap->height()+size().height()+40);
    QSizePolicy pol;
    pol.setHeightForWidth(true);
    mRenderWindow->setSizePolicy(pol);
    mMiniMap->setScaledContents(true);
}

void CtrlWindow::SetFolder()
{
    if(mFileDir!=NULL)
        delete mFileDir;
    mFileDir = new QDir(QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks));
    BuildFileList();
}

QWidget* CtrlWindow::CreateRenderControlTab()
{
    QWidget *RenderTab = new QWidget();
    int SliderStart_pos = 0;
    QGridLayout *RenderLayout = new QGridLayout();

    RenderLayout->addWidget(mDepthSliderSpinbox->m_Slider,SliderStart_pos,0,1,1);
    RenderLayout->addWidget(mDepthSliderSpinbox,SliderStart_pos,2,1,1);
    RenderLayout->addWidget(mDepthValue,SliderStart_pos,1,1,1);
    connect(mDepthSliderSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetDepthValue(double)));

    RenderLayout->addWidget(mAlphaSliderSpinbox->m_Slider,SliderStart_pos+1,0,1,1);
    RenderLayout->addWidget(mAlphaSliderSpinbox,SliderStart_pos+1,2,1,1);
    RenderLayout->addWidget(mAlphaValue,SliderStart_pos+1,1,1,1);
    connect(mAlphaSliderSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetAlphaValue(double)));

    RenderLayout->addWidget(mFilterSliderSpinbox->m_Slider,SliderStart_pos+2,0,1,1);
    RenderLayout->addWidget(mFilterSliderSpinbox,SliderStart_pos+2,2,1,1);
    RenderLayout->addWidget(mFilterValue,SliderStart_pos+2,1,1,1);
    connect(mFilterSliderSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetFilterValue(double)));

    RenderLayout->addWidget(mAmbSliderSpinbox->m_Slider,SliderStart_pos+3,0,1,1);
    RenderLayout->addWidget(mAmbSliderSpinbox,SliderStart_pos+3,2,1,1);
    RenderLayout->addWidget(mAmbValue,SliderStart_pos+3,1,1,1);
    connect(mAmbSliderSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetAmbValue(double)));

    RenderLayout->addWidget(mLODSliderSpinbox->m_Slider,SliderStart_pos+5,0,1,1);
    RenderLayout->addWidget(mLODSliderSpinbox,SliderStart_pos+5,2,1,1);
    RenderLayout->addWidget(mLODValue,SliderStart_pos+5,1,1,1);
    connect(mLODSliderSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetLevelOfDetail(double)));

    RenderLayout->addWidget(mSMQualitySpinbox->m_Slider,SliderStart_pos+6,0,1,1);
    RenderLayout->addWidget(mSMQualitySpinbox,SliderStart_pos+6,2,1,1);
    RenderLayout->addWidget(mSMQualityValue,SliderStart_pos+6,1,1,1);
    connect(mSMQualitySpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetSMQuality(double)));

    RenderLayout->addWidget(mCartoonShaSpinbox->m_Slider,SliderStart_pos+4,0,1,1);
    RenderLayout->addWidget(mCartoonShaSpinbox,SliderStart_pos+4,2,1,1);
    RenderLayout->addWidget(mCartoonShaValue,SliderStart_pos+4,1,1,1);
    connect(mCartoonShaSpinbox, SIGNAL(SliderSpinboxValueChange(double)), mRenderWindow, SLOT(SetCartoonSha(double)));


    //add check box
    mShowLight = new QCheckBox();
    RenderLayout->addWidget(mShowLight,SliderStart_pos,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Light"),SliderStart_pos,4,1,2);
    connect(mShowLight, SIGNAL(toggled(bool)), mRenderWindow, SLOT(ToggleInfor(bool)));

    mShowCos = new QCheckBox();
    mShowCos->setChecked(true);
    RenderLayout->addWidget(mShowCos,SliderStart_pos+1,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Cos"),SliderStart_pos+1,4,1,2);
    connect(mShowCos, SIGNAL(toggled(bool)), mRenderWindow, SLOT(ToggleCos(bool)));

    mShowAmb = new QCheckBox();
    mShowAmb->setChecked(true);
    RenderLayout->addWidget(mShowAmb,SliderStart_pos+2,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Amb"),SliderStart_pos+2,4,1,2);
    connect(mShowAmb, SIGNAL(toggled(bool)), mRenderWindow, SLOT(ToggleAmb(bool)));

    mShowSha = new QCheckBox();
    mShowSha->setChecked(true);
    RenderLayout->addWidget(mShowSha,SliderStart_pos+3,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Sha"),SliderStart_pos+3,4,1,2);
    connect(mShowSha, SIGNAL(toggled(bool)), mRenderWindow, SLOT(ToggleSha(bool)));

    mShowPoint = new QCheckBox();
    mShowPoint->setChecked(true);
    RenderLayout->addWidget(mShowPoint,SliderStart_pos+4,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Point"),SliderStart_pos+4,4,1,2);
    connect(mShowPoint, SIGNAL(toggled(bool)), mRenderWindow, SLOT(TogglePoint(bool)));


    mShowMirror = new QCheckBox();
    mShowMirror->setChecked(false);
    RenderLayout->addWidget(mShowMirror,SliderStart_pos+5,3,1,1, Qt::AlignRight);
    RenderLayout->addWidget(new QLabel("Toggle Mirror"),SliderStart_pos+5,4,1,2);
    connect(mShowMirror, SIGNAL(toggled(bool)), mRenderWindow, SLOT(ToggleMirror(bool)));

    RenderTab->setLayout(RenderLayout);
    return RenderTab;
}

QWidget* CtrlWindow::CreateVideoControlTab()
{
    mVideoBG = new VideoHandler();
    connect(mVideoBG, SIGNAL(NewFrame(QPixmap,int)), mRenderWindow, SLOT(SetBGTexture(QPixmap,int)));

    mVideoSM = new VideoHandler();
    connect(mVideoSM, SIGNAL(NewFrame(QPixmap,int)), mRenderWindow, SLOT(SetLenseTexture(QPixmap,int)));

    mVideoDIDark = new VideoHandler();
    connect(mVideoDIDark, SIGNAL(NewFrame(QPixmap,int)), mRenderWindow, SLOT(SetDIDarkTexture(QPixmap,int)));

    mVideoDIBright = new VideoHandler();
    connect(mVideoDIBright, SIGNAL(NewFrame(QPixmap,int)), mRenderWindow, SLOT(SetDIBrightTexture(QPixmap,int)));

    mVideoEnv = new VideoHandler();
    connect(mVideoEnv, SIGNAL(NewFrame(QPixmap,int)), mRenderWindow, SLOT(SetEnvTexture(QPixmap,int)));

    QWidget *VideoTab = new QWidget();
    QGridLayout *VideoLayout = new QGridLayout();
    VideoLayout->setSpacing(0);
    VideoLayout->setMargin(0);

    VideoLayout->addWidget(mVideoBG,0,1,1,5);
    VideoLayout->addWidget(new QLabel("Background"),0,0,1,1,Qt::AlignCenter);
    VideoLayout->addWidget(mVideoSM,1,1,1,5);
    VideoLayout->addWidget(new QLabel("ShapeMap"),1,0,1,1,Qt::AlignCenter);
    VideoLayout->addWidget(mVideoDIDark,2,1,1,5);
    VideoLayout->addWidget(new QLabel("DI0 Dark"),2,0,1,1,Qt::AlignCenter);
    VideoLayout->addWidget(mVideoDIBright,3,1,1,5);
    VideoLayout->addWidget(new QLabel("DI1 Bright"),3,0,1,1,Qt::AlignCenter);
    VideoLayout->addWidget(mVideoEnv,4,1,1,5);
    VideoLayout->addWidget(new QLabel("Enviroment"),4,0,1,1,Qt::AlignCenter);


    QPushButton *PlayAll = new QPushButton("PlayAll");
    QPushButton *PauseAll = new QPushButton("PauseAll");
    QPushButton *StopAll = new QPushButton("StopAll");

    connect(PlayAll,SIGNAL(clicked()),this,SLOT(PlayAllVideo()));
    connect(PauseAll,SIGNAL(clicked()),this,SLOT(PauseAllVideo()));
    connect(StopAll,SIGNAL(clicked()),this,SLOT(StopAllVideo()));

    VideoLayout->addWidget(PlayAll,5,0,1,2);
    VideoLayout->addWidget(PauseAll,5,2,1,2);
    VideoLayout->addWidget(StopAll,5,4,1,2);


    VideoTab->setLayout(VideoLayout);
    mVideoBG->show();

    return VideoTab;
}

void CtrlWindow::BuildFileList()
{
    if(mFileDir!=NULL)
    {
        mFileWidget->blockSignals(true);
        mFileWidget->clear();
        QStringList nameFilter;
        nameFilter << "*.png" << "*.jpg" << "*.gif"<<"*.bmp"<<"*.jpeg"<<"*.pbm"<<"*.pgm"<<"*.ppm"<<"*.wmv"<<"*.avi"<<"*.mp4"<<"*.mov"<<"*.mpg";
        QStringList list = mFileDir->entryList (nameFilter);
        mFileWidget->addItems(list);
        mFileWidget->blockSignals(false);
    }
    else
    {
        QMessageBox::warning( this, "Empty", "No folder is selected" );
    }
}


void CtrlWindow::GetBG()
{

//    mVideoHandler = new VideoHandler();
//    mVideoHandler->show();
////    QPixmap img = GetPixmap(mFileWidget->currentRow(),m_BGFileName);

////    if(!img.isNull())
////        emit SetBG(img);

//    QString fileName = QString("C:/Users/ywang-x/Videos/GeForce 3D Vision/GeForce 3D Vision Experience.wmv");
////    mMediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
////    mMediaPlayer->play();

//    connect(mVideoHandler, SIGNAL(NewFrame(QPixmap,bool)), mRenderWindow, SLOT(SetBGTexture(QPixmap,bool)));
//    mVideoHandler->SetVideo(fileName);
////    connect(mVideoSurface, SIGNAL(SetVideoFrame(const QVideoFrame &)), mRenderWindow, SLOT(SetBGTexture(const QVideoFrame &)));



    QPixmap img = GetPixmap(mFileWidget->currentRow(),m_BGFileName);

    if(img.isNull()|m_BGFileName.suffix()==QString("gif"))
    {
        mVideoBG->SetVideo(m_BGFileName);
    }
    else
    {
        mVideoBG->Stop();
        mVideoBG->SetEnable(false);
        mRenderWindow->SetBGTexture(img);

    }
}

void CtrlWindow::GetLense()
{
    QPixmap img = GetPixmap(mFileWidget->currentRow(),m_SMFileName);
    if(img.isNull()|m_SMFileName.suffix()==QString("gif"))
    {
        mVideoSM->SetVideo(m_SMFileName);
   }
    else
    {
        mVideoSM->Stop();
        mVideoSM->SetEnable(false);
//        emit SetLense(img);

        mRenderWindow->SetLenseTexture(img);
    }
}

void CtrlWindow::GetGradLense()
{
    QPixmap img =applySobelFiltering( GetPixmap(mFileWidget->currentRow(), m_SMFileName));
    if(!img.isNull())
        emit SetLense(img);
}

void CtrlWindow::GetDI()
{
    int cur_ind = mBoxCUrDI->currentIndex();
    QPixmap img = GetPixmap(mFileWidget->currentRow(), m_DIFileNames[cur_ind]);
    if(img.isNull()|m_DIFileNames[cur_ind].suffix()==QString("gif"))
    {
        mVideoDIDark->SetVideo(m_DIFileNames[cur_ind]);

    }
    else
    {
        mVideoDIDark->Stop();
        mVideoDIDark->SetEnable(false);
//        emit SetDIDark(img);
        mRenderWindow->SetDITexture(img,cur_ind);
    }
}

//void CtrlWindow::GetDIBright()
//{
//    QPixmap img = GetPixmap(mFileWidget->currentRow(), m_DIBrightFileName);
//    if(img.isNull()|m_DIBrightFileName.suffix()==QString("gif"))
//    {
//        mVideoDIBright->SetVideo(m_DIBrightFileName);

//    }
//    else
//    {
//        mVideoDIBright->Stop();
//        mVideoDIBright->SetEnable(false);
////        emit SetDIBright(img);
//        mRenderWindow->SetDITexture(img,1);
//    }
//}

void CtrlWindow::GetEnv()
{
    QPixmap img = GetPixmap(mFileWidget->currentRow(), m_ENVFileName);

    if(img.isNull()|m_ENVFileName.suffix()==QString("gif"))
    {
        mVideoEnv->SetVideo(m_ENVFileName);

    }
    else
    {
        mVideoEnv->Stop();
        mVideoEnv->SetEnable(false);
//        emit SetEnv(img);
        mRenderWindow->SetEnvTexture(img);
    }
}



void CtrlWindow::GetFG()
{
    QPixmap img = GetPixmap(mFileWidget->currentRow(), m_SIFileName);
    if(!img.isNull())
        emit SetFG(img);
}


//void CtrlWindow::GetDepthValue(double dep)
//{
////    mDepthValue->setText(QString("Ref : ") + QString("%1").arg(dep,0,'f',3));
//    mRenderWindow->SetDepthValue((float)dep);
//}

//void CtrlWindow::GetAlphaValue(double alp)
//{
////    mAlphaValue->setText(QString("Alpha : ") + QString("%1").arg(alp,0,'f',3));
//    mRenderWindow->SetAlphaValue((float)alp);
//}

//void CtrlWindow::GetFilterValue(double filter)
//{
////    mFilterValue->setText(QString("Filter : ") + QString("%1").arg(filter,0,'f',3));
//    mRenderWindow->SetFilterValue((float)filter);
//}

//void CtrlWindow::GetAmbValue(double amb)
//{
////    mAmbValue->setText(QString("Amb : ") + QString("%1").arg(amb,0,'f',3));
//    mRenderWindow->SetAmbValue((float)amb);
//}



void CtrlWindow::SaveProject()
{

    //Get the file path
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    "/home/Desktop/myShapeMapProj.smp",
                                                    tr("Files (*.smp *.smf)"));


    //try to open file as read mode
    if(!fileName.isNull())
    {
        if(fileName.endsWith("smp"))
        {
            QFile file(fileName);
            //Try to open the file as write mode
            if(file.open( QIODevice::WriteOnly))
            {

                QTextStream ts(&file);
                ts<<"#Project grecated at "<<QTime::currentTime().toString()<<' '<<QDate::currentDate().toString()<<"\n";
                ts<<"BG="<<m_BGFileName.absoluteFilePath()<<"\n";
                ts<<"SM="<<m_SMFileName.absoluteFilePath()<<"\n";
                ts<<"DI(Dark)="<<m_DIFileNames[0].absoluteFilePath()<<"\n";
                ts<<"DI(Bright)="<<m_DIFileNames[1].absoluteFilePath()<<"\n";
                ts<<"ENV=" <<m_ENVFileName.absoluteFilePath()<<"\n";
                ts<<"Depth="<<mDepthSliderSpinbox->GetDoubleValue()<<"\n";
                ts<<"Alpha="<<mAlphaSliderSpinbox->GetDoubleValue()<<"\n";
                ts<<"FilterSize="<<mFilterSliderSpinbox->GetDoubleValue()<<"\n";
                ts<<"ShadowSize="<<mAmbSliderSpinbox->GetDoubleValue()<<"\n";
                ts<<"SMQuality="<<mSMQualitySpinbox->GetDoubleValue()<<"\n";
                ts<<"Translucency="<<mLODSliderSpinbox->GetDoubleValue()<<"\n";
                ts<<"Cartoon="<<mCartoonShaSpinbox->GetDoubleValue()<<"\n";
//                qDebug()<<mCartoonShaSpinbox->GetDoubleValue();


                ts<<"ShowLight="<<(int)mShowLight->isChecked()<<"\n";
                ts<<"ShowCos="<<(int)mShowCos->isChecked()<<"\n";
                ts<<"ShowAmb="<<(int)mShowAmb->isChecked()<<"\n";
                ts<<"ShowSha="<<(int)mShowSha->isChecked()<<"\n";
                ts<<"ShowPoint="<<(int)mShowPoint->isChecked()<<"\n";
                ts<<"ShowMirror="<<(int)mShowMirror->isChecked()<<"\n";

                ts<<"LightPos=["<<mRenderWindow->mMousePos.x()<<","<<mRenderWindow->mMousePos.y()<<","<<mRenderWindow->mMousePos.z()<<"]\n";
            }
            else
                QMessageBox::critical(this, tr("Save File"),
                                      tr("could not create file"));
            file.close();
        }
        else
            if(fileName.endsWith("smf"))
            {
                QFileInfo fi(fileName);
//                qDebug()<<fi.absolutePath()+fi.baseName();
                QString newDir = fi.absolutePath()+'/'+fi.baseName();
                QDir().mkdir(newDir);
                QFile file(newDir+'/'+fi.baseName()+".smf");
                if(file.open( QIODevice::WriteOnly))
                {
                    QTextStream ts(&file);
                    ts<<"#Project grecated at "<<QTime::currentTime().toString()<<' '<<QDate::currentDate().toString()<<"\n";

                    ts<<"BG="<<m_BGFileName.fileName()<<"\n";
                    CopyPasteFile(newDir, m_BGFileName);

                    ts<<"SM="<<m_SMFileName.fileName()<<"\n";
                    CopyPasteFile(newDir, m_SMFileName);

                    ts<<"DI(Dark)="<<m_DIFileNames[0].fileName()<<"\n";
                    CopyPasteFile(newDir, m_DIFileNames[0]);

                    ts<<"DI(Bright)="<<m_DIFileNames[1].fileName()<<"\n";
                    CopyPasteFile(newDir, m_DIFileNames[1]);

                    ts<<"ENV=" <<m_ENVFileName.fileName()<<"\n";
                    CopyPasteFile(newDir, m_ENVFileName);

                    ts<<"Depth="<<mDepthSliderSpinbox->GetDoubleValue()<<"\n";
                    ts<<"Alpha="<<mAlphaSliderSpinbox->GetDoubleValue()<<"\n";
                    ts<<"FilterSize="<<mFilterSliderSpinbox->GetDoubleValue()<<"\n";
                    ts<<"ShadowSize="<<mAmbSliderSpinbox->GetDoubleValue()<<"\n";
                    ts<<"SMQuality="<<mSMQualitySpinbox->GetDoubleValue()<<"\n";
                    ts<<"Translucency="<<mLODSliderSpinbox->GetDoubleValue()<<"\n";
                    ts<<"Cartoon="<<mCartoonShaSpinbox->GetDoubleValue()<<"\n";
                    ts<<"LightPos=["<<mRenderWindow->mMousePos.x()<<","<<mRenderWindow->mMousePos.y()<<","<<mRenderWindow->mMousePos.z()<<"]\n";

                    ts<<"ShowLight="<<(int)mShowLight->isChecked()<<"\n";
                    ts<<"ShowCos="<<(int)mShowCos->isChecked()<<"\n";
                    ts<<"ShowAmb="<<(int)mShowAmb->isChecked()<<"\n";
                    ts<<"ShowSha="<<(int)mShowSha->isChecked()<<"\n";
                    ts<<"ShowPoint="<<(int)mShowPoint->isChecked()<<"\n";
                    ts<<"ShowMirror="<<(int)mShowMirror->isChecked()<<"\n";
                }
                file.close();
            }
            else
                QMessageBox::critical(this, tr("Save File"),
                                      tr("could not create file"));

    }
    else
        QMessageBox::critical(this, tr("Save File"),
                              tr("file does not exist"));
}

void CtrlWindow::CopyPasteFile(QString NewDir, QFileInfo FileName)
{
    if(FileName.size())
    {
        QFile Temp(FileName.absoluteFilePath());
        if(!Temp.copy(NewDir+'/'+FileName.fileName()))
            QMessageBox::critical(this, FileName.baseName()+" File",
                                  FileName.baseName()+" can not be copied");
    }
}


void CtrlWindow::LoadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/Desktop/myShapeMapProj.smp",
                                                    tr("Files (*.smp *.smf)"));
    if(!fileName.isNull())
    {
        QFile file(fileName);
        QImage temp_img(1,1,QImage::Format_ARGB32);
        temp_img.setPixel(0,0,QColor(0,0,0,0).rgba());
        QPixmap empty_img = QPixmap::fromImage(temp_img);
        //try to open file as read mode
        if(file.open( QIODevice::ReadOnly))
        {
            QString curPath;
            if(fileName.endsWith("smp"))    //file is saved as text file
                curPath = "";
            else
                if(fileName.endsWith("smf"))    //file is saved as text file
                {
                    QFileInfo projFile(fileName);
                    curPath = projFile.absolutePath()+"/";
                    if(mFileDir!=NULL)
                        delete mFileDir;
                    mFileDir = new QDir(projFile.absolutePath());
                    BuildFileList();
                }

            QTextStream strm(&file);
            QString a_line;
            //Jumpe over the '#' lines
            do{
                a_line=strm.readLine();
            }
            while(a_line[0]=='#');
            //Read in the actaul description parts of BSpline
            do
            {
                if(a_line[0]!='#')
                {
                    QStringList temp = a_line.split(QRegExp("(\\=|\\,|\\[|\\])"), QString::SkipEmptyParts);
                    if(temp.at(0)==QString("BG"))
                    {
                        if(temp.size()>=2)
                        {
                            m_BGFileName = QFileInfo(curPath+temp.at(1));
                            QPixmap img = QPixmap(curPath+temp.at(1));
                            if(!img.isNull())
                                mRenderWindow->SetBGTexture(img);
                            else
                                QMessageBox::warning( this, "BG", "Stroed Back Ground Image can not be found" );
                        }
                        else
                        {
                            mRenderWindow->SetBGTexture(empty_img);
                        }
                    }
                    else if(temp.at(0)==QString("SM"))
                    {
                        if(temp.size()>=2)
                        {
                            m_SMFileName = QFileInfo(curPath+temp.at(1));
                            QPixmap img = QPixmap(curPath+temp.at(1));
                            if(!img.isNull())
                                mRenderWindow->SetLenseTexture(img);
                            else
                                QMessageBox::warning( this, "SM", "Stroed Shape Map Image can not be found" );
                        }
                        else
                        {
                            mRenderWindow->SetLenseTexture(empty_img);
                        }
                    }
                    else if(temp.at(0)==QString("DI(Dark)"))
                    {
                        if(temp.size()>=2)
                        {
                            m_DIFileNames[0] = QFileInfo(curPath+temp.at(1));
                            QPixmap img = QPixmap(curPath+temp.at(1));
                            if(!img.isNull())
                                mRenderWindow->SetDITexture(img,0);
                            else
                                QMessageBox::warning( this, "DI(Dark)", "Stroed Diffusion Image(Dark) can not be found" );
                        }
                        else
                        {
                            mRenderWindow->SetDITexture(empty_img,0);
                        }
                    }
                    else if(temp.at(0)==QString("DI(Bright)"))
                    {
                        if(temp.size()>=2)
                        {
                        m_DIFileNames[1] = QFileInfo(curPath+temp.at(1));
                        QPixmap img = QPixmap(curPath+temp.at(1));
                        if(!img.isNull())
                            mRenderWindow->SetDITexture(img,1);
                        else
                            QMessageBox::warning( this, "DI(Bright)", "Stroed Diffusion Image(Bright) can not be found" );
                        }
                        else
                        {
                            mRenderWindow->SetDITexture(empty_img,1);
                        }
                    }
                    else if(temp.at(0)==QString("ENV"))
                    {
                        if(temp.size()>=2)
                        {
                            m_ENVFileName = QFileInfo(curPath+temp.at(1));
                            QPixmap img = QPixmap(curPath+temp.at(1));
                            if(!img.isNull())
                                mRenderWindow->SetEnvTexture(img);

                            else
                                QMessageBox::warning( this, "Env", "Stroed Enviromental Map can not be found" );
                        }
                        else
                        {
                            mRenderWindow->SetEnvTexture(empty_img);
                        }
                    }
                    else if(temp.at(0)==QString("Depth"))
                    {
                        mDepthSliderSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("Alpha"))
                    {
                        mAlphaSliderSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("FilterSize"))
                    {
                        mFilterSliderSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("ShadowSize"))
                    {
                        mAmbSliderSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("Translucency"))
                    {
                        mLODSliderSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("SMQuality"))
                    {
                        mSMQualitySpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("Cartoon"))
                    {
                        mCartoonShaSpinbox->SetSliderValue(temp.at(1).toDouble());
                    }
                    else if(temp.at(0)==QString("LightPos"))
                    {
                        if(temp.size()>=4)
                            mRenderWindow->SetMousePos(temp.at(1).toDouble(), temp.at(2).toDouble(),temp.at(3).toDouble());
                        else
                            QMessageBox::warning( this, "Light", "Light Information must be in [x,y,z] format" );
                    }
                    else if(temp.at(0)==QString("ShowLight"))
                    {
                        mShowLight->setChecked(temp.at(1).toInt());
                    }
                    else if(temp.at(0)==QString("ShowCos"))
                    {
                        mShowCos->setChecked(temp.at(1).toInt());
                    }
                    else if(temp.at(0)==QString("ShowAmb"))
                    {
                        mShowAmb->setChecked(temp.at(1).toInt());
                    }
                    else if(temp.at(0)==QString("ShowSha"))
                    {
                        mShowSha->setChecked(temp.at(1).toInt());
                    }
                    else if(temp.at(0)==QString("ShowPoint"))
                    {
                        mShowPoint->setChecked(temp.at(1).toInt());

                    }
                    else if(temp.at(0)==QString("ShowMirror"))
                    {
                        mShowMirror->setChecked(temp.at(1).toInt());
                    }
                }
                else
                    break;
                a_line=strm.readLine();
                //Stop if encounters a '#' or end of file
            }while(!(a_line.isNull()));
        }
        else
            QMessageBox::critical(this, tr("Save File"),
                                  tr("could not create file"));
    }
    else
        QMessageBox::critical(this, tr("Save File"),
                              tr("file does not exist"));

}

void CtrlWindow::SetDisplayImage(int)
{
    QFileInfo cur_name;
    QPixmap img = GetPixmap(mFileWidget->currentRow(),cur_name);
    if(img.isNull()|cur_name.suffix()==QString("gif"))
    {
        mMiniMap->SetVideo(cur_name);
    }
    else
    {
        mMiniMap->SetImage(img);
        mMiniMap->show();
    }
}

void CtrlWindow::SetPreWindow(int num)
{
    QPixmap img(m_DIFileNames[num].absoluteFilePath());
    if(img.isNull())
    {
        QImage temp_img(1,1,QImage::Format_ARGB32);
        temp_img.setPixel(0,0,QColor(0,0,0,0).rgba());
        QPixmap empty_img = QPixmap::fromImage(temp_img);
        mMiniMap->SetImage(empty_img);
        mMiniMap->show();
    }
    else if(m_DIFileNames[num].suffix()==QString("gif"))
    {
        mMiniMap->SetVideo(m_DIFileNames[num].absoluteFilePath());
    }
    else
    {
        mMiniMap->SetImage(img);
        mMiniMap->show();
    }
}

void CtrlWindow::SaveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "/home/untitled.png",
                               tr("Images (*.png *.jpg *.gif *.bmp *.jpeg *.pbm *.pgm *.ppm)"));
    if(!fileName.isNull())
    {
        QImage image = (mRenderWindow->grabFrameBuffer());
        if( !image.save(fileName))
        {
           QMessageBox::warning( this, "Save Image", "Error saving image." );
        }
    }
}

void CtrlWindow::About()
{
   QMessageBox::about(this, tr("Shape Map"),
               QString("<p><b>Control Penal</b></p>"
                       "<pre><b>Buttons:</b>\n"
                       "<b>Set Image Folder:</b> Set the folder for all images. All images in the set folder will show in the list.\n"
                       "<b>Set BG Image:</b> Refracted image.\n"
                       "<b>Set Shape Map:</b> Aspect ratio will change accordingly, and all other images will be resized to fit current shape map.\n"
                       "<b>Set Diffusion(Dark):</b> The DI0 image in paper.\n"
                       "<b>Set Diffusion(Bright):</b> The DI1 image in paper.\n"
                       "<b>Set Enviroment Map:</b> Reflected images.\n"
                       "<b>Set Image Folder:</b> Set the folder for all images.\n"
                       "<b>Refresh Folder:</b> Refresh current image folder.\n"
                       "<b>Load Project:</b> Load a project file.\n"
                       "<b>Save Project:</b> Save a project file.\n"
                       "A project file is a text file that contains all the current parameters as well as images(names).\n"
                       "A project with extension of '.smp' will only save the absolute path/name of images.\n"
                       "A project with extension of '.smf' will save the actual image files to a folder that has the same name as project name.\n"
                       "Within project file, if an image name is left empty, then a black(rgba = 0,0,0,0) image will be set instead\n"
                       "If the given image name is not readable, a warning will be given, but no image will be set\n"
                       "<b>Save Image:</b> Save current image in the rendering window.</pre>"
                       "<pre><b>Check Boxes</b>\n"
                       "Choose to see whether a certain effect should be turnned.\n"
                       "<b>Toggle Light:</b> Show/off lighting position.\n"
                       "<b>Toggle Cos:</b> Add/remove Shading effect.\n"
                       "<b>Toggle Amb:</b> Add/remove Ambient occlusion.\n"
                       "<b>Toggle Sha:</b> Add/remove Shadow effect.\n"
                       "<b>Toggle Point:</b> Switch bettwen parallel light and point light.</pre>"
                       "<pre><b>Sliders</b>\n"
                       "Refraction:</b> Amount of distortion during refraction.\n"
                       "<b>Alpha:</b> Transparency of current shape map.\n"
                       "<b>Filter Size:</b> Filter Size for calculating ambient occlusion and shadow.\n"
                       "<b>Shadow Size:</b> Threshold to decisde whether a pixel is in shadow or not.\n"
                       "<b>Cartoon:</b> Sharpness between shadow and non-shadow area.\n"
                       "<b>Transluceny:</b> Amount of transluceny for refration.\n"
                       "<b>SM Quality:</b> Flatness of Current shape map, a small value will make current shape map flat in order to eliminate some errors.</pre>"
                       "<p><b>Rendering Window</b></p>"
                       "<pre><b>Left_Button:</b> change lighting postion.\n"
                       "<b>Right_Button:</b> change lighting intensity.\n"
                       "<b>Ctrl+Left_Button:</b> Translate image\n"
                       "<b>Ctrl+Right_Button:</b> Zoomming image\n"
                       "<b>Drag on window</b>: Resize image\n"
                       "<b>Space</b>: Restore to default size\n"
                       "<b>Tab</b>: Turn on/off light auto motion\n"
                       ));
}

QPixmap CtrlWindow::GetPixmap(int index, QFileInfo & img_name)
{
    if(index<0||index>=mFileWidget->count())
    {
        QMessageBox::critical(this, tr("Image Setting"),
                                 tr("Cannot load selected image"));
        return QPixmap();
    }
    img_name = QFileInfo(mFileDir->path()+"/"+mFileWidget->currentItem()->text());
//    qDebug()<<imageFormat;
    QPixmap img(img_name.absoluteFilePath());
    return img;
}

void CtrlWindow::closeEvent( QCloseEvent * event)
{
//    mMediaPlayer->stop();
    StopAllVideo();
    mMiniMap->close();
    mRenderWindow->close();
}

void CtrlWindow::StopAllVideo()
{
    mVideoBG->Stop();
    mVideoSM->Stop();
    mVideoDIDark->Stop();
    mVideoDIBright->Stop();
    mVideoEnv->Stop();
}

void CtrlWindow::PauseAllVideo()
{
    mVideoBG->Pause();
    mVideoSM->Pause();
    mVideoDIDark->Pause();
    mVideoDIBright->Pause();
    mVideoEnv->Pause();

}

void CtrlWindow::PlayAllVideo()
{
    mVideoBG->Play();
    mVideoSM->Play();
    mVideoDIDark->Play();
    mVideoDIBright->Play();
    mVideoEnv->Play();

}

void CtrlWindow::DeleteAllVideo()
{
    delete mVideoBG;
    delete mVideoSM;
    delete mVideoDIDark;
    delete mVideoDIBright;
    delete mVideoEnv;
}

// Apply Sobel filter
QPixmap CtrlWindow::applySobelFiltering(QPixmap pixmap) {

    // Convert the stored pixmap into an image
    QImage image = pixmap.toImage();

    // Declare the various variables utilised within this function
    float **sumX, **sumY, minVal = 10000, maxVal = -10000;
    sumX = new float*[image.width()];
    sumY = new float*[image.width()];
    for(int i=0;i<image.width();i++)
    {
        sumX[i] = new float[image.height()];
        sumY[i] = new float[image.height()];
    }

    // Sobel X and Y masks
    QGenericMatrix<3,3,int> GX, GY;
    GX(0,0) = -1; GX(0,1) = 0; GX(0,2) = 1;
    GX(1,0) = -2; GX(1,1) = 0; GX(1,2) = 2;
    GX(2,0) = -1; GX(2,1) = 0; GX(2,2) = 1;

    GY(0,0) =  1; GY(0,1) = 2; GY(0,2) = 1;
    GY(1,0) =  0; GY(1,1) = 0; GY(1,2) = 0;
    GY(2,0) = -1; GY(2,1) = -2; GY(2,2) = -1;

    // Apply the Sobel filter
    for(int X=0; X < image.width(); X++) {
    for(int Y=0; Y < image.height(); Y++) {

            int tsumX = 0;
            int tsumY = 0;

            if (Y == 0 || Y == image.height()-1)
            {
                sumX[X][Y] = 0;
                sumY[X][Y] = 0;
            }
            else if (X == 0 || X == image.width()-1)
            {
                sumX[X][Y] = 0;
                sumY[X][Y] = 0;
            }
            else {

                for(int I=-1; I<=1; I++)  {

                    for(int J=-1; J<=1; J++)  {

                        int piX = J + X;
                        int piY = I + Y;

                        QRgb currentPixel = image.pixel(QPoint(piX,piY));

                        int R = qRed(currentPixel);
                        int G = qGreen(currentPixel);
                        int B = qBlue(currentPixel);

                        int NC = (R+G+B) / 3;

                        tsumX = tsumX + (NC) * GX(J+1,I+1);

                        tsumY = tsumY + (NC) * GY(J+1,I+1);
                    }
                }
            }
            if(minVal >tsumX )
                minVal = tsumX;
            else
                if(maxVal < tsumX)
                    maxVal = tsumX;
            if(minVal > tsumY)
                minVal = tsumY;
            else
                if(maxVal < tsumY)
                    maxVal = tsumY;

            sumX[X][Y] = tsumX;
            sumY[X][Y] = tsumY;

        }
    }

    if(maxVal == minVal)
    {
        maxVal = 1;
        minVal = 0;
    }
    for(int X=0; X < image.width(); X++) {
        for(int Y=0; Y < image.height(); Y++) {
            image.setPixel(X, Y, qRgb((unsigned char)255*(sumX[X][Y]-minVal)/(maxVal-minVal),(unsigned char)255*(sumY[X][Y]-minVal)/(maxVal-minVal),255));

        }
        delete [] sumX[X];
        delete [] sumY[X];
    }
    delete [] sumX;
    delete [] sumY;

    return QPixmap::fromImage(image);
}


