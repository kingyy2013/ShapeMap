#include "qtglrenderres.h"

#include <QtGui>
#include <QtOpenGL>
#include <gl/GLU.h>
#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

qtglRenderRes::qtglRenderRes(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //default value for rotation and translation parameters
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xTrans = 0;
    yTrans = 0;
    //different Z value under different set-up pf m_GLprojection matrix
#ifdef m_GLprojection_MODEL
    zTrans = 0;
#else
    zTrans = 400;
#endif
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    resize(400,400);

    m_CanvasRectangle.x1 =-1;
    m_CanvasRectangle.y1 =-1;
    m_CanvasRectangle.x2 =1;
    m_CanvasRectangle.y2 =1;

    //clear all the shaders
    ShaderProgram = NULL;
    VertexShader = FragmentShader = NULL;
    glGenTextures(NUM_LAYERS, m_textures);

    //do not show any light information
    m_ShowInfo = false;
    m_toggle_ShaAmbCos = 7;
    m_toggle_Mirror = false;


    m_AutoTimer = new QTimer(this);
    connect(m_AutoTimer, SIGNAL(timeout()), this, SLOT(AutoMove()));
    m_AutoTimer->blockSignals(true);
    m_AutoTimer->start(100);
}

qtglRenderRes::~qtglRenderRes()
{
}

QSize qtglRenderRes::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize qtglRenderRes::sizeHint() const
{
    return QSize(300, 300);
}

