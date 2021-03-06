#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "FishLayer.h"
NS_CC_BEGIN 
class HelloWorld : public cocos2d::Layer
{
	//捕鱼的层
	FishLayer*				  m_FishLayer;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);
    
  	//触屏事件处理
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};
NS_CC_END
#endif // __HELLOWORLD_SCENE_H__
