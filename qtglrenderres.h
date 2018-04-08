#ifndef QTGLRENDER_H
#define QTGLRENDER_H

#include <QList>
#include <QVector3D>
#include <QGLWidget>
#include <QGLShader>
#include <QGLFunctions>
#include <QVideoFrame>

#define NUM_LAYERS 15

class qtglRenderRes : public QGLWidget,  protected QGLFunctions
{
    Q_OBJECT

public:
    qtglRenderRes(QWidget * = 0);
    ~qtglRenderRes();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    QGLShaderProgram *ShaderProgram;


public slots:
    /// @brief Camera Control.Set amount of rotation around X axis
    /// @param Value The amount to be rotated
    void setXRotation(float value);
    /// @brief Camera Control.Set amount of rotation around Y axis
    /// @param Value The amount to be rotated
    void setYRotation(float value);
    /// @brief Camera Control.Set amount of rotation around Z axis
    /// @param Value The amount to be rotated
    void setZRotation(float value);
    /// @brief Camera Control.Set amount of traslation along X axis
    /// @param Value The amount to be moved
    void setXTranslation(float value);
    /// @brief Camera Control.Set amount of traslation along Y axis
    /// @param Value The amount to be moved
    void setYTranslation(float value);
    /// @brief Camera Control.Set amount of traslation along Z axis
    /// @param Value The amount to be moved
    void setZTranslation(float value);
    void SetTexture(QPixmap, int num,bool isMipmap = true, bool isReversed = true, bool SetSize = 0);
    void SetNormalMap(QPixmap);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *);

    void SetDepthValue(double dep);
    void SetAlphaValue(double alp);
    void SetAmbValue(double amb);
    void SetFilterValue(double filter);
    void SetLevelOfDetail(double LOD);
    void SetSMQuality(double Quality);
    void SetCartoonSha(double Strength);



    void ReloadShader();
    void ToggleInfor(bool info){m_ShowInfo = info;updateGL();}
    void ToggleCos(bool info);
    void ToggleAmb(bool info);
    void ToggleSha(bool info);
    void ToggleMirror(bool info);
    void TogglePoint(bool info);


    void SetMousePos(double x, double y, double z);

    void SetBGTexture(QPixmap, int option = 3);
    void SetLenseTexture(QPixmap, int option = 3);
    void SetEnvTexture(QPixmap img, int option = 3);
//    void SetFGTexture(QPixmap, int option = 3);
    void SetDITexture(QPixmap, int num, int option = 3);
    void AutoMove();


signals:
    /// @brief Qt signals fires when Rotation around X axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot, which makes it useless
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void xRotationChanged(float value);
    /// @brief Qt signals fires when Rotation around Y axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot, which makes it useless
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void yRotationChanged(float value);
    /// @brief Qt signals fires when Rotation around Z axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot, which makes it useless
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void zRotationChanged(float value);
    /// @brief Qt signals fires when Translation along X axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot, which makes it useless
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void xTranslationChanged(float value);
    /// @brief Qt signals fires when Translation along Y axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot, which makes it useless
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void yTranslationChanged(float value);
    /// @brief Qt signals fires when Translation along Z axis has been changed
    /// @details This signal is used for slider control, since slider is disabled for current
    /// use, this signal is not connected to any slot.
    /// This routine is only kept for further use
    /// @param Value Changed amount of rotation
    void zTranslationChanged(float value);
    /// @brief Qt signal fires when the mouse position has been changed
    /// @details Since is the a 1D Spline, only the x value is passed to dispaly a plain in qtglBSplineClassifier
    /// @param pos The new position of pmouse
    void MousePosChanged(float pos);


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


//    void mouseReleaseEvent(QMouseEvent *);
private:
    /// @brief This function will load OpenGL modelview/project/viewport matrix
    /// @details This function is called when m_CameraChanged is true.
    void LoadGLParameters();

    struct CanvasRect{
    float x1;
    float y1;
    float x2;
    float y2;
    float width(){return x2-x1;}
    float height(){return y2-y1;}
    }m_CanvasRectangle;
    /// @brief The amout of roatation around X axis
    float xRot;
    /// @brief The amout of roatation around Y axis
    float yRot;
    /// @brief The amout of roatation around Z axis
    float zRot;
    /// @brief The amout of translation along X axis
    float xTrans;
    /// @brief The amout of translation along Y axis
    float yTrans;
    /// @brief The amout of translation along Z axis
    float zTrans;

    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;


    GLfloat m_lightPosition[4];

    /// @brief OpenGL viewport container
    GLint m_GLviewport[4];
    /// @brief OpenGL ModelView matrix container
    GLdouble m_GLmodelview[16];
    /// @brief OpenGL Projection matrix container
    GLdouble m_GLprojection[16];
    /// @brief Whether current camer position has been changed
    /// @details This bool variable will trigger whether the OpenGL Viewport/ModelView/Projection matrix
    /// needs to be reloaded.
    bool m_CameraChanged;
    /// @brief The Origin(0,0,0) screen depth position under current camera coordinates
    double m_ZeroX,m_ZeroY,m_ZeroZ;
    GLuint m_textures[NUM_LAYERS];
    QPixmap m_TextureMap;
    QPixmap m_NormalMap;

    QGLShader *VertexShader, *FragmentShader;


    void SetShaderParame();
    void InitializeShaderParam();
    void LoadShader(const QString& vshader,const QString& fshader);
//    bool m_ShowInfo;
    //Shadow parameters

    QString m_VShaderFile;
    QString m_FShaderFile;
    float m_dist;
    float m_alpha;
    float m_filter_size;
    float m_amb_strength;
    float m_LOD;
    float m_width;
    float m_height;
    float m_SM_QUality;
    float m_CartoonSha;
    bool m_ShowInfo;
    char m_toggle_ShaAmbCos;
    bool m_toggle_Mirror;
    bool m_toggle_Point;
    QTimer *m_AutoTimer;
public:
    void CalculateNormal();
    /// @brief The contrainer for the current 3D position of mouse
    QVector3D mMousePos;
};

#endif // QTGLRENDER_H