static void qNormalizeAngle(float &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//following three functions are the rotation for XYZ direction
void qtglRenderRes::setXRotation(float value)
{
    qNormalizeAngle(value);
    if (value != xRot) {
        xRot = value;
        emit xRotationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}

void qtglRenderRes::setYRotation(float value)
{
    qNormalizeAngle(value);
    if (value != yRot) {
        yRot = value;
        emit yRotationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}

void qtglRenderRes::setZRotation(float value)
{
    qNormalizeAngle(value);
    if (value != zRot) {
        zRot = value;
        emit zRotationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}

//following three functions are the translation along XYZ
//m_CameraChanged will be set to true to enable the update loading of m_GLmodelview/m_GLprojection/m_GLviewport parameters
void qtglRenderRes::setXTranslation(float value)
{
    if (value != xTrans) {
        xTrans = value;
        emit xTranslationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}


void qtglRenderRes::setYTranslation(float value)
{
    if (value != yTrans) {
        yTrans = value;
        emit yTranslationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}


void qtglRenderRes::setZTranslation(float value)
{
    if (value != zTrans&&value>0) {
        zTrans = value;
        emit zTranslationChanged(value);
        m_CameraChanged = true;
        updateGL();
    }
}
void qtglRenderRes::initializeGL()
{
    initializeGLFunctions();
    //Make load camera initliazed
    m_CameraChanged = true;
//    m_ShowInfo = false;
    //initialization of OpenGL
    qglClearColor(QColor(0.0,0.0,0.0));
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);


    glPointSize(10.0);
    LoadGLParameters();
    glShadeModel(GL_SMOOTH);
    //    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Set The Texture Generation Mode For S To Sphere Mapping (NEW)
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Set The Texture Generation Mode For T To Sphere Mapping (NEW)


//    m_lightPosition[0] = mMousePos.x();
//    m_lightPosition[1] = mMousePos.y();
//    m_lightPosition[2] = mMousePos.z();
//    m_lightPosition[3] = 4.0;
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
    //    glActiveTexture(GL_TEXTURE0);
    //    SetTexture(QPixmap(".//../Images/Deck.png"), 0);
    //    glActiveTexture(GL_TEXTURE1);
    //    SetTexture(QPixmap(".//../Images/BedRoom.png"), 1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures[0]);

    m_VShaderFile = QString("./GLSL/Basic.vsh");
    m_FShaderFile = QString("./GLSL/Basic.fsh");
//    m_VShaderFile = QString(".//../ShapeMap_Q521/Basic.vsh");
//    m_FShaderFile = QString(".//../ShapeMap_Q521/Basic.fsh");

//    m_VShaderFile = QString(":Basic.vsh");
//    m_FShaderFile = QString(":Basic.fsh");
    LoadShader(m_VShaderFile, m_FShaderFile);

    InitializeShaderParam();
    SetShaderParame();


}

void qtglRenderRes::ReloadShader()
{
    LoadShader(m_VShaderFile, m_FShaderFile);
    SetShaderParame();
    updateGL();

}

void qtglRenderRes::InitializeShaderParam()
{
    m_dist = 0.0;
    m_alpha = 1.0;
    m_filter_size = 1;
    m_amb_strength = 1.0;
    m_width = 1.0;
    m_height = 1.0;
    m_SM_QUality = 0.5;
    m_CartoonSha = 0.1;
    m_toggle_ShaAmbCos = 7;
    m_toggle_Mirror = false;
    m_toggle_Point = true;
    mMousePos = QVector3D(0,0,1.0);

}

void qtglRenderRes::SetShaderParame()
{

    ShaderProgram->setUniformValue("tex_BG", 0);
    ShaderProgram->setUniformValue("tex_SM", 1);
    ShaderProgram->setUniformValue("tex_Env", 2);

    ShaderProgram->setUniformValue("tex_DI0", 3);
    ShaderProgram->setUniformValue("tex_DI1", 4);
    ShaderProgram->setUniformValue("tex_DI2", 5);
    ShaderProgram->setUniformValue("tex_DI3", 6);
    ShaderProgram->setUniformValue("tex_DI4", 7);
    ShaderProgram->setUniformValue("tex_DI5", 8);
    ShaderProgram->setUniformValue("tex_DI6", 9);
    ShaderProgram->setUniformValue("tex_DI7", 10);
    ShaderProgram->setUniformValue("tex_DI8", 11);
    ShaderProgram->setUniformValue("tex_DI9", 12);


    ShaderProgram->setUniformValue("is_Video", 0);
    ShaderProgram->setUniformValue("dist", (float)m_dist);
    ShaderProgram->setUniformValue("alpha", (float)m_alpha);
    ShaderProgram->setUniformValue("filter_size", (float)m_filter_size);
    ShaderProgram->setUniformValue("amb_strength", (float)m_amb_strength);
    ShaderProgram->setUniformValue("width", (float)m_width);
    ShaderProgram->setUniformValue("height", (float)m_height);
    ShaderProgram->setUniformValue("LOD", m_LOD);
    ShaderProgram->setUniformValue("toggle_ShaAmbCos", (int)m_toggle_ShaAmbCos);
    ShaderProgram->setUniformValue("toggle_Mirror", m_toggle_Mirror);
    ShaderProgram->setUniformValue("toggle_Point", m_toggle_Point);
    ShaderProgram->setUniformValue("SM_Quality", m_SM_QUality);
    ShaderProgram->setUniformValue("Cartoon_sha", m_CartoonSha);
    ShaderProgram->setUniformValue("light_dir", -mMousePos);
}


void qtglRenderRes::paintGL()
{
//    glGetIntegerv(GL_VIEWPORT, m_GLviewport);           // Retrieves The Viewport Values (X, Y, Width, Height)
//    qDebug()<<m_GLviewport[0]<<m_GLviewport[1]<<m_GLviewport[2]<<m_GLviewport[3];

//    //I have to always rest the viewport, not know why
//    int side = qMin(width(), height());
//    glViewport((width() - side) / 2, (height() - side) / 2, side, side);

    //set up a font
    QFont m_font;
    m_font.setPointSize(15);
    //Load new ModelView/Projection/Viewport parameter if there is any change of camera
//        if(m_CameraChanged)
//            LoadGLParameters();


    //basic OpenGL rutine, clear the screen, and load the identity for m_GLmodelview
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //differen prjection mode has different change of Z translation value
    //An orthognal m_GLprojection has a scaling of Z value
    //A perspective m_GLprojection has just the Z translation
#ifdef m_GLprojection_MODE
    glTranslatef(xTrans, yTrans, zTrans-10.0);
#else
    glScalef(400/(zTrans),400/(zTrans),400/(zTrans));
    glTranslatef(xTrans/100, yTrans/100, 0.0);
#endif
    //rotation
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    //    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);

    //    glDisable(GL_LIGHTING);
    //    //draw the XYZ axis, arrow, and a marker for equals 1
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    glLineWidth(2);
    //    glBegin(GL_LINES);
    //    glColor4f(1.0, 0.0, 0.0, 1.0);
    //    glVertex3f(-1,0,0);
    //    glVertex3f(-1,0.05,0);
    //    glVertex3f(-2,0,0);
    //    glVertex3f(2,0,0);
    //    glVertex3f(1,0,0);
    //    glVertex3f(1,0.05,0);
    //    glVertex3f(2,0,0);
    //    glVertex3f(1.9,0.05,0);
    //    glVertex3f(2,0,0);
    //    glVertex3f(1.9,-0.05,0);

    //    glColor4f(0.0, 1.0, 0.0, 1.0);
    //    glVertex3f(0,-1,0);
    //    glVertex3f(0.05,-1,0);
    //    glVertex3f(0,-2,0);
    //    glVertex3f(0,2,0);
    //    glVertex3f(0,1,0);
    //    glVertex3f(0.05,1,0);
    //    glVertex3f(0,2,0);
    //    glVertex3f(0.05,1.9,0);
    //    glVertex3f(0,2,0);
    //    glVertex3f(-0.05,1.9,0);

    //    glColor4f(0.0, 0.0, 1.0, 1.0);
    //    glVertex3f(0,0,-1);
    //    glVertex3f(0,0.05,-1);
    //    glVertex3f(0,0,-2);
    //    glVertex3f(0,0,2);
    //    glVertex3f(0,0,1);
    //    glVertex3f(0,0.05,1);
    //    glVertex3f(0,0,2);
    //    glVertex3f(0,0.05,1.9);
    //    glVertex3f(0,0,2);
    //    glVertex3f(0,-0.05,1.9);
    //    glEnd();



    //    glEnable(GL_LIGHTING);

    //the rectangle
    //    glActiveTexture(GL_TEXTURE1);
    //    glActiveTexture(GL_TEXTURE0);
    //    glDisable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    //    glBindTexture(GL_TEXTURE_2D, m_textures[0]);

    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    //    glActiveTexture(GL_TEXTURE1);
    //    ShaderProgram->setUniformValue("tex_coord", GL_TEXTURE1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures[1]);


    //    qDebug()<<"tex_BG"<<tex_BG;
    //    int tex_SM = ShaderProgram->uniformLocation("tex_SM");
    //    qDebug()<<"tex_SM"<<tex_SM;
    //    int tex_33 = ShaderProgram->uniformLocation("tex_ss");
    //    qDebug()<<"tex_33"<<tex_33;
    //    ShaderProgram->setUniformValue(tex_BG, GL_TEXTURE0);
    //    ShaderProgram->bind();

    //    glBindTexture(GL_TEXTURE_2D, m_textures[0]);

    //    glEnable(GL_TEXTURE_2D);
    //    GLint myLoc = glGetUniformLocation(ShaderProgram->, "tex_BG");
    //    glProgramUniform1i(ShaderProgram, myLoc, 0);
    //        glActiveTexture(GL_TEXTURE1);
    //    ShaderProgram->setUniformValue("tex_SM", GL_TEXTURE1);
    //        glBindTexture(GL_TEXTURE_2D, m_textures[1]);

    //    glEnable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glTexCoord2d(0.0,0.0);
    glVertex3f(m_CanvasRectangle.x1,m_CanvasRectangle.y1,0);
    glNormal3f(0.0,0.0,1.0);
    glTexCoord2d(0.0,1.0);
    glVertex3f(m_CanvasRectangle.x1,m_CanvasRectangle.y2,0);
    glNormal3f(0.0,0.0,1.0);
    glTexCoord2d(1.0,1.0);
    glVertex3f(m_CanvasRectangle.x2,m_CanvasRectangle.y2,0);
    glNormal3f(0.0,0.0,1.0);
    glTexCoord2d(1.0,0.0);
    glVertex3f(m_CanvasRectangle.x2,m_CanvasRectangle.y1,0);
    glNormal3f(0.0,0.0,1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

//    glBegin(GL_QUADS);
//    glColor4f(1.0, 1.0, 1.0, 0.0);
//    glTexCoord2d(0.0,0.0);
//    glVertex3f(m_CanvasRectangle.x1,m_CanvasRectangle.y1,0);
//    glNormal3f(0.0,0.0,1.0);
//    glTexCoord2d(0.0,1.0);
//    glVertex3f(m_CanvasRectangle.x1,m_CanvasRectangle.y2,0);
//    glNormal3f(0.0,0.0,1.0);
//    glTexCoord2d(1.0,1.0);
//    glVertex3f(m_CanvasRectangle.x2,m_CanvasRectangle.y2,0);
//    glNormal3f(0.0,0.0,1.0);
//    glTexCoord2d(1.0,0.0);
//    glVertex3f(m_CanvasRectangle.x2,m_CanvasRectangle.y1,0);
//    glNormal3f(0.0,0.0,1.0);
//    glEnd();
//    glDisable(GL_TEXTURE_2D);
    //    ShaderProgram->removeAllShaders();


    //    qDebug()<<"Paint"<<m_CameraChanged;
    //    //load camera parameters

    if(m_ShowInfo)
    {
        ShaderProgram->release();

        glColor4f(0.0, 1.0, 1.0, 1.0);
        glLineWidth(0.5);
        glBegin(GL_LINE_LOOP);
        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        glVertex3f(0.0,mMousePos.y(),mMousePos.z());
        glVertex3f(0.0,0.0,mMousePos.z());
        glVertex3f(mMousePos.x(),0.0,mMousePos.z());
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(mMousePos.x(),mMousePos.y(),0.0);
        glVertex3f(0.0,mMousePos.y(),0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(mMousePos.x(),0.0,0.0);
        glEnd();

        //        glBegin(GL_LINE_LOOP);
        //        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        //        glVertex3f(mMousePos.x(),0.0,mMousePos.z());
        //        glVertex3f(mMousePos.x(),0.0,0.0);
        //        glVertex3f(mMousePos.x(),mMousePos.y(),0.0);
        //        glEnd();
        //        glBegin(GL_LINE_LOOP);
        //        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        //        glVertex3f(0.0,mMousePos.y(),mMousePos.z());
        //        glVertex3f(0.0,mMousePos.y(),0.0);
        //        glVertex3f(mMousePos.x(),mMousePos.y(),0.0);
        //        glEnd();

        glColor4f(1.0, 0.0, 1.0, 1.0);
        glLineWidth(0.5);
        glBegin(GL_LINES);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,mMousePos.z());
        glVertex3f(mMousePos.x(),0.0,0.0);
        glVertex3f(mMousePos.x(),0.0,mMousePos.z());
        glVertex3f(0.0,mMousePos.y(),0.0);
        glVertex3f(0.0,mMousePos.y(),mMousePos.z());
        glVertex3f(mMousePos.x(),mMousePos.y(),0.0);
        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        glEnd();

        //draw mouse position
        glColor4f(253.0/255, 184.0/255, 19.0/255, 1.0);
        glBegin(GL_POINTS);
        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        glEnd();
        glLineWidth(1);

        glBegin(GL_LINES);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(mMousePos.x(),mMousePos.y(),mMousePos.z());
        glEnd();
        //write text at this position
        //    glActiveTexture(GL_TEXTURE0);
        QString xy_pos = QString("pos: (%1,%2)").arg(mMousePos.x(),0,'f',3).arg(mMousePos.y(),0,'f',3);
        renderText(mMousePos.x(), mMousePos.y()+0.01, mMousePos.z(),xy_pos, m_font);

//        qDebug()<<xy_pos;
//        glColor4f(184.0/255,253.0/255,19.0/255, 1.0);
//        QString z_temp;
//        z_temp.fill(' ', xy_pos.size()+8);
        QString z_pos = QString("pow: %1").arg(mMousePos.z(),0,'f',3);
//        qDebug()<<z_pos;

        renderText(mMousePos.x(), mMousePos.y()-0.04, mMousePos.z(), z_pos, m_font);

        //display mouse position
        ShaderProgram->bind();
    }

    if(m_CameraChanged)
        LoadGLParameters();
}

void qtglRenderRes::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    setWindowTitle(QString("Render Window %1 x %2").arg(width).arg(height));

//    glViewport(10,100,100,100);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0,1.0,-1.0,1.0,-10.0,1000000.0);
    glMatrixMode(GL_MODELVIEW);
    m_CameraChanged = true;
}

void qtglRenderRes::mousePressEvent(QMouseEvent *event)
{
    //the 2D position of mouse click
    lastPos = event->pos();
    //Load new ModelView/Projection/Viewport parameter if there is any change of camera
    if(m_CameraChanged)
        LoadGLParameters();
    if(!(event->modifiers() & Qt::ControlModifier))
    {
        if(event->buttons() & Qt::LeftButton )
        {
            GLdouble winX, winY;               // Holds Our X, Y and Z Coordinates

            winX = event->x();                  // Holds The Mouse X Coordinate
            winY = event->y();                  // Holds The Mouse Y Coordinate
            winY = (float)m_GLviewport[3] - winY;           // Subtract The Current Mouse Y Coordinate From The Screen Height.

            GLdouble posX, posY, posZ;
//            qDebug()<<mMousePos;
            //Find the position of click in 3D, with reference to the plane defined by origin and look_at direction
            gluUnProject( winX, winY, m_ZeroZ, m_GLmodelview, m_GLprojection, m_GLviewport, &posX, &posY, &posZ);//The new position of the mouse
            mMousePos.setX(posX);
            mMousePos.setY(posY);
            ShaderProgram->setUniformValue("light_dir", -mMousePos);
        }
    }
    updateGL();
}

void qtglRenderRes::mouseMoveEvent(QMouseEvent *event)
{
    //The change of the mouse
    float dx = event->x() - lastPos.x();
    float dy = event->y() - lastPos.y();
    //If Alt has been pressed, perform a rotation or translation
    if(!(event->modifiers() & Qt::ControlModifier))
    {
        if(event->buttons() & Qt::RightButton)
        {
//            qDebug()<<zTrans+dx-dy;
            mMousePos.setZ(mMousePos.z()+ float(dx-dy)/500);
            ShaderProgram->setUniformValue("light_dir", -mMousePos);
        }
        else if(event->buttons() & Qt::LeftButton)
        {
            m_AutoTimer->blockSignals(true);
            GLdouble winX, winY;               // Holds Our X, Y and Z Coordinates

            winX = event->x();                  // Holds The Mouse X Coordinate
            winY = event->y();                  // Holds The Mouse Y Coordinate
            winY = (float)m_GLviewport[3] - winY;           // Subtract The Current Mouse Y Coordinate From The Screen Height.

            GLdouble posX, posY, posZ;
//            qDebug()<<mMousePos;
            //Find the position of click in 3D, with reference to the plane defined by origin and look_at direction
            gluUnProject( winX, winY, m_ZeroZ, m_GLmodelview, m_GLprojection, m_GLviewport, &posX, &posY, &posZ);//The new position of the mouse
            mMousePos.setX(posX);
            mMousePos.setY(posY);
            ShaderProgram->setUniformValue("light_dir", -mMousePos);
        }
    }
    else
    {
        //If no Alt pressed
        if(event->buttons() & Qt::RightButton)
        {
//            qDebug()<<zTrans+dx-dy;
            setZTranslation(zTrans + dx-dy);
        }
        else if (event->buttons() & Qt::LeftButton) {

            setXTranslation(xTrans + (float)dx*0.1);
            setYTranslation(yTrans -  (float)dy*0.1);
        }
        else if (event->buttons() & Qt::MiddleButton) {
            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);
        }
    }
    emit MousePosChanged(mMousePos.x());
    lastPos = event->pos();
    updateGL();
}

void qtglRenderRes::AutoMove()
{
    double radius = QVector2D(mMousePos.x(),mMousePos.y()).length();
    double theta=atan2(mMousePos.x(), mMousePos.y())+0.05;
    mMousePos.setX(sin(theta)*radius);
    mMousePos.setY(cos(theta)*radius);
    ShaderProgram->setUniformValue("light_dir", -mMousePos);
    updateGL();
}


void qtglRenderRes::LoadGLParameters()
{
    //Load openGL ModelView/Project/Viewpot parameters
    glGetIntegerv(GL_VIEWPORT, m_GLviewport);           // Retrieves The Viewport Values (X, Y, Width, Height)
    glGetDoublev(GL_MODELVIEW_MATRIX, m_GLmodelview);       // Retrieve The Modelview Matrix
    glGetDoublev(GL_PROJECTION_MATRIX, m_GLprojection);     // Retrieve The Projection Matrix

    //Not know why, but it solves the problem, maybe some issue with QT
    if(width()<height())
        m_GLviewport[1] = -m_GLviewport[1];

    //After this function is called, set the CameraChanged to be false to prevent recall of this function
    m_CameraChanged = false;
    //calculate the depth value of the origin under current camera setting
    gluProject(mMousePos.x(),mMousePos.y(),mMousePos.z(),m_GLmodelview,m_GLprojection,m_GLviewport,&m_ZeroX,&m_ZeroY,&m_ZeroZ);
}

void qtglRenderRes::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        //toggle show info for all the points of just the clicked one
    case Qt::Key_Escape: //key to exit
        close();
        break;
    case Qt::Key_Tab:
        if(m_AutoTimer->signalsBlocked())
            m_AutoTimer->blockSignals(false);
        else
            m_AutoTimer->blockSignals(true);
        break;
    case Qt::Key_Space:
        m_CameraChanged = true;

        if(xRot == 0&&
                   yRot == 0&&
                   zRot ==0&&
                   xTrans == 0&&
                   yTrans == 0)
        {
            //different Z value under different set-up pf m_GLprojection matrix
            #ifdef m_GLprojection_MODEL
                    zTrans = 0;
            #else
                    zTrans = 400;
            #endif
        }
        //default value for rotation and translation parameters
        xRot = 0;
        yRot = 0;
        zRot = 0;
        xTrans = 0;
        yTrans = 0;
        resize(m_width*(float)917/m_height,917);

        break;
//    case Qt::Key_Control:
//        m_ShowInfo = true;
    default:
        QGLWidget::keyPressEvent(e);
    }
    //Update the Drawing
    updateGL();
}

void qtglRenderRes::keyReleaseEvent(QKeyEvent *e)
{

    switch(e->key())
    {
//    case Qt::Key_Control:
//        m_ShowInfo = false;
    default:
        QGLWidget::keyPressEvent(e);
    }
    updateGL();
}

void qtglRenderRes::SetTexture(QPixmap TextureMap, int num,bool isMipmap, bool isReversed, bool SetSize)
{
    // allocate a texture name
    m_TextureMap = TextureMap.copy();

    QGLContext::BindOptions options = QGLContext::NoBindOption;
    GLint bind_type = GL_RGB;
    if(isMipmap)
        options = options|QGLContext::MipmapBindOption|QGLContext::LinearFilteringBindOption;
    //if the image has to be reversed, it means this is an image, in order not to make any changes
    //to the GLSL part of checking alpha channle, I set this bind type to be GL_RGB
    if(isReversed)
    {
        bind_type = GL_RGBA;
        options = options|QGLContext::InvertedYBindOption;
    }

    m_textures[num] = bindTexture(m_TextureMap, GL_TEXTURE_2D, bind_type, options);

    //    // when texture area is small, bilinear filter the closest mipmap
    //    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    //    // when texture area is large, bilinear filter the original
    //    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //    // the texture wraps over at the edges (repeat)
    //    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    if(SetSize)
    {
        m_width = TextureMap.width();
        m_height = TextureMap.height();
        ShaderProgram->setUniformValue("width", (float)TextureMap.width());
        ShaderProgram->setUniformValue("height", (float)TextureMap.height());
        resize(m_width*(float)height()/m_height,height());

    }
    //    updateGL();
}

void qtglRenderRes::SetNormalMap(QPixmap TextureMap)
{}

void qtglRenderRes::SetDepthValue(double dep)
{
    m_dist = dep;
    ShaderProgram->setUniformValue("dist", m_dist);
    show();
    updateGL();
}

void qtglRenderRes::SetAlphaValue(double alp)
{
    m_alpha = alp;
    ShaderProgram->setUniformValue("alpha", m_alpha);
    show();
    updateGL();
}


void qtglRenderRes::SetFilterValue(double filter)
{
    m_filter_size = filter;
    ShaderProgram->setUniformValue("filter_size", m_filter_size);
    show();
    updateGL();
}
void qtglRenderRes::SetAmbValue(double amb)
{
    m_amb_strength = amb;
    //    ShaderProgram->bind();
    ShaderProgram->setUniformValue("amb_strength", m_amb_strength);
    //    qDebug()<<dep;
    //    LoadShader("./GLSL/Basic.vsh", "./GLSL/Basic.fsh");
    show();
    updateGL();
}

void qtglRenderRes::SetLevelOfDetail(double LOD)
{
    m_LOD = LOD;
    ShaderProgram->setUniformValue("LOD", m_LOD);
    show();
    updateGL();

}

void qtglRenderRes::SetSMQuality(double Quality)
{
    m_SM_QUality = Quality;
    ShaderProgram->setUniformValue("SM_Quality", m_SM_QUality);
    show();
    updateGL();

}

void qtglRenderRes::SetCartoonSha(double Strength)
{
    m_CartoonSha = Strength;
//    qDebug()<<m_CartoonSha;
    ShaderProgram->setUniformValue("Cartoon_sha", m_CartoonSha);
    show();
    updateGL();

}


//some problem here with compilation, I am still suspecting it is realted to glactivetexture.
//if this function is defined as input QVector3D, then it crashed.
void qtglRenderRes::SetMousePos(double x, double y, double z)
{
    mMousePos = QVector3D(x,y,z);
//    qDebug()<<mMousePos;
    ShaderProgram->setUniformValue("light_dir", -mMousePos);
    updateGL();
}


void qtglRenderRes::SetBGTexture(QPixmap img, int option)
{
    deleteTexture(m_textures[0]);
    glActiveTexture(GL_TEXTURE0);
//    SetTexture(img, 0, 0, QGLContext::NoBindOption|QGLContext::MipmapBindOption|QGLContext::LinearFilteringBindOption);
    SetTexture(img, 0,  option&1,option&2);

    glDisable(GL_TEXTURE0);
    show();
    updateGL();
}


void qtglRenderRes::SetLenseTexture(QPixmap img, int option)
{
    deleteTexture(m_textures[1]);
    glActiveTexture(GL_TEXTURE1);
    SetTexture(img, 1, option&1,option&2, true);
    show();
//    qDebug()<<"123";
    glDisable(GL_TEXTURE1);

    //if I do not put this active texture here, then the rendtext() of Qt will not work,
    //I think it is related to how active texture works, or some bug with Qt rendertext()
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE0);

//    SetFGTexture(img);
    updateGL();
}

//void qtglRenderRes::SetFGTexture(QPixmap img, int option)
//{
//    deleteTexture(m_textures[3]);
//    glActiveTexture(GL_TEXTURE3);
//    SetTexture(img, 3, option&1,option&2);
//    show();
//    glDisable(GL_TEXTURE3);

//    glActiveTexture(GL_TEXTURE0);
//    glDisable(GL_TEXTURE0);
//    updateGL();
//}

void qtglRenderRes::SetEnvTexture(QPixmap img, int option)
{
    deleteTexture(m_textures[2]);
    glActiveTexture(GL_TEXTURE2);
    SetTexture(img, 2, option&1,option&2);
    show();
    glDisable(GL_TEXTURE2);

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE0);
    updateGL();

}

