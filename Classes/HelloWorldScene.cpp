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
    
	//���������ڵĲ�
	m_FishLayer = FishLayer::create();
	m_FishLayer->retain();
	this->addChild(m_FishLayer);


	//��������
	std::vector<LightLineRender::Line> lines;
	//��������λ��
	//��һ������������յ�
	Vec3 segStart = Vec3(-50,-50,-8);
	Vec3 segEnd   = Vec3(50,50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//�ڶ�������������յ�
	segStart = Vec3(50,50,-8);
	segEnd   = Vec3(-50,50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//����������������յ�
	segStart = Vec3(-50,50,-8);
	segEnd   = Vec3(50,-50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//���Ķ�����������յ�
	segStart = Vec3(50,-50,-8);
	segEnd   = Vec3(0,100,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//���������������յ�
	segStart = Vec3(0,100,-8);
	segEnd   = Vec3(-50,-50,-8);
	lines.push_back( LightLineRender::Line( segStart, segEnd, 0 ) );
	//������������
	LightLineRender*	_lighting = LightLineRender::create();
	//���ò���Ҫǿ������ѭ��
    _lighting->setForceTexLoop( false );
	//���ÿ�
	_lighting->setWidth( 80 );
	//���� ���������ȣ����������ֵ���Ա���������������ѹ
	_lighting->setTextueLength( 100 );
	//���õ�����Ƭ���񳤣�ԽС����Խƽ������ֵ����С���ܴ���Ч������
	_lighting->setStep( 10 );
	//�������1
	_lighting->setAmplitude0( 4 );
	//����Ƶ��1
	_lighting->setFrequency0( 500 );
	//�������2
	_lighting->setAmplitude1( 1 );
	//����Ƶ��2
	_lighting->setFrequency1( 400 );
	//����
	_lighting->setTimeFactor( 0.5 );
	//ʹ���߶���������������
	_lighting->setLines( lines );
	//ʹ�ð��������㷨
	_lighting->setLineType( LineType::LT_PerlinNosie );
	//����ÿ֡ǿ�Ƹ����ؽ�ģ��
	_lighting->setForceUpdate(true);

	_lighting->setPosition(Vec2(visibleSize.width / 4 + origin.x,visibleSize.height / 2 + origin.y));
	//�����������뵽��ǰ���С�
	this->addChild(_lighting,0,10);

	//���ÿ��Ե��
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

//�����¼�����
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	//��Ļת��������
	kmVec3    tPt;
	kmVec3	  tDir;
	// ��ȡ������ͼ�е�����
	CCPoint touchLocation = touches[0]->getLocation();
	auto    visibleSize = Director::getInstance()->getVisibleSize();
	auto	origin = Director::getInstance()->getVisibleOrigin();
	//��������
	std::vector<LightLineRender::Line> lines;
	//��������λ��

	//����������յ�
	Vec2	tFishPos(Vec2(visibleSize / 2) + origin);
	tFishPos = m_FishLayer->GetSpritePosition() + origin;
	Vec3 segStart = Vec3(0,0,-8);
	Vec3 segEnd   = Vec3(touchLocation.x - tFishPos.x ,touchLocation.y - tFishPos.y ,-8);
	//ȡ�÷���
	Vec3  dir = segEnd - segStart ;
	float fLength = dir.length();
	dir.normalize();
	//˳ʱ��ת��45���γ�һ��ƫ�Ƶ���Ϊ��һ���������߶Ρ�
	Vec3  rotate_left;
	Mat4  rotate_left_Mat;
	kmMat4RotationZ(&rotate_left_Mat,MATH_DEG_TO_RAD(-45));
	kmVec3TransformCoord(&rotate_left,&dir,&rotate_left_Mat);
	rotate_left.normalize();
	//��ʱ��ת��45���γ�һ��ƫ�Ƶ���Ϊ��һ���������߶Ρ�
	Vec3  rotate_right;
	Mat4  rotate_right_Mat;
	kmMat4RotationZ(&rotate_right_Mat,MATH_DEG_TO_RAD(45));
	kmVec3TransformCoord(&rotate_right,&dir,&rotate_right_Mat);
	rotate_right.normalize();

	//�ֳ�����������
	Vec3  v1_s = segStart ;
	Vec3  v1_e = segStart + dir * fLength / 4.0 + rotate_left * (fLength / 6.0);

	Vec3  v2_s = v1_e ;
	Vec3  v2_e = segStart + dir * fLength / 2.0 + rotate_right * (fLength / 6.0);

	Vec3  v3_s = v2_e ;
	Vec3  v3_e = segEnd;

	lines.push_back( LightLineRender::Line( v1_s, v1_e, 0 ) );
	lines.push_back( LightLineRender::Line( v2_s, v2_e, 0 ) );
	lines.push_back( LightLineRender::Line( v3_s, v3_e, 0 ) );
	//������������
	LightLineRender*	_lighting = dynamic_cast<LightLineRender*>(getChildByTag(10));
	//ʹ���߶���������������
	_lighting->setLines( lines );
	_lighting->setPosition(tFishPos);
	//��һ���������������1���ڽ�����������ͨ������Shader�е�u_colorֵ��1��Ϊ0��ʵ�֡�
	_lighting->OpenAlphaToZero(1.0);
	//�����ڹ꣬���ڹ귭��
	m_FishLayer->AttackWuGui();
}