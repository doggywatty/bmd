#include "GameClasses.h"
#include "Graphics.h"
#include "Image.h"
#include "WidgetManager.h"
#include <cstdio>

using namespace Sexy;

// ResourceManager access
ResourceManager* Board::GetResourceManager()
{
    if (WidgetContainer::mWidgetManager != NULL && WidgetContainer::mWidgetManager->mApp != NULL) {
        return WidgetContainer::mWidgetManager->mApp->mResourceManager;
    }
    return NULL;
}

SexyAppBase* Board::GetApp()
{
    if (WidgetContainer::mWidgetManager != NULL && WidgetContainer::mWidgetManager->mApp != NULL) {
        return WidgetContainer::mWidgetManager->mApp;
    }
    return NULL;
}

Piece::Piece() 
{
    WidgetContainer::mDirty = true;
    Widget::mVisible = true;
    Widget::mWidth = 32;
    Widget::mHeight = 32;
    mType = 0;
    mColor = 0;
    mRow = 0;
    mCol = 0;
    mCollected = false;
    mAnimating = false;
    mAnimFrame = 0;
    mValue = 10;
}
Piece::~Piece() { }

Board::Board()
{
    WidgetContainer::mDirty = true;
    Widget::mVisible = true;
    Widget::mX = 0;
    Widget::mY = 0;
    Widget::mWidth = 640;
    Widget::mHeight = 480;

    mBackdropImage = NULL;
    mBagImage = NULL;
    mBagHolderImage = NULL;
    mCoinImage = NULL;
    mCoinsImage = NULL;
    mCoinglowImage = NULL;
    mRightBarImage = NULL;
    mLeftBarImage = NULL;

    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            mPieces[r][c] = NULL;
            mPieceGrid[r][c] = 0;
        }
    }

    mBoardX = 80;
    mBoardY = 30;
    mCellWidth = 60;
    mCellHeight = 60;

    mGameState = 0;
    mTickCount = 0;
    mSpawnCounter = 0;
    mSpawnDelay = 30;
    mSpawnRate = 300;
    mMaxPieces = 40;
    mPieceCount = 0;
    mBagsCollected = 0;
    mBagsTarget = 10;
    mScoreCurrent = 0;
    mScoreTarget = 1000;
    mScoreFrac = 0;
    mScoreTargetFrac = 0;
    mSpawnX = 0;
    mSpawnNext = 0;
    mPauseFlag = 0;
    mCoinSpawnDelay = 0;
    mLastSound = 0;
    mHoverState = 0;
    mAnimCounter = 0;
    mFadeCounter = 0;
    mFadeValue = 0;
    mCountdown = 0;
    mCountdownFrac = 0;
    mCountdownActive = 0;
    mAnimTimer = 0;
    mAnimTarget = 300;
    mTotalTime = 0;
    mLastTick = 0;
    mPaused = 0;
    mGameOver = 0;
    mTimer2 = 0;
    mTimer3 = 0;
    mTimer2Frac = 0;
    mVictoryTimer = 0;
    mMessageTimer = 0;
    mParticleAnim = 0;
    mParticleFlag = 0;
    mParticleFlag2 = 0;
    mParticleX = 0;
    mParticleY = 0;
    mFadeOut = 0;
    mFadeTimer = 0;
    mFinalTimer = 0;
    mFinalTime = 0.0;
    mFinalTimeFrac = 0;
    mFlashCounter = 0;
    mFlashIndex = 0;
    mAnimFlag = 0;
    mAnimFlag2 = 0;
    mAnimX = 0;
    mAnimY = 0;
    mAnimZ = 0;
    mAnimW = 0;
    mAnimAngle = 0;
    mAnimAngleFrac = 0;
    mAnimYFrac = 0;
    mAnimWFrac = 0;
    mComboTextLength = 0;
    mComboTimer = 0;
    mComboMultiplier = 1;
    mComboTextIndex = 0;
    mTimerActive = false;
    mTimerCount = 0;
    mTimerValue = 0.0;
    mTimerFrac = 0;
    mCounter = 0;
    mSpawnActive = 0;
    mSpawnActiveFlag = 0;
    mSpawnTimer = 0;
    mSpawnTimerFrac = 0;

    // load images via SexyAppBase::GetImage (not ResourceManager::LoadImage which crashes?????)
    SexyAppBase* app = GetApp();
    if (app != NULL)
    {
        mBackdropImage = (Image*)app->GetImage("images\\backdrop.jpg");
        mBagImage = (Image*)app->GetImage("images\\bag.gif");
        mBagHolderImage = (Image*)app->GetImage("images\\bagholder.gif");
        mCoinImage = (Image*)app->GetImage("images\\coin.gif");
        mCoinsImage = (Image*)app->GetImage("images\\coins.gif");
        mCoinglowImage = (Image*)app->GetImage("images\\coinglow.gif");
        mRightBarImage = (Image*)app->GetImage("images\\rightbar2.jpg");
        mLeftBarImage = (Image*)app->GetImage("images\\leftbar.jpg");
    }
}

