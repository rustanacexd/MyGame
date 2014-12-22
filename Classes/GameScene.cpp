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
        if (backgroundSpriteArray[i]->getPositionY() >= visibleSize.height + (visibleSize.height / 2) - 1) {
            backgroundSpriteArray[i]->setPosition(
                Point((visibleSize.width / 2) + origin.x,
                      (-1 * visibleSize.height) + (visibleSize.height / 2)));
        }
    }

    for (int i = 0; i < asteroids.size(); i++) {
        asteroids[i]->setPosition(
            Point(asteroids[i]->getPositionX(),
                  asteroids[i]->getPositionY() + (0.75 * visibleSize.height * dt)));
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
