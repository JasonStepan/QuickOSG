#include "quickosgrenderer.h"
#include "quickosgviewer.h"
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
#include <osgGA/StateSetManipulator>
#include <QtMath>
#include<osgShadow/SoftShadowMap>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osg/BoundingSphere>
#include <osg/AutoTransform>
#include <osgViewer/Viewer>
#include "viewerdatum.h"
#include <osgEarth/MapNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarth/ImageLayer>
#include <osgEarth/Viewpoint>
#include <osgEarth/GeoTransform>
#include <osgEarth/ModelLayer>
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
#include <QFileInfo>
#include <QDir>

QuickOSGRenderer::QuickOSGRenderer(QuickOSGViewer* viewer)
    : m_quickViewer(viewer)
{
    m_eventQueue = viewer->m_eventQueue;
    m_operationQueue = viewer->m_operationQueue;

    initOSG();
    viewer->m_camera = m_osgViewer->getCamera();
    viewer->m_viewer = m_osgViewer.get();
}

QuickOSGRenderer::~QuickOSGRenderer()
{
    qDebug()<< "Renderer destroyed.";
}


void QuickOSGRenderer::initOSG()
{
    qDebug() << "QuickOSGRenderer::initOSG()";
//    m_osgViewer = new osgViewer::Viewer();
//    auto manipulator = new osgGA::TrackballManipulator;
//    manipulator->setAllowThrow(false);
//    m_osgViewer->setCameraManipulator(manipulator);
//    osg::ref_ptr<osg::Camera> camera = m_osgViewer->getCamera();
//    camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
//    //camera->setCullingMode(osg::CullSettings::NO_CULLING);
//    camera->setProjectionMatrixAsOrtho(0.0, 1.0, 0.0, 1.0, 0.1, 10000);
//    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

//    m_osgViewer->setUpViewerAsEmbeddedInWindow(1000, 1000, 300, 300);
//    m_quickViewer->resetWidth();  // 触发geometryChanged，改变渲染窗口大小
//    m_quickViewer->setCamera(camera);

////    camera->setClearColor(QColor2OSGColor(m_clearColor));
//    camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
//    camera->setClearDepth(100);

//    m_graphicsWindow = dynamic_cast<osgViewer::GraphicsWindow*>(m_osgViewer->getCamera()->getGraphicsContext());
//    //auto gc = m_osgViewer->getCamera()->getGraphicsContext();

//    osg::State* state = m_graphicsWindow->getState();
//    state->setUseModelViewAndProjectionUniforms(true);
////    state->setUseVertexAttributeAliasing(true);

//    m_graphicsWindow->setEventQueue(m_eventQueue);
//    m_osgViewer->setUpdateOperations(m_operationQueue);

//    m_rootNode = new osg::Group;
//    m_rootNode->addChild(m_quickViewer->scene());
//    m_osgViewer->setSceneData(m_rootNode.get());


    m_osgViewer = new osgViewer::Viewer();
    m_rootNode = new osg::Group();
    osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
    csn->setEllipsoidModel(new osg::EllipsoidModel);
    std::string filePath = "./model";
    //添加状态事件，可以相应键盘和鼠标事件，响应L T B W
//    m_osgViewer->addEventHandler(new osgGA::StateSetManipulator(m_osgViewer->getCamera()->getOrCreateStateSet()));
//    //窗口大小变化，响应F
//    m_osgViewer->addEventHandler(new osgViewer::WindowSizeHandler);
//    //添加路径记录 Z
//    m_osgViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
//    //帮助文档显示H
//    m_osgViewer->addEventHandler(new osgViewer::HelpHandler);
//    //截屏 C
//    m_osgViewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
//    //添加一些常用状态设置，响应S
//    m_osgViewer->addEventHandler(new osgViewer::StatsHandler);
     osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator;
     m_osgViewer->setCameraManipulator(earthManipulator);
     m_osgViewer->setSceneData(m_rootNode.get());
//     m_osgViewer->realize();

     osg::ref_ptr<osg::Node> earth = osgDB::readNodeFile("E:/BaiduNetdiskDownload/offline/offline.earth");
     m_rootNode->addChild(earth);
     m_rootNode->addChild(LayOSGB(csn, filePath, 116.0, 40.0, 100));

     //解决飞机黑色问题
     m_rootNode->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
     //解决飞机模型被遮挡问题
     m_rootNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
     m_rootNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

     earthManipulator->setViewpoint(osgEarth::Viewpoint("osgb", 116.0, 40.0, 0, 0.0, -90, 2e4));

//    auto viewerDatum = new ViewerDatum(2, camera.get());
//    viewerDatum->setViewport(30.0, 0.0, 170.0, 170.0);
//    viewerDatum->setProjectionMatrixAsPerspective(45, 1.0, 0.001, 100000);
//    viewerDatum->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
//    viewerDatum->setViewMatrixAsLookAt(osg::Vec3d(0, 0, 20), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));
//    viewerDatum->setRenderOrder(osg::Camera::POST_RENDER, 1000);
//    viewerDatum->setAllowEventFocus(false);
//    viewerDatum->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    viewerDatum->getOrCreateStateSet()->setMode(GL_LIGHTING,
//                                                  osg::StateAttribute::OFF);
//    viewerDatum->getOrCreateStateSet()->setMode(GL_BLEND,
//                                                  osg::StateAttribute::ON);
//    viewerDatum->setClearMask(GL_DEPTH_BUFFER_BIT);
//    m_rootNode->addChild(viewerDatum);

}

void QuickOSGRenderer::findDir(const QString &dirPath, QStringList &subDirs)
{
    subDirs.clear();
    QDir dir(dirPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);
    foreach(QFileInfo fileInfo, fileInfoList){
        subDirs.append(fileInfo.filePath());
    }
}

osg::ref_ptr<osg::Group> QuickOSGRenderer::createObliqueIndexes(const QString &filePath)
{
    osg::ref_ptr<osg::Group> group = new osg::Group();
    QStringList subDirs;
    //返回所有子文件夹路径
    findDir(filePath, subDirs);

    for (int i = 0; i < subDirs.size(); i++)
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

osg::ref_ptr<osg::Group> QuickOSGRenderer::LayOSGB(osg::ref_ptr<osg::CoordinateSystemNode> csn, std::string fileDir, double lng, double lat, double height)
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
void QuickOSGRenderer::synchronize(QQuickFramebufferObject* item)
{
//    qDebug() << "void QuickOSGRenderer::synchronize";
    m_quickViewer = qobject_cast<QuickOSGViewer*>(item);

    if(m_sceneNode != m_quickViewer->scene()){
        if(m_sceneNode.valid())
            m_rootNode->replaceChild(m_sceneNode.get(), m_quickViewer->scene());
        else
            m_rootNode->addChild(m_quickViewer->scene());
        m_sceneNode = m_quickViewer->scene();
    }
}

void QuickOSGRenderer::render()
{
    QOpenGLContext::currentContext()->functions()->glUseProgram(0);
    m_osgViewer->frame();
//    m_osgViewer->run();
    m_quickViewer->window()->resetOpenGLState();
}

QOpenGLFramebufferObject* QuickOSGRenderer::createFramebufferObject(const QSize& size)
{
    qDebug() << "QOpenGLFramebufferObject* QuickOSGRenderer::createFramebufferObject";
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    auto fbo = new QOpenGLFramebufferObject(size, format);
    m_quickViewer->setDefaultFbo(fbo);
    return fbo;
}