Board::~Board()
{
    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            delete mPieces[r][c];
            mPieces[r][c] = NULL;
        }
    }
}

void Board::Update()
{
    if (mGameState == 8)
        return;

    mTickCount++;

    if (mTimerCount < 1)
    {
        if (mTimerValue > 0.0)
        {
            mTimerValue -= 0.016;
            if (mTimerValue < 0.0)
            {
                mTimerValue = 0.0;
                mTimerFrac = 0;
            }
        }
    }
    else
    {
        if (mTimerValue < 3.0)
        {
            mTimerValue += 0.016;
            if (mTimerValue > 3.0)
            {
                mTimerValue = 0.0;
                mTimerFrac = 0x3ff00000;
            }
        }
    }

    if (mTimerValue > 0.0)
    {
        mCounter++;
        uint mask = (mCounter + 1) & 0x80000007;
        bool zero = (mask == 0);
        if ((int)mask < 0)
            zero = ((mask - 1) | 0xfffffff8) == 0xffffffff;
        if (zero)
            WidgetContainer::mDirty = true;
    }

    if (mGameState != 0)
    {
        if (mGameState == 2)
        {
            uint u = mTickCount & 0x80000001;
            bool zero = (u == 0);
            if ((int)u < 0)
                zero = ((u - 1) | 0xfffffffe) == 0xffffffff;
            if (zero)
                mSpawnCounter++;
            WidgetContainer::mDirty = true;
            if (mSpawnDelay < 1)
            {
                int col = rand() % 8;
                int row = 0;
                SpawnPiece(col, row, 0);
            }
            else
                mSpawnDelay--;
        }
        else if (mGameState == 4)
        {
            if (!mPaused && !mGameOver)
            {
                if (mTimer2 > 0)
                {
                    mTimer2 -= 1;
                    WidgetContainer::mDirty = true;
                }
                if (mTimer3 > 0)
                {
                    mTimer3 -= 1;
                    WidgetContainer::mDirty = true;
                }
                if (mTimer2 == 0 && mTimer2Frac == 0x40300000)
                {
                    mGameState = 1;
                    DWORD tick = GetTickCount();
                    mTotalTime += (tick - mLastTick) / 1000;
                }
            }
        }
        else if (mGameState == 3)
        {
            uint u = mTickCount & 0x8000000f;
            bool zero = (u == 0);
            if ((int)u < 0)
                zero = ((u - 1) | 0xfffffff0) == 0xffffffff;
            if (zero)
                UpdatePieces();
        }
        else if (mGameState == 1)
        {
            if (mCountdown < 3.0)
            {
                mCountdownActive = 1;
                mCountdown += 1;
                if (mCountdown > 3.0)
                {
                    mCountdown = 0;
                    mCountdownFrac = 0x3ff00000;
                }
                WidgetContainer::mDirty = true;
            }
        }
        else if (mGameState == 6)
        {
            if (mAnimCounter < 500)
            {
                mAnimCounter += 5;
                WidgetContainer::mDirty = true;
            }
            if (mFadeCounter < 1)
            {
                if (-0x92 < mFadeValue)
                    mFadeValue -= 2;
                WidgetContainer::mDirty = true;
            }
            else
                mFadeCounter--;
        }
    }

    if ((mBagsCollected > 0) && (rand() % 15000 == 0)) { }

    int randomState = rand() % 1000;
    if ((randomState < 4) && (mTickCount % 12 == 0))
    {
        bool was = (mHoverState == 0);
        mHoverState = was ? 1 : 0;
        WidgetContainer::mDirty = true;
    }

    if (mCoinSpawnDelay > 0)
        mCoinSpawnDelay--;

    if ((mTickCount % 3 == 0) && (mCoinSpawnDelay == 0))
        mSpawnCounter++;

    if (mScoreCurrent < mScoreTarget)
    {
        mScoreCurrent += 1;
        if (mScoreTarget < mScoreCurrent)
            mScoreCurrent = mScoreTarget;
        WidgetContainer::mDirty = true;
    }

    if (((mGameState == 4) || (mGameState == 1)) && (mAnimTimer < mAnimTarget))
    {
        mAnimTimer += 1;
        WidgetContainer::mDirty = true;
    }

    if ((mAnimFlag != 0) && (mAnimFlag2 != 0))
    {
        mAnimX += 1;
        mAnimY = mAnimX + mAnimY;
        if (10.0 < mAnimX + mAnimY)
        {
            mAnimX = (int)(mAnimX * 0.9);
            mAnimY = 0;
            mAnimYFrac = 0x40540000;
            mAnimX = (int)mAnimX;
            if (1.0 < mAnimX)
            {
                mAnimX = 0;
                mAnimYFrac = 0;
            }
        }
        mAnimZ += 1;
        mAnimW = mAnimZ + mAnimW;
        if (10.0 < mAnimZ + mAnimW)
        {
            mAnimZ = (int)(mAnimZ * 0.9);
            mAnimW = 0;
            mAnimWFrac = 0x40540000;
            mAnimZ = (int)mAnimZ;
            if (1.0 < mAnimZ)
            {
                mAnimZ = 0;
                mAnimWFrac = 0;
            }
        }
        mAnimAngle = (mAnimW + mAnimY) * 1;
        if ((mAnimX == 0) && (mAnimZ == 0))
        {
            mAnimFlag2 = 0;
            mAnimAngle = 0;
            mAnimAngleFrac = 0;
        }
        WidgetContainer::mDirty = true;
    }

    if ((mGameState == 5) || (mGameState == 6))
    {
        if (mCountdown < 3.0)
        {
            mCountdownActive = 1;
            mCountdown += 1;
            if (mCountdown > 3.0)
            {
                mCountdown = 0;
                mCountdownFrac = 0x3ff00000;
            }
            WidgetContainer::mDirty = true;
        }
        if (mVictoryTimer > 0)
            mVictoryTimer--;
        if ((mTickCount % 3 == 0) && (mVictoryTimer == 0))
            mSpawnCounter++;
    }

    if (mMessageTimer > 0)
    {
        mMessageTimer--;
        WidgetContainer::mDirty = true;
    }

    if (mParticleAnim != 0)
    {
        mParticleAnim--;
        if (mParticleFlag != 0)
        {
            mParticleX = rand() % Widget::mWidth;
            mParticleY = rand() % Widget::mHeight;
        }
        if (mParticleFlag2 != 0)
        {
            int uVar = rand();
            int iVar15 = uVar;
            if (0xa0 < mParticleY)
                mParticleY = (mParticleY - (mParticleY - iVar15) / 3) - 1;
            if (mParticleY < iVar15)
                mParticleY = iVar15;
        }
    }

    if (mFadeOut != 0)
    {
        mFadeTimer -= 0.1;
        if (mFadeTimer < 0.0)
            mFadeOut = 0;
        WidgetContainer::mDirty = true;
    }

    if ((mBagsCollected < mBagsTarget) && (mGameState == 0)) { }

    if ((mFinalTimer > 0) || (mFinalTime != 0.0))
    {
        double dVar1 = mFinalTime + 0.016;
        mFinalTime = dVar1;
        if (0.0 < dVar1)
        {
            mFinalTime = 0;
            mFinalTimeFrac = 0;
            mFinalTimer--;
        }
    }

    WidgetContainer::mDirty = true;
}

