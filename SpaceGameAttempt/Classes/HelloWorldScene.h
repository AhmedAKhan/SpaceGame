#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	virtual void onAcceleration(Acceleration* acc, Event* event);
    //virtual void ccTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event);
    
    float randomValueBetween(float low, float high);
    void setInvisible(Node * node);
    unsigned long getTimeTick();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    //variables
    SpriteBatchNode * _batchNode;
    Sprite * _ship;
    
    ParallaxNodeExtras * _backgroundNode;
    Sprite * _spaceDust1;
    Sprite * _spaceDust2;
    Sprite * _planetSunrise;
    Sprite * _galaxy;
    Sprite * _spacialAnomaly;
    Sprite * _spaceialAnomaly2;
    
    float _shipPointsPerSecY;
    
    void update(float dt);
    
    //variables needed for asteroid
    Vector<Sprite *> * _asteroids;
    int _nextAsteroid;
    unsigned long _nextAsteroidSpawn;
    
    Vector<Sprite *> _shipLasers;
    int _nextShipLaser;
    
};

#endif // __HELLOWORLD_SCENE_H__
