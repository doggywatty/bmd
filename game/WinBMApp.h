#ifndef __WINBMAPP_H__
#define __WINBMAPP_H__

#include "SexyApp.h"
#include "Font.h"
#include "WidgetContainer.h"

namespace Sexy
{

class Board;

class WinBMApp : public SexyApp
{
public:
    WinBMApp();
    virtual ~WinBMApp();

    virtual void    Init();
    virtual void    UpdateFrames();
    virtual bool    Process(bool allowSleep = true);

    // 0x41d9a0
    Font* mFont1; // 0x1bb | data\bigmoneyfont1.txt
    Font* mFont2; // 0x1bc | derived from mFont1
    Font* mFont3; // 0x1bd | bigmoneyfont2.txt noshadow
    Font* mFont4; // 0x1be | bigmoneyfont3.txt underlined
    Font* mFont5; // 0x1bf | bigmoneyfont3.txt outline
    Font* mFont6; // 0x1c0 | bigmoneyfont3.txt underlined
    Font* mFont7; // 0x1c1 | derived from mFont6
    Font* mFont8; // 0x1c2 | derived from mFont6
    Font* mFont9; // 0x1c3 | derived from mFont6
    Font* mScoreFont; // 0x1c4 | data\scorefont.txt
    Font* mLevelFont; // 0x1c5 | data\levelfont.txt
    Font* mFloatFont; // 0x1c6 | data\floatfont.txt

    Board* mBoard; // 0x1b6

    WidgetContainer* mMrMoney; // 0x1ba

    // FUN_0041d7d0
    int mImages[71]; // 0x1b9

    bool mRegValid; // 0x1af | set when registry key found

    bool mCobranded; // from "Cobranded" property check
};

}

#endif // __WINBMAPP_H__