void Board::SpawnPiece(int col, int row, int type)
{
    if (mPieces[row][col] != NULL)
        return;
    if (mPieceCount >= mMaxPieces)
        return;

    Piece* piece = new Piece();
    piece->mType = type;
    piece->mRow = row;
    piece->mCol = col;
    piece->mX = mBoardX + col * mCellWidth + mCellWidth / 2 - 16;
    piece->mY = mBoardY + row * mCellHeight + mCellHeight / 2 - 16;
    piece->mWidth = 32;
    piece->mHeight = 32;
    piece->mValue = 10;

    mPieces[row][col] = piece;
    mPieceGrid[row][col] = type;
    mPieceCount++;

    WidgetContainer::mWidgets.push_back(piece);
}

void Board::UpdatePieces()
{
    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* p = mPieces[r][c];
            if (p != NULL && !p->mCollected)
            {
                p->mAnimFrame++;
                if (p->mAnimFrame > 60)
                    p->mAnimFrame = 0;
                WidgetContainer::mDirty = true;
            }
        }
    }
}

void Board::CollectPiece(int row, int col)
{
    if (row < 0 || row >= 6 || col < 0 || col >= 8)
        return;
    Piece* p = mPieces[row][col];
    if (p == NULL || p->mCollected)
        return;

    p->mCollected = true;
    mPieceCount--;
    mBagsCollected++;
    mScoreCurrent += p->mValue * mComboMultiplier;
    mScoreTarget += 100;

    mPieceGrid[row][col] = 0;
    mPieces[row][col] = NULL;

    mParticleAnim = 30;
    mParticleX = p->mX;
    mParticleY = p->mY;

    WidgetContainer::mDirty = true;

    if (mBagsCollected >= mBagsTarget)
    {
        mGameState = 4;
        mVictoryTimer = 180;
    }
}