void qtglRenderRes::SetDITexture(QPixmap img, int num, int option)
{
    deleteTexture(m_textures[3+num]);

    glActiveTexture(GL_TEXTURE3+num);
    SetTexture(img, 3+num, option&1,option&2);
    show();
    glDisable(GL_TEXTURE3+num);

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE0);
    updateGL();
}


void qtglRenderRes::ToggleCos(bool info)
{

    m_toggle_ShaAmbCos &= 0xe;
    if(info)
        m_toggle_ShaAmbCos += 1;
    ShaderProgram->setUniformValue("toggle_ShaAmbCos", (int)m_toggle_ShaAmbCos);
    updateGL();
}

void qtglRenderRes::ToggleAmb(bool info)
{
    m_toggle_ShaAmbCos &= 0xd;
    if(info)
        m_toggle_ShaAmbCos += 2;
    ShaderProgram->setUniformValue("toggle_ShaAmbCos", (int)m_toggle_ShaAmbCos);
    updateGL();

}

void qtglRenderRes::ToggleSha(bool info)
{
    m_toggle_ShaAmbCos &= 0xb;
    if(info)
        m_toggle_ShaAmbCos += 4;
    ShaderProgram->setUniformValue("toggle_ShaAmbCos", (int)m_toggle_ShaAmbCos);
    updateGL();
}

