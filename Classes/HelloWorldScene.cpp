#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "LightLineRender.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld*		layer = HelloWorld::create();
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    auto sprite = Sprite::create("Sea.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite);
    
	//创建鱼所在的层
	m_FishLayer = FishLayer::create();
	m_FishLayer->retain();
	this->addChild(m_FishLayer);


	//线条容器
	std::vector<LightLineRender::Line> lines;
	//设置线条位置
	//第一段闪电的起点和终点
	Vec3 segStart = Vec3(-50,-50,-8);
	Vec3 segEnd   = Vec3(50,50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//第二段闪电的起点和终点
	segStart = Vec3(50,50,-8);
	segEnd   = Vec3(-50,50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//第三段闪电的起点和终点
	segStart = Vec3(-50,50,-8);
	segEnd   = Vec3(50,-50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//第四段闪电的起点和终点
	segStart = Vec3(50,-50,-8);
	segEnd   = Vec3(0,100,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//第五段闪电的起点和终点
	segStart = Vec3(0,100,-8);
	segEnd   = Vec3(-50,-50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//创建出闪光链
	LightLineRender*	_lighting = LightLineRender::create();
	//设置不需要强制纹理循环
    _lighting->setForceTexLoop( false );
	//设置宽
	_lighting->setWidth( 80 );
	//设置 单张纹理长度，调整这个数值可以避免纹理过度拉伸或挤压
	_lighting->setTextueLength( 100 );
	//设置单个面片网格长，越小曲线越平滑，数值过于小可能带来效率问题
	_lighting->setStep( 10 );
	//设置振幅1
	_lighting->setAmplitude0( 4 );
	//设置频率1
	_lighting->setFrequency0( 500 );
	//设置振幅2
	_lighting->setAmplitude1( 1 );
	//设置频率2
	_lighting->setFrequency1( 400 );
	//设置
	_lighting->setTimeFactor( 0.5 );
	//使用线段容器创建闪电链
	_lighting->setLines( lines );
	//使用柏林噪音算法
	_lighting->setLineType( LineType::LT_PerlinNosie );
	//设置每帧强制更新重建模型
	_lighting->setForceUpdate(true);

	_lighting->setPosition(Vec2(visibleSize.width / 4 + origin.x,visibleSize.height / 2 + origin.y));
	//将闪电链加入到当前层中。
	this->addChild(_lighting,0,10);

	//设置可以点击
	setTouchEnabled( true );
    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//触屏事件处理
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	//屏幕转换到射线
	kmVec3    tPt;
	kmVec3	  tDir;
	// 获取点在视图中的坐标
	CCPoint touchLocation = touches[0]->getLocation();
	auto    visibleSize = Director::getInstance()->getVisibleSize();
	auto	origin = Director::getInstance()->getVisibleOrigin();
	//线条容器
	std::vector<LightLineRender::Line> lines;
	//设置线条位置

	//闪电的起点和终点
	Vec2	tFishPos(Vec2(visibleSize / 2) + origin);
	tFishPos = m_FishLayer->GetSpritePosition() + origin;
	Vec3 segStart = Vec3(0,0,-8);
	Vec3 segEnd   = Vec3(touchLocation.x - tFishPos.x ,touchLocation.y - tFishPos.y ,-8);
	//取得方向
	Vec3  dir = segEnd - segStart ;
	float fLength = dir.length();
	dir.normalize();
	//顺时针转动45度形成一个偏移点做为第一个闪电链线段。
	Vec3  rotate_left;
	Mat4  rotate_left_Mat;
	kmMat4RotationZ(&rotate_left_Mat,MATH_DEG_TO_RAD(-45));
	kmVec3TransformCoord(&rotate_left,&dir,&rotate_left_Mat);
	rotate_left.normalize();
	//逆时针转动45度形成一个偏移点做为第一个闪电链线段。
	Vec3  rotate_right;
	Mat4  rotate_right_Mat;
	kmMat4RotationZ(&rotate_right_Mat,MATH_DEG_TO_RAD(45));
	kmVec3TransformCoord(&rotate_right,&dir,&rotate_right_Mat);
	rotate_right.normalize();

	//分成三段闪电链
	Vec3  v1_s = segStart ;
	Vec3  v1_e = segStart + dir * fLength / 4.0 + rotate_left * (fLength / 6.0);

	Vec3  v2_s = v1_e ;
	Vec3  v2_e = segStart + dir * fLength / 2.0 + rotate_right * (fLength / 6.0);

	Vec3  v3_s = v2_e ;
	Vec3  v3_e = segEnd;

	lines.push_back( LightLineRender::Line( v1_s, v1_e, 0 ) );
	lines.push_back( LightLineRender::Line( v2_s, v2_e, 0 ) );
	lines.push_back( LightLineRender::Line( v3_s, v3_e, 0 ) );
	//创建出闪光链
	LightLineRender*	_lighting = dynamic_cast<LightLineRender*>(getChildByTag(10));
	//使用线段容器创建闪电链
	_lighting->setLines( lines );
	_lighting->setPosition(tFishPos);
	//这一句可以让闪电链在1秒内渐渐消隐。它通过调节Shader中的u_color值从1变为0来实现。
	_lighting->OpenAlphaToZero(1.0);
	//击中乌龟，让乌龟翻身。
	m_FishLayer->AttackWuGui();
}