void Board::Draw(Graphics* g)
{
    if (mBackdropImage != NULL)
        g->DrawImage(mBackdropImage, 0, 0, Widget::mWidth, Widget::mHeight);
    else
    {
        g->SetColor(Color(30, 100, 30));
        g->FillRect(0, 0, Widget::mWidth, Widget::mHeight);
    }

    if (mLeftBarImage != NULL)
        g->DrawImage(mLeftBarImage, 0, mBoardY, 80, 450);
    if (mRightBarImage != NULL)
        g->DrawImage(mRightBarImage, Widget::mWidth - 80, mBoardY, 80, 450);

    g->SetColor(Color(50, 120, 50));
    for (int col = 0; col <= 8; col++)
    {
        int x = mBoardX + col * mCellWidth;
        g->DrawLine(x, mBoardY, x, mBoardY + 6 * mCellHeight);
    }
    for (int row = 0; row <= 6; row++)
    {
        int y = mBoardY + row * mCellHeight;
        g->DrawLine(mBoardX, y, mBoardX + 8 * mCellWidth, y);
    }

    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* p = mPieces[r][c];
            if (p != NULL && !p->mCollected)
            {
                int px = p->mX;
                int py = p->mY;

                if (mCoinglowImage != NULL)
                    g->DrawImage(mCoinglowImage, px - 8, py - 8, 48, 48);

                if (p->mType == 0)
                {
                    if (mCoinImage != NULL)
                        g->DrawImage(mCoinImage, px, py, 32, 32);
                    else
                    {
                        g->SetColor(Color(255, 215, 0));
                        g->FillRect(px - 14, py - 14, 28, 28);
                    }
                }
                else if (p->mType == 1)
                {
                    if (mBagImage != NULL)
                        g->DrawImage(mBagImage, px, py, 32, 32);
                    else
                    {
                        g->SetColor(Color(139, 69, 19));
                        g->FillRect(px - 14, py - 14, 28, 28);
                    }
                }

                if (mHoverState != 0)
                {
                    g->SetColor(Color(255, 255, 255, 100));
                    g->DrawRect(px - 16, py - 16, 32, 32);
                }
            }
        }
    }

    g->SetColor(Color(255, 255, 255));
    char scoreText[128];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", mScoreCurrent);
    g->DrawString(scoreText, 20, Widget::mHeight - 40);

    char bagText[128];
    snprintf(bagText, sizeof(bagText), "Bags: %d / %d", mBagsCollected, mBagsTarget);
    g->DrawString(bagText, 20, 10);

    char levelText[128];
    snprintf(levelText, sizeof(levelText), "Level: %d", (mGameState == 4) ? 2 : 1);
    g->DrawString(levelText, Widget::mWidth - 150, 10);

    if (mMessageTimer > 0)
    {
        g->SetColor(Color(255, 255, 255, 180));
        g->FillRect(100, Widget::mHeight / 2 - 30, Widget::mWidth - 200, 60);
        g->SetColor(Color(255, 255, 0));
        g->DrawString("COLLECT MONEY BAGS!", Widget::mWidth / 2 - 100, Widget::mHeight / 2);
    }

    if (mPaused)
    {
        g->SetColor(Color(0, 0, 0, 150));
        g->FillRect(0, 0, Widget::mWidth, Widget::mHeight);
        g->SetColor(Color(255, 255, 0));
        g->DrawString("GAME PAUSED", Widget::mWidth / 2 - 60, Widget::mHeight / 2);
        g->DrawString("CLICK HERE TO CONTINUE!", Widget::mWidth / 2 - 100, Widget::mHeight / 2 + 30);
    }

    if ((mGameState == 5) || (mGameState == 6))
    {
        g->SetColor(Color(255, 215, 0));
        if (mGameState == 6)
            g->DrawString("VICTORY! LEVEL COMPLETE!", Widget::mWidth / 2 - 100, Widget::mHeight / 2);
        else
            g->DrawString("GAME OVER", Widget::mWidth / 2 - 60, Widget::mHeight / 2);
        g->DrawString("CLICK HERE TO PLAY AGAIN", Widget::mWidth / 2 - 120, Widget::mHeight / 2 + 40);
    }
}

