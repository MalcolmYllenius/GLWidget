#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtMath>
#include "objmodel.h"

class GLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget* parent = 0);
    ~GLWidget();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

protected:
    OBJModel model;

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;

    QOpenGLShaderProgram program;

    GLuint positionAttribute;
    GLuint colorUniform;
    GLuint matrixUniform;

    QPoint previousMousePosition;

    QTimer timer;

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer colorBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;

    bool isMouseDown;
    float cameraRadius = 5.0f;
    float cameraAngle = 0.0f;
    float cameraTilt = 0.0f;

    const float PI = 3.141593f;
};

#endif //GLWIDGET_H
