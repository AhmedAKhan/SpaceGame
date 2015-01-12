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
    _spaceDust1 = Sprite::create("Backgrounds/bg_front_spacedust.png");
    _spaceDust2 = Sprite::create("Backgrounds/bg_front_spacedust.png");
    _planetSunrise = Sprite::create("Backgrounds/bg_planetsunrise.png");
    _galaxy = Sprite::create("Backgrounds/bg_galaxy.png");
    _spacialAnomaly = Sprite::create("Backgrounds/bg_spacialanomaly.png");
    _spaceialAnomaly2 = Sprite::create("Backgrounds/bg_spacialanomaly2.png");

    //determine the relative movement speed for space dust and background
    Point dustSpeed = Point(0.1, 0.1);
    Point bgSpeed = Point(0.05, 0.05);
    
    // add the children to the parrallax node
    _backgroundNode->addChild(_spaceDust1, 0, dustSpeed, Point(0, winSize.height/2));
    _backgroundNode->addChild(_spaceDust2, 0, dustSpeed, Point(_spaceDust1->getContentSize().width, winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, Point(0, winSize.height * 0.7));
    _backgroundNode->addChild(_planetSunrise, -1, bgSpeed, Point(600, winSize.height * 0));
    _backgroundNode->addChild(_spacialAnomaly, -1, bgSpeed, Point(900, winSize.height * 0.3));
    _backgroundNode->addChild(_spaceialAnomaly2, -1, bgSpeed, Point(1500, winSize.height * 0.9));

    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt){
    Point backgroundScrollVert = Point(-1000, 0);
    _backgroundNode->setPosition(_backgroundNode->getPosition() + backgroundScrollVert * dt);
    
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
