#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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
    _backgroundNode = ParallaxNodeExtras::node();
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
    
    //make the update function run
    this->scheduleUpdate();
    
    //add the stars
    HelloWorld::addChild(ParticleSystemQuad::create("Particles/Stars1.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Particles/Stars2.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Particles/Stars3.plist"));
    
    //the accelerometer stuff
    Device::setAccelerometerEnabled(true);
    auto accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);
    
    //do the asteroid stuff
#define KNUMASTEROIDS 15
    _asteroids = new Vector<Sprite*>(KNUMASTEROIDS);
    for(int i = 0; i < KNUMASTEROIDS; i++){
        auto * asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(true);
        _batchNode->addChild(asteroid);
        _asteroids->pushBack(asteroid);
    }
    
#define KNUMBLASTERS 5
    _shipLasers = new Vector<Sprite *>(KNUMBLASTERS);
    for(int i = 0; i < KNUMBLASTERS; i++){
        auto shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");
        shipLaser->setVisible(false);
        _batchNode->addChild(shipLaser);
        _shipLasers->pushBack(shipLaser);
    }//end for
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    _lives = 3;
    double curTime = getTimeTick();
    _gameOverTime = curTime + 300000;
    
    //get all the sound files
    SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/SpaceGame.wav", true);
    SimpleAudioEngine::getInstance()->preloadEffect("Sounds/explosion_large.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("Sounds/laser_ship.wav");
    
    return true;
}//end init function

void HelloWorld::update(float dt){
    Point backgroundScrollVert = Point(-1000, 0);
    _backgroundNode->setPosition(_backgroundNode->getPosition() + backgroundScrollVert * dt);
    
    // moving the spacedust
    auto spaceDusts = new Vector<Sprite *> (2);
    spaceDusts->pushBack(_spaceDust1);
    spaceDusts->pushBack(_spaceDust2);
    for (auto spaceDust : *spaceDusts) {
        float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        float size = spaceDust->getContentSize().width;
        if(xPosition < -size/2) _backgroundNode->incrementOffset(Point(spaceDust->getContentSize().width * 2.0, 0), spaceDust);
    }
    
    //moving the backgrounds
    auto backgrounds = new Vector<Sprite *>(4);
    backgrounds->pushBack(_galaxy);
    backgrounds->pushBack(_planetSunrise);
    backgrounds->pushBack(_spacialAnomaly);
    backgrounds->pushBack(_spaceialAnomaly2);
    for(auto background : * backgrounds){
        float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
        float size = background->getContentSize().width;
        if(xPosition < -size) _backgroundNode->incrementOffset(Point(2000, 0), background);
    }//end for loop
    
    //do the acceleration stuff
    Size winSize = Director::getInstance()->getWinSize();
    float maxY = winSize.height - _ship->getContentSize().height/2;
    float minY = _ship->getContentSize().height/2;
    float diff = (_shipPointsPerSecY * dt);
    float newY = _ship->getPosition().y + diff;
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(Vec2(_ship->getPosition().x, newY));
    
    //move the asteroids
    unsigned long curTimeMills = getTimeTick();
    if(curTimeMills > _nextAsteroidSpawn){
       // log("inside the if statement");
        unsigned long randomMills = randomValueBetween(0.2F, 1.0F) * 1000;
        _nextAsteroidSpawn = randomMills + curTimeMills;
      //  printf("randomMills: %f", randomMills);
        float randY = randomValueBetween(0.0F, winSize.height);
        float randDuration = randomValueBetween(2.0F, 10.0F);
        
        Sprite * asteroid = _asteroids->at(_nextAsteroid);
        _nextAsteroid++;
        if(_nextAsteroid >= _asteroids->size())
            _nextAsteroid=0;
        
        asteroid->stopAllActions();
        asteroid->setPosition(Vec2(winSize.width + asteroid->getContentSize().width/2, randY));
        asteroid->setVisible(true);
        asteroid->runAction(
                Sequence::create(MoveBy::create(randDuration, Vec2(-winSize.width-asteroid->getContentSize().width, 0)),
                        CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                        NULL)
                        );
    }//end if
    
    //handle collision detection
    for (auto asteroid : * _asteroids) {
        if(!(asteroid->isVisible())) continue;
        
        for(auto shipLaser : *_shipLasers){
            if(!(shipLaser->isVisible())) continue;
            
            if(shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())){
                shipLaser->setVisible(false);
                asteroid->setVisible(false);
                SimpleAudioEngine::getInstance()->playEffect("Sounds/explosion_large.wav");
                continue;
            }//end if the laser hit the asteroid
        }//end for
        
        //if ship hits the asteroid
        if(_ship->getBoundingBox().intersectsRect(asteroid->getBoundingBox())){
            asteroid->setVisible(false);
            _ship->runAction(Blink::create(1.0, 9));
            _lives--;
        }//end if
    }//end for
    
    //handle win or lose
    if(_lives <= 0){
        _ship->stopAllActions();
        _ship->setVisible(false);
        this->endScene(KENDREASONLOSE);
    }else if(curTimeMills >= _gameOverTime){
        this->endScene(KENDREASONWIN);
    }
    
    
    
}//end update function

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event){
    SimpleAudioEngine::getInstance()->playEffect("Sounds/laser_ship.wav");
    Size winSize = Director::getInstance()->getWinSize();
    Sprite *shipLaser = (Sprite *)_shipLasers->at(_nextShipLaser++);
    if(_nextShipLaser >= _shipLasers->size())
        _nextShipLaser = 0;
    
    shipLaser->setPosition(_ship->getPosition() + Point(shipLaser->getContentSize().width/2, 0));
    shipLaser->setVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(Sequence::create(MoveBy::create(0.5, Point(winSize.width, 0)),
                        CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                        NULL));
    //_lives = 3;
}

