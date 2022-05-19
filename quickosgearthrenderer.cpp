#include "quickosgearthrenderer.h"
#include "quickosgearthviewer.h"
#include <QtDebug>
#include <QQuickWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgViewer/GraphicsWindow>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <QtMath>
#include<osgShadow/SoftShadowMap>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osg/BoundingSphere>
#include <osg/AutoTransform>
#include <osgViewer/Viewer>
//#include "viewerdatum.h"
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/Config>
#include <osgEarth/MapNode>
#include <osgEarth/MapNodeOptions>
#include <osgEarth/TerrainOptions>
#include <osgEarth/OverlayDecorator>
#include <osgEarth/TileRasterizer>
#include <osgEarthDrivers/engine_rex/RexTerrainEngineNode>
#include <QDebug>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QDir>
#include <QFileInfo>

QuickOSGEarthRenderer::QuickOSGEarthRenderer(QuickOSGEarthViewer* viewer)
    : m_quickosgEarthViewer(viewer)
{
    m_eventQueue = viewer->m_eventQueue;
    m_operationQueue = viewer->m_operationQueue;
 
    initOSG();
    viewer->m_camera = m_osgViewer->getCamera();
    viewer->m_viewer = m_osgViewer.get();
}
 
QuickOSGEarthRenderer::~QuickOSGEarthRenderer()
{
    qDebug()<<"Renderer destroyed.";
}
 
class CustomDrawCallback:public osg::Drawable::DrawCallback
{
 
 
    // DrawCallback interface
public:
    void drawImplementation(osg::RenderInfo &ri, const osg::Drawable *drawable) const{
        osg::State& state = *ri.getState();
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *funcs = ctx->functions();
        int i;
        ::glGetIntegerv(GL_ACTIVE_TEXTURE,&i);
        ::glGetIntegerv(GL_FRAMEBUFFER_BINDING,&i);
        i=1;
 
        i=i+1;
        ::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POLYGON);
 
