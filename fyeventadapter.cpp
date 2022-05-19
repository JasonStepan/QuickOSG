#include "fyeventadapter.h"

FYEventAdapter::FYEventAdapter(QQuickItem *parent):
    fbo(nullptr),
    texture(nullptr),
    textureNode(nullptr)
{
    initOSG();
    connect(this, SIGNAL(windowChanged(QQuickWindow*)),this, SLOT(onWindowChanged(QQuickWindow*)));

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

osgViewer::Viewer *FYEventAdapter::getViewer()
{
    return viewer.get();
}

void FYEventAdapter::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (window())
        {
            updateViewport();
        }

        QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void FYEventAdapter::mousePressEvent(QMouseEvent *event)
{
    int button = mouseButton(event);
    viewer->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void FYEventAdapter::mouseMoveEvent(QMouseEvent *event)
{
    viewer->getEventQueue()->mouseMotion(event->x(), event->y());
}

void FYEventAdapter::mouseReleaseEvent(QMouseEvent *event)
{
    int button = mouseButton(event);
    viewer->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void FYEventAdapter::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        viewer->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    else
        viewer->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);

}

void FYEventAdapter::keyPressEvent(QKeyEvent *event)
{
    viewer->getEventQueue()->keyPress(static_cast<int>(*(event->text().toLatin1().data())));
}

QSGNode *FYEventAdapter::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if (oldNode && oldNode != textureNode) {
        delete oldNode;
    }
    Q_UNUSED(updatePaintNodeData)
    return textureNode;
}

//StateSetManipulator
void FYEventAdapter::initOSG()
{
    viewer = new osgViewer::Viewer();
//    viewer->addEventHandler( new osgGA::StateSet(viewer->getCamera()->getOrCreateStateSet()) );
    viewer->addEventHandler(new osgViewer::StatsHandler);
    //viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    //viewer->getEventQueue()->getCurrentEventState()
}

void FYEventAdapter::initFBO()
{
    QRectF rect = this->boundingRect();
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    QSize size(rect.size().toSize());
    fbo = new QOpenGLFramebufferObject(size, format);
    texture = this->window()->createTextureFromId(fbo->texture(), size);
    textureNode = new QSGSimpleTextureNode();
    textureNode->setRect(0, this->height(), this->width(), -height());
    textureNode->setTexture(texture);
    this->setFlag(QQuickItem::ItemHasContents, true);
    updateViewport();
    this->update();
}

void FYEventAdapter::updateFBO()
{
    if (fbo)
        delete fbo;
    QRectF rect = this->mapRectToItem(nullptr, boundingRect());
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    QSize size(rect.size().toSize());
    fbo = new QOpenGLFramebufferObject(size, format);
    if (texture)
        delete texture;
    texture = window()->createTextureFromId(fbo->texture(), size);
    textureNode = new QSGSimpleTextureNode();
    textureNode->setRect(0, height(), width(), -this->height());
    textureNode->setTexture(texture);
    this->update();
}

void FYEventAdapter::updateViewport()
{
    if (!this->window())
        return;

    QSize size(this->boundingRect().size().toSize());
    viewer->getCamera()->getGraphicsContext()->resizedImplementation(0, 0, size.width(), size.height());
    osgGA::GUIEventAdapter *ea = viewer->getEventQueue()->getCurrentEventState();
    ea->setXmin(0);
    ea->setXmax(size.width());
    ea->setYmin(0);
    ea->setYmax(size.height());
    viewer->getCamera()->setViewport(0, 0, size.width(), size.height());
    viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(size.width()) / static_cast<double>(size.height()), 0.1f, 10000.0f);
    // 				view->getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    // 				view->getCamera()->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);
    if (texture && texture->textureSize() != size) {
        updateFBO();
    }
}

void FYEventAdapter::onWindowChanged(QQuickWindow *window)
{
    if (!window)
    {
        return;
    }
    connect(window, SIGNAL(beforeRendering()),this, SLOT(frame()), Qt::DirectConnection);
    window->setClearBeforeRendering(false);

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow
            = new osgViewer::GraphicsWindowEmbedded(0, 0, window->width(), window->height());
    graphicsWindow->setClearColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    viewer->getCamera()->setGraphicsContext(graphicsWindow);
    updateViewport();
    connect(window, SIGNAL(widthChanged(int)),
            this, SLOT(updateViewport()));
    connect(window, SIGNAL(heightChanged(int)),
            this, SLOT(updateViewport()));
}

void FYEventAdapter::frame()
{
    window()->update();
    if(!fbo)
        initFBO();
    if(fbo)
    {
        fbo->bind();
    }
    QOpenGLContext::currentContext()->functions()->glUseProgram(0);
    viewer->frame();
}