void Board::UpdateFrames()
{
    WidgetContainer::mDirty = false;
}

void Board::MouseMove(int x, int y)
{
    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* p = mPieces[r][c];
            if (p != NULL && !p->mCollected)
            {
                if (x >= p->mX && x < p->mX + p->mWidth && y >= p->mY && y < p->mY + p->mHeight)
                {
                    mHoverState = 1;
                    WidgetContainer::mDirty = true;
                    return;
                }
            }
        }
    }
    mHoverState = 0;
}

bool Board::Overlaps(int* theOther)
{
    return false;
}

void Board::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Widget::mX = theX;
    Widget::mY = theY;
    Widget::mWidth = theWidth;
    Widget::mHeight = theHeight;
    WidgetContainer::mDirty = true;
}

void Board::UpdateComplex()
{
    WidgetContainer::mDirty = false;
}

void Board::NotifyWidgets(int param_1)
{
    for (WidgetList::iterator it = WidgetContainer::mWidgets.begin(); it != WidgetContainer::mWidgets.end(); ++it)
    {
        Widget* w = *it;
        if (w != NULL)
            w->mDirty = true;
    }
}

void Board::Cleanup()
{

}

void Board::Method5(int param_1) { WidgetContainer::mDirty = true; }
void Board::Method6() { }
void Board::Method10(unsigned int *param_1) { WidgetContainer::mDirty = true; }
void Board::Method11() { }
void Board::Method12(unsigned int *param_1, int param_2) { WidgetContainer::mDirty = true; }
void Board::Method14(unsigned int *param_1, void *param_2) { WidgetContainer::mDirty = true; }
void Board::Method16(char param_1) { WidgetContainer::mDirty = true; }
void Board::Method17(int param_1, int param_2) { WidgetContainer::mDirty = true; }
void Board::Method18(int param_1, int param_2) { WidgetContainer::mDirty = true; }
void Board::Method19(int param_1, int *param_2) { WidgetContainer::mDirty = true; }
void Board::Method20(unsigned int *param_1, int param_2, unsigned int *param_3) { WidgetContainer::mDirty = true; }
void * Board::Method21(int param_1) { WidgetContainer::mDirty = true; return nullptr; }
void Board::Method22(char param_1) { WidgetContainer::mDirty = true; }
void Board::Method23(char param_1) { WidgetContainer::mDirty = true; }
void Board::Method24(unsigned int *param_1) { WidgetContainer::mDirty = true; }
void Board::Method25(int param_1, int param_2, int param_3, int param_4) { WidgetContainer::mDirty = true; }
void Board::Method26(int param_1, int param_2) { WidgetContainer::mDirty = true; }
void Board::Method27() { }
void Board::Method28() { }
void Board::Method29() { WidgetContainer::mDirty = true; }
void Board::Method30() { WidgetContainer::mDirty = true; }
void Board::Method31() { }
void Board::Method32(int param_1) { if (param_1 == 9) return; return; }
void Board::Method33() { }
void Board::Method34() { }
void Board::Method35(int *param_1) { WidgetContainer::mDirty = true; }
void Board::Method36(int param_1, int param_2)
{
    *(int *)((int)this + 0x4b0) = param_1;
    *(int *)((int)this + 0x4b4) = param_2;
}
void Board::Method37() { }
void Board::Method38(int param_1, int param_2, int param_3) { WidgetContainer::mDirty = true; }
void Board::Method39() { }
void Board::Method40(unsigned int param_1, unsigned int param_2, int param_3) { return; }
void Board::Method41() { }
void Board::Method42() { }
void Board::Method43() { }
void Board::Method44() { }
void Board::Method45(int *param_1, void *param_2, int param_3, unsigned int param_4)
{
    *param_1 = *param_1 + (*(int *)((int)this + 0x34) - *(int *)((int)this + 0x2c)) / 2;
    param_1[1] = param_3;
}
void Board::Method46(int *param_1, void *param_2, int param_3, unsigned int param_4)
{
    int iVar2 = 0;
    int iVar5 = (*(int *)((int)this + 0x34) - iVar2) / 2;
    *param_1 = iVar5;
    param_1[1] = param_3;
    param_1[2] = iVar2;
    param_1[3] = iVar5;
}
void Board::Method47(void *param_1, int param_2, int param_3, int param_4, int param_5, int param_6, char param_7, int param_8, int param_9)
{
    WidgetContainer::mDirty = true;
}
void Board::Method48(void *param_1, int *param_2, int param_3, int param_4, int param_5)
{
    WidgetContainer::mDirty = true;
}
void Board::Method49(void *param_1, unsigned int param_2, int param_3, int param_4)
{
    WidgetContainer::mDirty = true;
}
int Board::Method50(int param_1)
{
    int iVar2 = 10;
    int iVar1 = 1;
    if (9 < param_1)
    {
        do
        {
            iVar2 = iVar2 * 10;
            iVar1 = iVar1 + 1;
        }
        while (iVar2 <= param_1);
    }
    return iVar1;
}
void Board::Method51(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
    WidgetContainer::mDirty = true;
}
bool Board::Method52(int param_1, int param_2)
{
    if ((*(int *)((int)this + 0x2c) <= param_1) &&
        (param_1 < *(int *)((int)this + 0x34) + *(int *)((int)this + 0x2c))) {
        if ((*(int *)((int)this + 0x30) <= param_2) &&
            (param_2 < *(int *)((int)this + 0x38) + *(int *)((int)this + 0x30))) {
            return true;
        }
    }
    return false;
}
void Board::Method53(int *param_1)
{
    int iVar1 = *(int *)((int)this + 0x38);
    int iVar2 = *(int *)((int)this + 0x68);
    int iVar3 = *(int *)((int)this + 100);
    int iVar4 = *(int *)((int)this + 0x70);
    int iVar5 = *(int *)((int)this + 0x6c);
    int iVar6 = *(int *)((int)this + 0x34);
    int iVar7 = *(int *)((int)this + 0x2c);
    param_1[1] = *(int *)((int)this + 0x30) + iVar2;
    param_1[2] = (iVar6 - iVar5) - iVar3;
    *param_1 = iVar7 + iVar3;
    param_1[3] = (iVar1 - iVar4) - iVar2;
    return;
}