        glEnd();
        glFlush();
    }
};
void QuickOSGEarthRenderer::initOSG()
{
    m_osgViewer = new osgViewer::Viewer();
    m_subOsgViewer = new osgViewer::Viewer();

    osg::ref_ptr< osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator();
    m_osgViewer->setCameraManipulator(earthManipulator);

    osg::ref_ptr< osgEarth::Util::EarthManipulator> subEarthManipulator = new osgEarth::Util::EarthManipulator();
    m_subOsgViewer->setCameraManipulator(subEarthManipulator);


    osg::ref_ptr<osg::Camera> camera = m_osgViewer->getCamera();

//    camera->setCullingMode(osg::CullSettings::NO_CULLING);
    camera->setProjectionMatrixAsOrtho(0.0, 1.0, 0.0, 1.0, 0.1, 30000);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
 
    m_osgViewer->setUpViewerAsEmbeddedInWindow(0, 0, 2560, 1440);
    m_subOsgViewer->setUpViewerAsEmbeddedInWindow(0,0,200,200);

//    m_quickosgEarthViewer->resetWidth();  // 触发geometryChanged，改变渲染窗口大小
    m_quickosgEarthViewer->setCamera(m_osgViewer->getCamera());
 
    m_osgViewer->getCamera()->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
    m_osgViewer->getCamera()->setClearDepth(1);
 
    m_graphicsWindow = dynamic_cast<osgViewer::GraphicsWindow*>(m_osgViewer->getCamera()->getGraphicsContext());
    //auto gc = m_osgViewer->getCamera()->getGraphicsContext();
 
    osg::State* state = m_graphicsWindow->getState();
    state->setUseModelViewAndProjectionUniforms(true);
    //state->setUseVertexAttributeAliasing(true);
 
    m_graphicsWindow->setEventQueue(m_eventQueue);
    m_osgViewer->setUpdateOperations(m_operationQueue);
 

//    GLenum result = funcs->glGetError();
 
//    const GLubyte* name = funcs->glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
//    const GLubyte* biaoshifu = funcs->glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
//    const GLubyte* OpenGLVersion = funcs->glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
//    //const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本
//    const GLubyte* glslVersion = funcs->glGetString(GL_SHADING_LANGUAGE_VERSION);
 
 
//    osg::ref_ptr<osgDB::Options> myReadOptions = osgEarth::Registry::cloneOrCreateOptions(0);
//    osgEarth::Config c;
//    c.add("elevation_smoothing", false);
//    osgEarth::TerrainOptions to(c);
//    osgEarth::MapNodeOptions defMNO;
//    defMNO.setTerrainOptions( to );
//    myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
    _node_earth = osgDB::readNodeFile("D:/work/doc/osgEarth/offline/offline.earth");
 
    osgEarth::MapNode* mapNode = osgEarth::MapNode::get(_node_earth);
//    osg::Group* container=dynamic_cast<osg::Group*>(mapNode->getChild(1));
//    osgEarth::OverlayDecorator* deco= dynamic_cast<osgEarth::OverlayDecorator*>(container->getChild(0));
//    osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode* Rexter=dynamic_cast<osgEarth::Drivers::RexTerrainEngine::RexTerrainEngineNode*>(deco->getChild(0));
//    unsigned int num = Rexter->getNumChildren();
//    qDebug() << "num:==========================" << num;
//    osgEarth::TileRasterizer* ras= dynamic_cast<osgEarth::TileRasterizer*>(Rexter->getChild(5));
//    ras->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);

 

 
 
//    GLint framebuffer;
//    funcs->glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
 
//    result = funcs->glGetError();
//    name = funcs->glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
//    result = funcs->glGetError();
//    biaoshifu = funcs->glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
//    OpenGLVersion = funcs->glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
//    //const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本
//    glslVersion = funcs->glGetString(GL_SHADING_LANGUAGE_VERSION);
//    result = funcs->glGetError();
//    result = funcs->glGetError();
//    result = funcs->glGetError();
//    GL_INVALID_OPERATION;
 
    m_rootNode = new osg::Group;
//    osg::Drawable* drawable=new osg::Drawable;
//    CustomDrawCallback* callback = new CustomDrawCallback;
//    drawable->setDrawCallback(callback);
//    m_rootNode->addChild(drawable);
    m_rootNode->addChild( _node_earth );
    m_quickosgEarthViewer->setMapNode(mapNode);

//    std::string filePath = "D:/work/doc/osgb";
//    osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
//    csn->setEllipsoidModel(new osg::EllipsoidModel);
//    m_rootNode->addChild(LayOSGB(csn, filePath, 116.0, 40.0, 100));
    m_osgViewer->setSceneData( m_rootNode.get() );
    m_subOsgViewer->setSceneData(m_rootNode.get());

//    earthManipulator->setViewpoint(osgEarth::Viewpoint("osgb", 116.0, 40.0, 0, 0.0, -90, 2e4));


    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QOpenGLFunctions *funcs = ctx->functions();
    QSurfaceFormat format=ctx->format();
    ctx->setFormat(format);
    ctx->makeCurrent(ctx->surface());
}

void QuickOSGEarthRenderer::findDir(const QString &dirPath, QStringList &subDirs)
{
    subDirs.clear();
    QDir dir(dirPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);
    foreach(QFileInfo fileInfo, fileInfoList){
        subDirs.append(fileInfo.filePath());
    }
}

osg::ref_ptr<osg::Group> QuickOSGEarthRenderer::createObliqueIndexes(const QString &filePath)
{
    osg::ref_ptr<osg::Group> group = new osg::Group();
    QStringList subDirs;
    //返回所有子文件夹路径
    findDir(filePath, subDirs);

    for (size_t i = 0; i < subDirs.size(); i++)
    {
        QString name = subDirs.at(i);
        QString baseName = QFileInfo(name).baseName();

        std::string path = name.toStdString() + "\\" + baseName.toStdString() + ".osgb";
        std::cout << path << std::endl;

        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
        osg::ref_ptr<osg::PagedLOD> lod = new osg::PagedLOD();

        auto bs = node->getBound();
        auto c = bs.center();
        auto r = bs.radius();
        lod->addChild(node);
        lod->setCenter(c);
        lod->setRadius(r);
        lod->setRangeMode(osg::LOD::RangeMode::PIXEL_SIZE_ON_SCREEN);

        //解决飞机黑色问题
        lod->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
        //解决飞机模型被遮挡问题
        lod->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        lod->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

        QString relativeFilePath = "./" + baseName + "/" + baseName + ".osgb";  //相对路径

        lod->setFileName(0, "");
        lod->setFileName(1, relativeFilePath.toStdString());

        lod->setRange(0, 0.0, FLT_MAX);			//第一层不可见
        //lod->setRange(1, 1.0, FLT_MAX);

        lod->setDatabasePath("");

        group->addChild(lod);

    }
    //std::string outputLodFile = fileDir + "/Data.osgb";
    //osgDB::writeNodeFile(*group, outputLodFile);
    return group;
}