void HelloWorld::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)
#define KMAXDIFF 0.2
    
    // Cocos2DX inverts X and Y accelerometer depending on device orientation
    // in landscape mode right x=-y and y=x !!! (Strange and confusing choice
    double rollingX;

    acc->x = acc->y; //doing this because cocos2dx inverts x and y accelerometer
    rollingX = (acc->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
    float accelX = acc->x - rollingX;
    Size winSize = Director::getInstance()->getWinSize();
    float accelDiff = accelX - KRESTACCELX;
    float accelerationFraction = accelDiff/KMAXDIFF;
    _shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelerationFraction;
}

float HelloWorld::randomValueBetween(float low, float high){
    return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
    //return (((float)arc4random()/0xFFFFFFFFu) * (high - low)) + low;
}
unsigned long HelloWorld::getTimeTick(){
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return millisecs;
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
void HelloWorld::setInvisible(Node * node){
    node->setVisible(false);
}

void HelloWorld::endScene(EndReason endReason){
    if(_gameOver) return;
    _gameOver = true;
    
    //could make this auto
    auto winSize = Director::getInstance()->getWinSize();
    char message[10] = "You Win";
    if(endReason == KENDREASONLOSE) strcpy(message, "You Lose");
    
    auto label = Label::createWithBMFont("fonts/Arial.fnt", message);
    label->setScale(0.1F);
    label->setPosition(winSize.width /2, winSize.height *0.6F);
    this->addChild(label);
    
    strcpy(message, "Restart");
    auto restartLabel = Label::createWithBMFont("fonts/Arial.fnt", message);
    auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(HelloWorld::restartTapped, this));
    restartItem->setScale(0.1F);
    restartItem->setPosition(winSize.width/2, winSize.height *0.4);

    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    //clear label and menu
    restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
    label->runAction(ScaleTo::create(0.5F, 1.0F));
    
    //terminal update callback
    this->unscheduleUpdate();
    
}//end function endScene


void HelloWorld::restartTapped(Ref * pSender){
    Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));
    this->scheduleUpdate();
}


