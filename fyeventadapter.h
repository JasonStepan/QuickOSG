#ifndef FYEVENTADAPTER_H
#define FYEVENTADAPTER_H

#include <QObject>
#include <QQuickItem>

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QSGSimpleTextureNode>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QSGSimpleTextureNode>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickItem>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>


class FYEventAdapter : public QQuickItem
{
    Q_OBJECT
public:
    explicit FYEventAdapter(QQuickItem *parent = nullptr);
    osgViewer::Viewer* getViewer();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QSGNode* updatePaintNode(QSGNode *oldNode,
                             UpdatePaintNodeData *updatePaintNodeData);
private:
    void initOSG();
    void initFBO();
    void updateFBO();

public:
    inline int mouseButton(QMouseEvent *event)
    {
        int button = 0;
        switch (event->button())
        {
            case Qt::LeftButton: button = 1; break;
            case Qt::MidButton: button = 2; break;
            case Qt::RightButton: button = 3; break;
            case Qt::NoButton: button = 0; break;
            default: button = 0; break;
        }
        return button;
    }

public slots:
    void updateViewport();
private slots:
    void onWindowChanged(QQuickWindow *window);
    void frame();

private:
    osg::ref_ptr<osgViewer::Viewer> viewer;
    QOpenGLFramebufferObject *fbo;
    QSGTexture *texture;
    QSGSimpleTextureNode *textureNode;
};

#endif // FYEVENTADAPTER_H
