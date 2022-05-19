﻿#ifndef QUICKOSGEARTHVIEWER_H
#define QUICKOSGEARTHVIEWER_H
 
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <QVector3D>
#include <QList>
#include <osg/OperationThread>
#include <osg/MatrixTransform>
#include <osgGA/EventQueue>
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <QTimer>
#include <osgEarth/MapNode>
 
#define QColor2OSGColor(c) osg::Vec4(c.redF(), c.greenF(), c.blueF(), 1.0f)
#define QVec3ToOSGVec3(v) osg::Vec3(v[0], v[1], v[2])
 
class QuickOSGEarthRenderer;
 
class QuickOSGEarthViewer : public QQuickFramebufferObject
{
    Q_OBJECT
    friend class QuickOSGEarthRenderer;
    Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor)
    Q_PROPERTY(bool hoverEnabled READ hoverEnabled WRITE setHoverEnabled NOTIFY hoverEnabledChanged)
    Q_PROPERTY(osg::Node* scene READ scene WRITE setScene NOTIFY sceneChanged)
    Q_PROPERTY(int framerate READ framerate WRITE setFramerate NOTIFY framerateChanged)
    Q_PROPERTY(osg::Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QOpenGLFramebufferObject* defaultFbo READ defaultFbo NOTIFY defaultFboChanged)
public:
 
    QuickOSGEarthViewer(QQuickItem *parent = nullptr);
    ~QuickOSGEarthViewer() override;
 
    Q_INVOKABLE void home();    // 模型home
    Q_INVOKABLE void lookAt(QVector3D eye, QVector3D center, QVector3D up);
 
    Q_INVOKABLE void addOperation(osg::Operation* operation) { m_operationQueue->add(operation); }
 
    QColor clearColor(){ return m_clearColor; }
    void setClearColor(const QColor& newColor);
 
    bool hoverEnabled() { return m_hoverEnabled; }
    void setHoverEnabled(bool newHoverEnabled);
 
    osg::Node* scene() const;
    void setScene(osg::Node* node);
 
    osg::Node* getNode(QString& name);
 
    int framerate() const { return m_framerate; }
    void setFramerate(int newFramerate);
 
    osg::Camera* camera() const { return m_camera.get(); }
    void setCamera(osg::Camera* newCamera);
 
    void addEventhandle(osgGA::GUIEventHandler* handler);
    QOpenGLFramebufferObject* defaultFbo() const
    {
        return m_defaultFbo;
    }
 
    void setDefaultFbo(QOpenGLFramebufferObject* newDefaultFbo){
        if(m_defaultFbo == newDefaultFbo) return;
        m_defaultFbo = newDefaultFbo;
        emit defaultFboChanged(m_defaultFbo);
    }
 
    osg::Camera* createRTTCamera() const;
    osgViewer::Viewer* rawViewer() const{return m_viewer.get();}

    void setMapNode(const osg::ref_ptr<osgEarth::MapNode> &mapNode);

public slots:
    void addChild(osg::Node* childNode, osg::Node* parentNode = nullptr);
    void testAddChild();
protected:
    virtual QQuickFramebufferObject::Renderer* createRenderer() const override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void hoverMoveEvent(QHoverEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
 
private:
    void setKeyboardModifiers(QInputEvent* event);
    void setMouseEventData(QMouseEvent* event);
    void setHoverEventData(QHoverEvent* event);
 
    int getOsgKey(QKeyEvent* event);
 
signals:
    void framerateChanged();
    void sceneChanged();
    void hoverEnabledChanged();
    void cameraChanged();
    void sceneEvent(QString event);
    void defaultFboChanged(QOpenGLFramebufferObject* defaultFbo);
 
private:
    int m_keyMask;
    int m_mouseX, m_mouseY;
    int m_mouseBtn;   //Button numbering is 1 for left mouse button, 2 for middle, 3 for right.
    QColor m_clearColor = QColor(Qt::white);
    bool m_hoverEnabled = false;
    osg::ref_ptr<osgGA::EventQueue> m_eventQueue;
    osg::ref_ptr<osg::OperationQueue> m_operationQueue;
    osg::ref_ptr<osg::Node> m_sceneNode;
    osg::ref_ptr<osg::Switch> m_invisibleGroup;
    osg::ref_ptr<osg::Camera> m_camera;
    osg::NodePath m_nodePath;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    int m_framerate = 60;
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    QTimer m_updateTimer;
    QOpenGLFramebufferObject* m_defaultFbo = nullptr;
};
 
Q_DECLARE_METATYPE(osg::Node*)
Q_DECLARE_METATYPE(osg::Operation*)
Q_DECLARE_METATYPE(osg::Vec3Array*)
Q_DECLARE_METATYPE(osg::DrawElementsUInt*)
 
 
#endif // QUICKOSGEARTHVIEWER_H