// MrMoney
MrMoney::MrMoney()
{
    WidgetContainer::mDirty = true;
    mMrMoneyImage = NULL;
    mMrMoneyBagsImage = NULL;

    SexyAppBase* app = NULL;
    if (WidgetContainer::mParent != NULL &&
        WidgetContainer::mParent->mWidgetManager != NULL &&
        WidgetContainer::mParent->mWidgetManager->mApp != NULL) {
        app = WidgetContainer::mParent->mWidgetManager->mApp;
    }
    if (app != NULL) {
        mMrMoneyImage = (Image*)app->GetImage("images\\mrmoney.gif");
        mMrMoneyBagsImage = (Image*)app->GetImage("images\\mrmoneybags.gif");
    }
}
MrMoney::~MrMoney() { }

Points::Points()
{
    WidgetContainer::mDirty = true;
}
Points::~Points() { }

GameSelector::GameSelector()
{
    WidgetContainer::mDirty = true;
    mBigMoneyLogo = NULL;
    mStartButtonImage = NULL;
}
GameSelector::~GameSelector() { }

TitleScreen::TitleScreen()
{
    WidgetContainer::mDirty = true;
    mTitleBackImage = NULL;
    mBigMoneyLogoImage = NULL;
    mStartTextImage = NULL;
    mStartArrowsImage = NULL;
    mAnimTime = 0;
    mShowStart = true;
}
TitleScreen::~TitleScreen() { }

