#include "WinBMApp.h"
#include "GameClasses.h"
#include <cstdio>
#include <windows.h>
#include "WidgetManager.h"
#include "DSoundManager.h"
#include "SoundManager.h"
#include "Font.h"
#include "ImageFont.h"
#include "MusicInterface.h"
#include "WidgetManager.h"

char DYNAMIC_DATA_BLOCK[400] = {0};
extern char DYNAMIC_DATA_BLOCK[400];

using namespace Sexy;

WinBMApp::WinBMApp()
{
    strncpy(DYNAMIC_DATA_BLOCK + 80, "00000003Mon Nov 14 11:49:25 2005", 80);
    strncpy(DYNAMIC_DATA_BLOCK + 160, "46107743", 8);
    mProdName = "BigMoney";
    mCompanyName = "PopCap";
    mRegKey = "PopCap\\BigMoneyDeluxe";
    mTitle = "Big Money Deluxe";
    mProductVersion = "1.3";
    mBuildNum = 3;
    mBuildDate = "Mon Nov 14 11:49:25 2005";

    for (int i = 0; i < 71; i++) {
        mImages[i] = 0;
    }
    mFont1 = mFont2 = mFont3 = mFont4 = mFont5 = nullptr;
    mFont6 = mFont7 = mFont8 = mFont9 = nullptr;
    mScoreFont = mLevelFont = mFloatFont = nullptr;
    mBoard = nullptr;
    mMrMoney = nullptr;
    mRegValid = false;
    mCobranded = false;
    mSkipSignatureChecks = true;
}

WinBMApp::~WinBMApp()
{
    
}

void WinBMApp::Init()
{
    SexyApp::Init();

    mBuildUnlocked = true;
    mSkipAd = false;
    mSkipSignatureChecks = true;
    mDontUpdate = false;
    mTimesPlayed = 0;
    mTimesExecuted = 0;
    mBuildNum = 3;
    mBuildUnlocked = true;

    Board* board = new Board();
    board->Resize(0, 30, mWidth, mHeight - 30);
    mWidgetManager->AddWidget(board);
    mBoard = board;
    mBoard->Widget::mVisible = true;

    if (mSoundManager == NULL)
        mSoundManager = new DSoundManager(mNoSoundNeeded ? NULL : mHWnd, false);

    mMusicInterface = CreateMusicInterface(mInvisHWnd);

    ::ShowWindow(mHWnd, SW_NORMAL);
    ::SetForegroundWindow(mHWnd);
}

void WinBMApp::UpdateFrames()
{
    SexyApp::UpdateFrames();
}

bool WinBMApp::Process(bool allowSleep)
{
    bool aResult = SexyApp::Process(allowSleep);
    return aResult;
}