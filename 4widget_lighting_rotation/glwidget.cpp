
#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
#include "qtlogo.h"


//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    logo = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromRgbF(0,1,0,1);
    qtPurple = QColor::fromRgbF(0,0,0.5,1);
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]


static void AngleRange(int &angle)
{
    while (angle < 0)
        angle += 360 ;
    while (angle > 360 )
        angle -= 360 ;
}

//! [5]
void GLWidget::setXRotation(int angle)
{
    AngleRange(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(int angle)
{
    AngleRange(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    AngleRange(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

//! [6]
void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

    logo = new QtLogo(this, 64);
    logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
   
    static GLfloat lightPosition[4] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
//! [6]

//! [7]
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot , 1.0, 0.0, 0.0);
    glRotatef(yRot , 0.0, 1.0, 0.0);
    glRotatef(zRot , 0.0, 0.0, 1.0);
    logo->draw();
}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 0.5 * dy);
        setYRotation(yRot + 0.5 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 0.5 * dy);
        setZRotation(zRot + 0.5 * dx);
    }
    lastPos = event->pos();
}
//! [10]