void TitleScreen::Draw(Graphics* g)
{
    SexyAppBase* app = NULL;
    if (WidgetContainer::mParent != NULL && WidgetContainer::mParent->mWidgetManager != NULL && WidgetContainer::mParent->mWidgetManager->mApp != NULL)
        app = WidgetContainer::mParent->mWidgetManager->mApp;
    if (app != NULL)
    {
        if (mTitleBackImage == NULL)
            mTitleBackImage = (Image*)app->GetImage("images\\titleback.jpg");
        if (mBigMoneyLogoImage == NULL)
            mBigMoneyLogoImage = (Image*)app->GetImage("images\\bigmoneylogo.jpg");
        if (mStartTextImage == NULL)
            mStartTextImage = (Image*)app->GetImage("images\\_starttext.gif");
        if (mStartArrowsImage == NULL)
            mStartArrowsImage = (Image*)app->GetImage("images\\_startarrows.gif");
    }

    if (mTitleBackImage != NULL)
        g->DrawImage(mTitleBackImage, 0, 0, mWidth, mHeight);
    else
    {
        g->SetColor(Color(200, 50, 50));
        g->FillRect(0, 0, mWidth, mHeight);
    }

    if (mBigMoneyLogoImage != NULL && mWidth > 0 && mHeight > 0)
        g->DrawImage(mBigMoneyLogoImage, mWidth / 2 - 150, 50, 300, 150);

    if (mShowStart)
    {
        if (mStartTextImage != NULL && mWidth > 0 && mHeight > 0)
            g->DrawImage(mStartTextImage, mWidth / 2 - 100, mHeight - 100, 200, 50);
        if (mStartArrowsImage != NULL && mWidth > 0 && mHeight > 0)
            g->DrawImage(mStartArrowsImage, mWidth / 2 - 50, mHeight - 40, 100, 30);
    }
}