osg::ref_ptr<osg::Group> QuickOSGEarthRenderer::LayOSGB(osg::ref_ptr<osg::CoordinateSystemNode> csn, std::string fileDir, double lng, double lat, double height)
{
    osg::ref_ptr<osg::Group> osgbGroup = new osg::Group;

    //读取模型
    osg::ref_ptr<osg::Node> OSGB = createObliqueIndexes(QString::fromStdString(fileDir))->asNode();
    //创建矩阵变换节点
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->addChild(OSGB);
    osg::Matrix matrix;
    csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(
        osg::DegreesToRadians(lat), osg::DegreesToRadians(lng), height, matrix);
    //在倾斜摄影模型外绘制包围球，方便测试时找到osgb数据
    osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
    osg::BoundingSphere bs = mt->getBound();
    osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(new osg::Sphere(bs.center(), bs.radius()));
    sd->setColor(osg::Vec4(1, 0, 0, 0.2));
    gnode->addChild(sd);
    //mt->addChild(gnode);
    gnode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateSet::TRANSPARENT_BIN);
    gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    mt->addChild(OSGB.get());
    mt->setMatrix(matrix);
    osgbGroup->addChild(mt);
    return osgbGroup;
}
 
// 此处不做互斥同步处理是因为Qt保证在调用该sync函数时GUI线程是阻塞的
// 看Qt源代码也可以发现，在进入该函数之前，renderer线程使用mutex进行了互斥操作
void QuickOSGEarthRenderer::synchronize(QQuickFramebufferObject* item)
{
//    m_rootNode->addChild( _node_cow );
//    m_rootNode->addChild( _node_earth );
    m_quickosgEarthViewer = qobject_cast<QuickOSGEarthViewer*>(item);
 
    if(m_sceneNode != m_quickosgEarthViewer->scene()){
        if(m_sceneNode.valid())
            m_rootNode->replaceChild(m_sceneNode.get(), m_quickosgEarthViewer->scene());
        else
            m_rootNode->addChild(m_quickosgEarthViewer->scene());
        m_sceneNode = m_quickosgEarthViewer->scene();
    }
}
 
void QuickOSGEarthRenderer::render()
{
    QOpenGLContext::currentContext()->functions()->glUseProgram(0);
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QOpenGLFunctions *funcs = ctx->functions();
    GLenum result = funcs->glGetError();
    GLint activeTexture,framebuffer;
    funcs->glGetIntegerv(GL_ACTIVE_TEXTURE,&activeTexture);
    funcs->glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
    m_osgViewer->frame();
    m_subOsgViewer->frame();
    funcs->glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
    funcs->glGetIntegerv(GL_ACTIVE_TEXTURE,&activeTexture);
    funcs->glActiveTexture(GL_TEXTURE0);
    funcs->glGetIntegerv(GL_ACTIVE_TEXTURE,&activeTexture);
    result = funcs->glGetError();

    m_quickosgEarthViewer->window()->resetOpenGLState();
}
 
QOpenGLFramebufferObject* QuickOSGEarthRenderer::createFramebufferObject(const QSize& size)
{
//    QOpenGLContext *ctx = QOpenGLContext::currentContext();
//    ctx->makeCurrent(ctx->surface());
//    QOpenGLFunctions *funcs = ctx->functions();
//    GLenum result = funcs->glGetError();
 
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    //format.setSamples(4);
    auto fbo = new QOpenGLFramebufferObject(size, format);
    m_quickosgEarthViewer->setDefaultFbo(fbo);
    return fbo;
}
