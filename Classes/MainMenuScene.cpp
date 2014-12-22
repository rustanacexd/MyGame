#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto menuTitle = MenuItemImage::create("MainMenuScreen/Game_Title.png",
                                           "MainMenuScreen/Game_Title.png");
    auto playItem = MenuItemImage::create("MainMenuScreen/Play_Button.png",
                                          "MainMenuScreen/Play_Button(Click).png",
                                          CC_CALLBACK_1(MainMenu::GoToGameScene, this));
    
    auto menu = Menu::create(menuTitle, playItem, NULL);
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    this->addChild(menu);
    
    auto backgroundSprite = Sprite::create("MainMenuScreen/Background.png");
    backgroundSprite->setPosition(Point((visibleSize.width / 2 ) + origin.x, (visibleSize.height /2 ) + origin.y));
    this->addChild(backgroundSprite, -1);
    
    
    return true;
}

void MainMenu::GoToGameScene(cocos2d::Ref *pSender)
{
    auto scene = GameScreen::createScene();
    
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}