void TitleScreen::Update()
{
    mAnimTime++;
    if (mAnimTime > 60)
    {
        mShowStart = !mShowStart;
        mAnimTime = 0;
    }
    WidgetContainer::mDirty = true;
}

void TitleScreen::UpdateFrames()
{
    WidgetContainer::mDirty = false;
}

StartButton::StartButton() : ButtonWidget(0, NULL)
{
    Widget::mDirty = true;
}
StartButton::~StartButton() { }

MoneyDialog::MoneyDialog() : Dialog(NULL, NULL, 0, true, "", "", "", Dialog::BUTTONS_NONE)
{
    Widget::mDirty = true;
}
MoneyDialog::~MoneyDialog() { }

void MoneyDialog::ButtonPress(int theId)
{
    mDirty = true;
}

HighScoreDialog::HighScoreDialog() : MoneyDialog() { }
HighScoreDialog::~HighScoreDialog() { }

void HighScoreDialog::ButtonPress(int theId)
{
    mDirty = true;
}

HighScoreWidget::HighScoreWidget()
{
    WidgetContainer::mDirty = true;
}
HighScoreWidget::~HighScoreWidget() { }

void HighScoreWidget::ButtonPress(int theId)
{
    mDirty = true;
}

HelpWidget::HelpWidget()
{
    WidgetContainer::mDirty = true;
}
HelpWidget::~HelpWidget() { }

void HelpWidget::ButtonPress(int theId)
{
    mDirty = true;
}

OverlayWidget::OverlayWidget()
{
    WidgetContainer::mDirty = true;
}
OverlayWidget::~OverlayWidget() { }

LockableButton::LockableButton() : ButtonWidget(0, NULL)
{
    Widget::mDirty = true;
}
LockableButton::~LockableButton() { }

OptionsDialog::OptionsDialog() : MoneyDialog(), SliderListener(), CheckboxListener()
{
    Widget::mDirty = true;
}
OptionsDialog::~OptionsDialog() { }

void OptionsDialog::SliderVal(int theId, double theVal)
{
    mDirty = true;
}

void OptionsDialog::CheckboxChecked(int theId, bool checked)
{
    mDirty = true;
}

void OptionsDialog::ButtonPress(int theId)
{
    mDirty = true;
}

RecordScoreDialog::RecordScoreDialog() : MoneyDialog(), ButtonListener(), EditListener()
{
    Widget::mDirty = true;
}
RecordScoreDialog::~RecordScoreDialog() { }

void RecordScoreDialog::ButtonPress(int theId)
{
    mDirty = true;
}

void RecordScoreDialog::EditWidgetText(int theId, const std::string& theString)
{
    mDirty = true;
}

RegisterDialog::RegisterDialog() : Dialog(NULL, NULL, 0, true, "", "", "", Dialog::BUTTONS_NONE), ButtonListener(), EditListener()
{
    Widget::mDirty = true;
}
RegisterDialog::~RegisterDialog() { }

void RegisterDialog::ButtonPress(int theId)
{
    mDirty = true;
}

void RegisterDialog::EditWidgetText(int theId, const std::string& theString)
{
    mDirty = true;
}

CreditsDialog::CreditsDialog() : Dialog(NULL, NULL, 0, true, "", "", "", Dialog::BUTTONS_NONE), ButtonListener()
{
    Widget::mDirty = true;
}
CreditsDialog::~CreditsDialog() { }

void CreditsDialog::ButtonPress(int theId)
{
    mDirty = true;
}

UpdateCheckDialog::UpdateCheckDialog() : Dialog(NULL, NULL, 0, true, "", "", "", Dialog::BUTTONS_NONE), ButtonListener()
{
    Widget::mDirty = true;
}
UpdateCheckDialog::~UpdateCheckDialog() { }

void UpdateCheckDialog::ButtonPress(int theId)
{
    mDirty = true;
}