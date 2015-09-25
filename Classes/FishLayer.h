#ifndef _FISHLAYER_H
#define _FISHLAYER_H
#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "3d/CCMesh.h"
#include "Math/Vec2.h"

NS_CC_BEGIN 
//这是用于显示3D鱼模型的层
class FishLayer : public cocos2d::Layer
{
public:
	//初始化当前层
    virtual bool init();  
    CREATE_FUNC(FishLayer);
	//重载绘制函数
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
	//击中乌龟
	void    AttackWuGui();
	//继续游动
	void    ContinueSwim();
	//取得精灵位置
	Vec2	GetSpritePosition();
protected:

	//鱼精灵
    cocos2d::Sprite3D*		m_Sprite;
	//骨骼动画
	cocos2d::Animation3D*	m_Animation3D;
	//向前游的动作
    cocos2d::Animate3D*		m_Swim;
	//受伤的动作
    cocos2d::Animate3D*		m_Hurt;
	//uv滚动值
	Vec2					m_LightAni;
}
;

NS_CC_END

#endif