void qtglRenderRes::ToggleMirror(bool info)
{
    m_toggle_Mirror = info;
    ShaderProgram->setUniformValue("toggle_Mirror", m_toggle_Mirror);
    updateGL();
}

void qtglRenderRes::TogglePoint(bool info)
{
    m_toggle_Point = info;
    ShaderProgram->setUniformValue("toggle_Point", m_toggle_Point);
    updateGL();
}


void qtglRenderRes::LoadShader(const QString& vshader,const QString& fshader)
{
    if(ShaderProgram)
    {
        ShaderProgram->release();
        ShaderProgram->removeAllShaders();
    }
    else ShaderProgram = new QGLShaderProgram;

    if(VertexShader)
    {
        delete VertexShader;
        VertexShader = NULL;
    }

    if(FragmentShader)
    {
        delete FragmentShader;
        FragmentShader = NULL;
    }

    // load and compile vertex shader
    QFileInfo vsh(vshader);
    if(vsh.exists())
    {
        VertexShader = new QGLShader(QGLShader::Vertex);
        if(VertexShader->compileSourceFile(vshader))
            ShaderProgram->addShader(VertexShader);
        else
            QMessageBox::critical(this, "GLSL Vertex Shader Error",
                                   QString("GLSL ")+VertexShader->log());
    }
    else qWarning() << "Vertex Shader source file " << vshader << " not found.";


    // load and compile fragment shader
    QFileInfo fsh(fshader);
    if(fsh.exists())
    {
        FragmentShader = new QGLShader(QGLShader::Fragment);
        if(FragmentShader->compileSourceFile(fshader))
            ShaderProgram->addShader(FragmentShader);
        else
            QMessageBox::critical(this, "GLSL Fragment Shader Error",
                                  QString("GLSL ")+FragmentShader->log());
    }
    else qWarning() << "Fragment Shader source file " << fshader << " not found.";

    if(!ShaderProgram->link())
    {
        QMessageBox::critical(this, "GLSL Shader Program Linker Error",
                              QString("GLSL ")+ShaderProgram->log());
    }
    else ShaderProgram->bind();
}



