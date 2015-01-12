#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if(!Layer::init()) return false;

    _batchNode = CCSpriteBatchNode::create("Spritesheets/Sprites.pvr.ccz");
    this->addChild(_batchNode);

    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("Spritesheets/Sprites.plist");
    
    ///add the ship
    _ship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    Size winSize = Director::getInstance()->getWinSize();
    _ship->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);
    
    
    //create the parallaxNode
    _backgroundNode = ParallaxNode::create();
    this->addChild(_backgroundNode, -1);
    //create the sprites, then add them to the parallaxNode
    _spaceDust1 = Sprite::create("");
    _spaceDust2 = Sprite::create();
    _planetSunrise = Sprite::create();
    _galaxy = Sprite::create();
    _spacialAnomaly = Sprite::create();
    _spaceialAnomaly2 = Sprite::create();
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
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
