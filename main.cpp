#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include "appglobal.h"
//#include "quickosgviewer.h"
#include "quickosgearthviewer.h"
#include "fyeventadapter.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <QDir>
#include <iostream>
#include <string>
#include <QQuickView>

#include <osgEarth/MapNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarth/ImageLayer>
#include <osgEarth/Viewpoint>
#include <osgEarth/GeoTransform>
#include <osgEarth/ModelLayer>
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
//#include <gdal_priv.h>

using namespace std;

#include<osg/ShapeDrawable>
#include <osg/Notify>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/MouseCoordsTool>
#include <osgEarthUtil/MGRSFormatter>
#include <osgEarthUtil/LatLongFormatter>

#include <osgEarthUtil/GeodeticGraticule>
#include <osgEarthUtil/MGRSGraticule>
#include <osgEarthUtil/UTMGraticule>
#include <osgEarthUtil/GraticuleLabelingEngine>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/gdal/GDALOptions>

using namespace osgEarth::Util;
using namespace osgEarth::Drivers;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QGuiApplication app(argc, argv);
    qmlRegisterType<QuickOSGEarthViewer>("QuickOSGEarth", 1, 0, "QuickOSGEarthViewer");
    qmlRegisterType<AppGlobal>("CppBackend", 1, 0, "AppGlobal");
//    qmlRegisterType<QuickOSGViewer>("QuickOSG", 1, 0, "QuickOSGViewer");
    auto view = new QQuickView();
    view->engine()->addImportPath(QLatin1Literal("qrc:/"));

    // 构造app全局变量，将engine传入该全局变量构造函数
    auto appGlobal = new AppGlobal();

    view->rootContext()->setContextProperty("$app", appGlobal);
    view->rootContext()->setContextProperty("$applicationPath", app.applicationDirPath());

    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view->show();

    auto result = app.exec();

    delete view;
    return result;


//====================================================================================================
//    CPLSetConfigOption("GDAL_DATA", "D:/work/demo/QuickOSG-master/3rd/windows/OSG_3rd/gdal-data");
//    //string wktString = "EPSG:3857";			//web墨卡托投影
//    //string wktString = "EPSG:4326";			//wgs84
//    osgEarth::ProfileOptions profileOpts;
//    //profileOpts.srsString() = wktString;

//    //osgEarth::Bounds bs(535139, 3365107, 545139, 3375107);
//    //osgEarth::Bounds bs(73, 3, 135, 53);
//    //profileOpts.bounds() = bs;

//    //地图配置：设置缓存目录
//    osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
//    string cacheDir =  "D:/work/doc/osgb";
//    cacheOpts.rootPath() = cacheDir;

//    //
//    osgEarth::MapOptions mapOpts;
//    mapOpts.cache() = cacheOpts;
//    //mapOpts.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;

//    mapOpts.profile() = profileOpts;

//    //创建地图节点
//    osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map(mapOpts);
//    osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);

//    osgEarth::Drivers::GDALOptions gdal;
//    //gdal.url() = "D:/Work/OSGNewBuild/osgearth-2.10.1/data/world.tif";
//    //gdal.url() = "D:/Work/SinianGIS/bin/Resource/BlueMarbleNASA.jpg";
////    gdal.url() = "D:/Work/SinianGIS/bin/Resource/baseMap.jpg";
//    osg::ref_ptr<osgEarth::ImageLayer> imgLayer = new osgEarth::ImageLayer("BlueMarble", gdal);
//    map->addLayer(imgLayer);

//    AddModel(map, mapNode);

//    osgViewer::Viewer viewer;
//    viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0));
//    viewer.setSceneData(mapNode);

//    osg::ref_ptr< osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
//    viewer.setCameraManipulator(mainManipulator);

//    osgEarth::Viewpoint vp;
//    osgEarth::GeoPoint newPoint(map->getSRS(), 108.9594, 34.2196, 0);
//    vp.focalPoint() = newPoint;
//    vp.heading() = 0;
//    vp.pitch() = -90;
//    vp.range() = 1000;
//    mainManipulator->setViewpoint(vp);

//    viewer.setUpViewInWindow(100, 100, 800, 600);

//    return viewer.run();
//    osgEarth::ProfileOptions profileOpts;

    //地图配置：设置缓存目录
//    osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
//    string cacheDir = "D:/work/tmp";
//    cacheOpts.rootPath() = cacheDir;

//    osgEarth::MapOptions mapOpts;
//    mapOpts.cache() = cacheOpts;
//    mapOpts.profile() = profileOpts;

//    //创建地图节点
//    osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map(mapOpts);
//    osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);

//    osgEarth::Drivers::GDALOptions gdal;
//    gdal.url() = "E:/BaiduNetdiskDownload/osg3.6.3+osgearth2.10+qt5.12/Data/world.tif";
//    osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer("BlueMarble", gdal);
//    map->addLayer(layer);

//    osgViewer::Viewer viewer;
//    viewer.setSceneData(mapNode);

//    osg::ref_ptr< osgEarth::Util::EarthManipulator> mainManipulator = new osgEarth::Util::EarthManipulator;
//    viewer.setCameraManipulator(mainManipulator);

//    viewer.setUpViewInWindow(100, 100, 800, 600);
//    return viewer.run();

//    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//    osg::ref_ptr<osg::Group> rootGroup = new osg::Group();
//    osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
//    csn->setEllipsoidModel(new osg::EllipsoidModel);


//    //添加状态事件，可以相应键盘和鼠标事件，响应L T B W
//    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
//    //窗口大小变化，响应F
//    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
//    //添加路径记录 Z
//    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
//    //帮助文档显示H
//    viewer->addEventHandler(new osgViewer::HelpHandler);
//    //截屏 C
//    viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
//    //添加一些常用状态设置，响应S
//    viewer->addEventHandler(new osgViewer::StatsHandler);
//    //操作器
//    osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator;
//    //设置相机操作器
//    viewer->setCameraManipulator(earthManipulator);
//    //设置现场数据
//    viewer->setSceneData(rootGroup.get());
//    //实现
//    viewer->realize();

//    osg::ref_ptr<osg::Node> earth = osgDB::readNodeFile("E:/BaiduNetdiskDownload/offline/offline.earth");
//    rootGroup->addChild(earth);
//    string filePath = "./model";
//    rootGroup->addChild(LayOSGB(csn, filePath, 116.0, 40.0, 100));
//    //解决飞机黑色问题
//    rootGroup->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
//    //解决飞机模型被遮挡问题
//    rootGroup->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
//    rootGroup->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

//    earthManipulator->setViewpoint(osgEarth::Viewpoint("osgb", 116.0, 40.0, 0, 0.0, -90, 2e4));

////    viewer->setSceneData(rootGroup.get());
////    viewer->realize();
//    return viewer->run();

}
