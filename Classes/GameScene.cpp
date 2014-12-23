#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto pauseItem = MenuItemImage::create(
        "GameScreen/Pause_Button.png", "GameScreen/Pause_Button(Click).png",
        CC_CALLBACK_1(GameScreen::GoToPauseScene, this));

    pauseItem->setPosition(
        Point(pauseItem->getContentSize().width - (pauseItem->getContentSize().width / 4) + origin.x,
              visibleSize.height - pauseItem->getContentSize().height + (pauseItem->getContentSize().width / 4) + origin.y));

    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    for (int i = 0; i < 2; i++) {
        backgroundSpriteArray[i] = Sprite::create("GameScreen/Game_Screen_Background.png");
        backgroundSpriteArray[i]->setPosition(
            Point((visibleSize.width / 2) + origin.x,
                  (visibleSize.height / 2) + origin.y));
        this->addChild(backgroundSpriteArray[i], -2);
    }

    this->scheduleUpdate();
    this->schedule(schedule_selector(GameScreen::spawnAsteroid), 1.0);

    playerSprite = Sprite::create("GameScreen/Space_Pod.png");
    playerSprite->setPosition(Point(
        (visibleSize.width / 2),
        (pauseItem->getPositionY() - (pauseItem->getContentSize().height / 2) - (playerSprite->getContentSize().height / 2))));
    this->addChild(playerSprite, -1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(GameScreen::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScreen::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScreen::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScreen::onTouchCancelled, this);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    this->isTouching = false;
    this->touchPosition = 0;

    return true;
}

void GameScreen::GoToPauseScene(cocos2d::Ref* pSender)
{
    auto scene = PauseMenu::createScene();

    Director::getInstance()->pushScene(scene);
}

void GameScreen::GoToGameOverScene(cocos2d::Ref* pSender)
{
    auto scene = GameOver::createScene();

    Director::getInstance()->replaceScene(scene);
}

void GameScreen::update(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    for (int i = 0; i < 2; i++) {
        backgroundSpriteArray[i]->setPosition(
            Point(backgroundSpriteArray[i]->getPositionX(),
                  backgroundSpriteArray[i]->getPositionY() + (0.75 * visibleSize.height * dt)));
    }

    for (int i = 0; i < 2; i++) {
        if (backgroundSpriteArray[i]->getPosition().y >= visibleSize.height + (visibleSize.height / 2) - 1) {
            backgroundSpriteArray[i]->setPosition(
                Point((visibleSize.width / 2) + origin.x,
                      (-1 * visibleSize.height) + (visibleSize.height / 2)));
        }
    }

    for (int i = 0; i < asteroids.size(); i++) {
        asteroids[i]->setPosition(
            Point(asteroids[i]->getPositionX(),
                  asteroids[i]->getPositionY() + (0.75 * visibleSize.height * dt)));

        if (asteroids[i]->getPosition().y > visibleSize.height + (asteroids[i]->getContentSize().height / 2)) {
            this->removeChild(asteroids[i]);
            asteroids.erase(asteroids.begin() + i);
        }
    }

    if (isTouching) {
        if (touchPosition < visibleSize.width / 2) {
            //move the space pod to the left
            playerSprite->setPositionX(playerSprite->getPositionX() - (0.50 * visibleSize.width * dt));

            //check to prevent the space pod from going off the screen (left side)
            if (playerSprite->getPositionX() <= 0 + (playerSprite->getContentSize().width / 2)) {
                playerSprite->setPositionX(playerSprite->getContentSize().width / 2);
            }
        }
        else {
            // move the space pod to the right
            playerSprite->setPositionX(playerSprite->getPositionX() + (0.50 * visibleSize.width * dt));
             //check to prevent the space pod from going off the screen (right side)
            if (playerSprite->getPositionX() >= visibleSize.width - (playerSprite->getContentSize().width / 2)) {
               playerSprite->setPositionX(visibleSize.width  - (playerSprite->getContentSize().width / 2));
            }
        }
    }
}

void GameScreen::spawnAsteroid(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    int asteroidIndex = (arc4random() % 3) + 1;
    __String* asteroidString = __String::createWithFormat("GameScreen/Asteroid_%i.png", asteroidIndex);

    Sprite* tempAsteroid = Sprite::create(asteroidString->getCString());

    int xRandomPosition = (arc4random() % (int)(visibleSize.width - (tempAsteroid->getContentSize().width / 2))) + (tempAsteroid->getContentSize().width / 2);

    tempAsteroid->setPosition(
        Point(xRandomPosition + origin.x,
              -tempAsteroid->getContentSize().height + origin.y));

    asteroids.push_back(tempAsteroid);

    this->addChild(asteroids[asteroids.size() - 1], -1);
}

bool GameScreen::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    isTouching = true;
    touchPosition = touch->getLocation().x;
    return true;
}
void GameScreen::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void GameScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    isTouching = false;
}

void GameScreen::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    onTouchEnded(touch, event);
}
