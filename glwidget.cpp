#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    isMouseDown = false;
    timer.setInterval(16);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start();

    QGLFormat format;
    format.setVersion(3, 3);
    format.setProfile(QGLFormat::CoreProfile);
    format.setSampleBuffers(true);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    doneCurrent();
}

void GLWidget::initializeGL()
{
    model.LoadOBJ(":/External/Monkey.obj");
    if (!model.IsLoaded())
    {
        printf("Failure to load OBJ\n");
    }
    model.Centralize();

    initializeOpenGLFunctions();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shader.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shader.frag");
    program.link();
    program.bind();

    modelMatrix.setToIdentity();

    viewMatrix.setToIdentity();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::paintGL()
{
    if (cameraTilt >= 0.5f * PI)
        cameraTilt = 0.5f * PI - 0.01f;
    if (cameraTilt <= -0.5f * PI)
        cameraTilt = -0.5f * PI + 0.01f;

    if(cameraRadius < 1.0f)
        cameraRadius = 1.0f;

    viewMatrix.setToIdentity();
    QVector2D tiltVector(-cos(cameraTilt) * cameraRadius, -sin(cameraTilt) * cameraRadius);
    viewMatrix.lookAt(QVector3D(-cos(cameraAngle) * tiltVector.x(), tiltVector.y(), -sin(cameraAngle) * tiltVector.x()), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.enableAttributeArray("position");
    program.setAttributeArray("position", model.vertices.constData());

    program.enableAttributeArray("normal");
    program.setAttributeArray("normal", model.normals.constData());

    program.setUniformValue("matrix", projectionMatrix * viewMatrix * modelMatrix);
    QMatrix4x4 normalMatrix = modelMatrix;
    normalMatrix.inverted();
    normalMatrix.transposed();
    program.setUniformValue("nMatrix", normalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, model.vertices.count());
}

void GLWidget::resizeGL(int w, int h)
{
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, GLfloat(w) / h, 0.01f, 10000.0f);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;
    }
    previousMousePosition = event->pos();
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = false;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(isMouseDown)
    {
        float dX = previousMousePosition.x() - event->pos().x();
        float dY = previousMousePosition.y() - event->pos().y();
        cameraAngle -= (float)dX * 0.01f;
        cameraTilt += (float)dY * 0.01f;
    }
    previousMousePosition = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    cameraRadius -= (float)event->delta() * 0.001f * cameraRadius;
}
