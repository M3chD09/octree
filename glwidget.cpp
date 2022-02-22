#include "glwidget.h"

#include <cmath>

GLWidget::GLWidget()
{
    timerUpdate = new QTimer(this);
    timerCal = new QTimer(this);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete program;
    delete vshader;
    delete fshader;
    delete timerUpdate;
    delete timerCal;
    vbo.destroy();
    vao.destroy();
    doneCurrent();
}

void GLWidget::updateOctree()
{
    if (cutter.nowTime == cutter.maxTime) {
        timerUpdate->stop();
        timerCal->stop();
        return;
    }

    for (size_t i = 0; i < octree.getOctantCount(octree.depth); i++) {
        if (octree.data[i].color != OctreeBlack) {
            continue;
        }

        if ((octree.getOctantRect(i) & cutter.getRect(cutter.nowTime)).isEmpty()) {
            continue;
        }

        if (!octree.isMaxLeaf(i)) {
            octree.subdivide(i);
        } else if (cutter.isInside(cutter.nowTime, octree.getOctantLocation(i))) {
            octree.data[i] = OctreeNode { OctreeWhite };
        }
    }

    cutter.nowTime++;
}

void GLWidget::calOctree()
{
    octree.calculateVoxels();
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shader.geom");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag");
    program->link();

    locModel = program->uniformLocation("model");
    locView = program->uniformLocation("view");
    locProjection = program->uniformLocation("projection");

    program->bind();
    program->setUniformValue("objectColor", objectColor);
    program->setUniformValue("lightColor", lightColor);
    program->setUniformValue("lightPos", lightPos);

    octree.calculateVoxels();
    unsigned int leafCount = (unsigned int)octree.voxelLocations.size();

    if (!vao.isCreated()) {
        vao.create();
    }
    vao.bind();
    if (!vbo.isCreated()) {
        vbo.create();
        vbo.bind();
        vbo.allocate(leafCount * 4 * sizeof(float));
    }

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, leafCount * 3 * sizeof(float), 1);
    vao.release();
}

void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    matModel.setToIdentity();
    matModel.rotate(camera.rotation);
    matModel.rotate(-90.0f, QVector3D(1.0f, 0, 0));
    matModel.rotate(-90.0f, QVector3D(0, 0, 1.0f));
    program->setUniformValue(locModel, matModel);

    matView.setToIdentity();
    matView.lookAt(camera.pos, camera.pos + camera.front, camera.up);
    program->setUniformValue(locView, matView);

    matProjection.setToIdentity();
    matProjection.perspective(camera.fov, qreal(width()) / qreal(height()), 0.1f, 100.f);
    program->setUniformValue(locProjection, matProjection);

    unsigned int leafCount = (unsigned int)octree.voxelLocations.size();
    vao.bind();
    vbo.allocate(leafCount * 4 * sizeof(float));
    vbo.write(0, octree.voxelLocations.data(), leafCount * 3 * sizeof(float));
    vbo.write(leafCount * 3 * sizeof(float), octree.voxelSizes.data(), leafCount * sizeof(float));
    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, leafCount * 3 * sizeof(float), 1);
    glDrawArrays(GL_POINTS, 0, leafCount);
    vao.release();

    program->disableAttributeArray(0);
}

void GLWidget::resizeGL(int w, int h)
{
    matProjection.setToIdentity();
    matProjection.perspective(camera.fov, qreal(w) / qreal(h), 0.1f, 100.f);
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    camera.keyPressEvent(event);

    if (event->key() == Qt::Key_Z) {
        connect(timerUpdate, &QTimer::timeout, this, QOverload<>::of(&GLWidget::updateOctree));
        timerUpdate->start(10);
        connect(timerCal, &QTimer::timeout, this, QOverload<>::of(&GLWidget::calOctree));
        timerCal->start(10);
    }
    if (event->key() == Qt::Key_P) {
        timerUpdate->stop();
        timerCal->stop();
    }
    if (event->key() == Qt::Key_R) {
        timerUpdate->stop();
        timerCal->stop();
        cutter.nowTime = 0;
        octree.reset();
        octree.calculateVoxels();
    }

    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    camera.mouseMoveEvent(event);
    update();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    camera.wheelEvent(event);
    update();
}
