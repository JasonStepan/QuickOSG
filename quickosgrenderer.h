#ifndef _OSG_QUICKRENDERER_H_
#define _OSG_QUICKRENDERER_H_

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>

#include <osg/Switch>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>


class QuickOSGViewer;
class QuickOSGRenderer;

class QuickOSGRenderer : public QQuickFramebufferObject::Renderer
{
public:
    friend class QuickOSGViewer;
    QuickOSGRenderer(QuickOSGViewer* viewer);
     ~QuickOSGRenderer() override;
    void initOSG();

    osgViewer::Viewer* osgViewer() const { return m_osgViewer.get(); }

    static void findDir(const QString& dirPath, QStringList& subDirs);
    osg::ref_ptr<osg::Group> createObliqueIndexes(const QString& filePath);
    osg::ref_ptr<osg::Group> LayOSGB(osg::ref_ptr<osg::CoordinateSystemNode> csn, std::string fileDir, double lng, double lat, double height);
protected:
    virtual QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    virtual void synchronize(QQuickFramebufferObject* item) override;
    virtual void render() override;

private:
    osg::ref_ptr<osg::StateSet> m_rootStateSet;
    osg::ref_ptr<osgViewer::GraphicsWindow> m_graphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> m_osgViewer;
    QuickOSGViewer* m_quickViewer;
    QColor m_clearColor = QColor(Qt::red);
    osg::ref_ptr<osgGA::EventQueue> m_eventQueue;
    osg::ref_ptr<osg::OperationQueue> m_operationQueue;
    osg::ref_ptr<osg::Node> m_sceneNode;
    osg::ref_ptr<osg::Group> m_rootNode;
};

#endif // _OSG_QUICKRENDERER_H_
