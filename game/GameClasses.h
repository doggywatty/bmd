#ifndef __GAME_CLASSES_H__
#define __GAME_CLASSES_H__

#include "Widget.h"
#include "WidgetContainer.h"
#include "ButtonWidget.h"
#include "Dialog.h"
#include "Checkbox.h"
#include "Slider.h"
#include "EditWidget.h"
#include "EditListener.h"
#include "SliderListener.h"
#include "CheckboxListener.h"
#include <string>
#include "SexyApp.h"

namespace Sexy
{

class ResourceManager;

class Board; class Piece; class MrMoney; class Points;
class GameSelector; class TitleScreen; class StartButton;
class MoneyDialog; class HighScoreDialog; class HighScoreWidget;
class HelpWidget; class OverlayWidget; class LockableButton;
class OptionsDialog; class RecordScoreDialog; class RegisterDialog;
class CreditsDialog; class UpdateCheckDialog;

class Piece : public Widget {
public:
	Piece(); virtual ~Piece();
	int mType; // 0 = coin, 1 = bag, 2 = dollar bomb
	int mColor; // coin color index
	int mRow, mCol; // board position
	bool mCollected; // has been collected
	bool mAnimating; // is animating
	int mAnimFrame; // animation frame
	int mValue; // point value
};

class Board : public Widget, public ButtonListener {
public:
	Board(); virtual ~Board();
	virtual void Draw(Graphics* g);
	virtual void MouseMove(int x, int y);
	virtual bool Overlaps(int* theOther);
	void Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void UpdateComplex();
	virtual void UpdateFrames();
	virtual void Update();
	void NotifyWidgets(int param_1);
	void Cleanup();
	void Method5(int param_1);
	void Method6();
	void Method7();
	void Method8();
	void Method9();
	void Method10(unsigned int *param_1);
	void Method11();
	void Method12(unsigned int *param_1, int param_2);
	void Method13();
	void Method14(unsigned int *param_1, void *param_2);
	void Method15();
	void Method16(char param_1);
	void Method17(int param_1, int param_2);
	void Method18(int param_1, int param_2);
	void Method19(int param_1, int *param_2);
	void Method20(unsigned int *param_1, int param_2, unsigned int *param_3);
	void *Method21(int param_1);
	void Method22(char param_1);
	void Method23(char param_1);
	void Method24(unsigned int *param_1);
	void Method25(int param_1, int param_2, int param_3, int param_4);
	void Method26(int param_1, int param_2);
	void Method27();
	void Method28();
	void Method29();
	void Method30();
	void Method31();
	void Method32(int param_1);
	void Method33();
	void Method34();
	void Method35(int *param_1);
	void Method36(int param_1, int param_2);
	void Method37();
	void Method38(int param_1, int param_2, int param_3);
	void Method39();
	void Method40(unsigned int param_1, unsigned int param_2, int param_3);
	void Method41();
	void Method42();
	void Method43();
	void Method44();
	void Method45(int *param_1, void *param_2, int param_3, unsigned int param_4);
	void Method46(int *param_1, void *param_2, int param_3, unsigned int param_4);
	void Method47(void *param_1, int param_2, int param_3, int param_4, int param_5, int param_6, char param_7, int param_8, int param_9);
	void Method48(void *param_1, int *param_2, int param_3, int param_4, int param_5);
	void Method49(void *param_1, unsigned int param_2, int param_3, int param_4);
	int Method50(int param_1);
	void Method51(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6);
	bool Method52(int param_1, int param_2);
	void Method53(int *param_1);

	void SpawnPiece(int col, int row, int type);
	void UpdatePieces();
	void CollectPiece(int row, int col);

	// ResourceManager access
	ResourceManager* GetResourceManager();
	SexyAppBase* GetApp();

	// Game state variables (translated from 0x40c9f0 decompilation)
	int mGameState; // 0=idle, 1=countdown, 2=playing, 3=processing, 4=transition, 5=gameover, 6=victory, 7=paused, 8=exiting
	int mTickCount; // frame counter
	int mSpawnCounter; // coin spawn counter
	int mSpawnDelay; // spawn delay timer
	int mSpawnRate; // spawn rate
	int mMaxPieces; // max pieces on board
	int mPieceCount; // current piece count
	int mBagsCollected; // money bags collected
	int mBagsTarget; // bags needed to advance
	int mScoreCurrent; // current score
	int mScoreTarget; // target score
	int mScoreFrac; // score fraction
	int mScoreTargetFrac; // target score fraction
	int mSpawnX; // spawn X position
	int mSpawnNext; // next spawn value
	int mPauseFlag; // pause flag
	int mCoinSpawnDelay; // coin spawn delay
	int mLastSound; // last sound time
	int mHoverState; // hover state
	int mAnimCounter; // animation counter
	int mFadeCounter; // fade counter
	int mFadeValue; // fade value
	int mCountdown; // countdown timer
	int mCountdownFrac; // countdown fraction
	int mCountdownActive; // countdown active flag
	int mAnimTimer; // animation timer
	int mAnimTarget; // animation target
	int mTotalTime; // total time played
	int mLastTick; // last tick count
	int mPaused; // paused flag
	int mGameOver; // game over flag
	int mTimer2; // timer 2
	int mTimer3; // timer 3
	int mTimer2Frac; // timer 2 fraction
	int mVictoryTimer; // victory timer
	int mMessageTimer; // message timer
	int mParticleAnim; // particle animation flag
	int mParticleFlag; // particle flag
	int mParticleFlag2; // particle flag 2
	int mParticleX; // particle X
	int mParticleY; // particle Y
	int mFadeOut; // fade out flag
	int mFadeTimer; // fade timer
	int mFinalTimer; // final timer
	double mFinalTime; // final time
	int mFinalTimeFrac; // final time frac
	int mFlashCounter; // flash counter
	int mFlashIndex; // flash index
	int mAnimFlag; // anim flag
	int mAnimFlag2; // anim flag 2
	int mAnimX; // anim X
	int mAnimY; // anim Y
	int mAnimZ; // anim Z
	int mAnimW; // anim W
	int mAnimAngle; // anim angle
	int mAnimAngleFrac; // anim angle frac
	int mAnimYFrac; // anim Y fraction
	int mAnimWFrac; // anim W fraction
	int mComboTextLength; // combo text length
	int mComboTimer; // combo timer
	int mComboMultiplier; // combo multiplier
	int mComboTextIndex; // combo text index
	bool	mTimerActive; // timer active flag
	int mTimerCount; // timer count
	double mTimerValue; // timer value
	int mTimerFrac; // timer fraction
	int mCounter; // general counter
	int mSpawnActive; // spawn active flag
	int mSpawnActiveFlag; // spawn active flag 2
	int mSpawnTimer; // spawn timer
	int mSpawnTimerFrac; // spawn timer fraction

	Image* mBackdropImage; // backdrop.jpg
	Image* mBagImage; // bag.gif
	Image* mBagHolderImage; // bagholder.gif
	Image* mCoinImage; // coin.gif
	Image* mCoinsImage; // coins.gif
	Image* mCoinglowImage; // coinglow.gif
	Image* mRightBarImage; // rightbar2.jpg
	Image* mLeftBarImage; // leftbar.jpg

	Piece* mPieces[6][8];
	int mPieceGrid[6][8];

	int mBoardX;
	int mBoardY;
	int mCellWidth;
	int mCellHeight;
};

class MrMoney : public WidgetContainer {
public:
	MrMoney(); virtual ~MrMoney();
	Image* mMrMoneyImage;
	Image* mMrMoneyBagsImage;
};

class Points : public WidgetContainer {
public:
	Points(); virtual ~Points();
};

class GameSelector : public WidgetContainer, public ButtonListener {
public:
	GameSelector(); virtual ~GameSelector();
	Image* mBigMoneyLogo;
	Image* mStartButtonImage;
};

class TitleScreen : public WidgetContainer, public ButtonListener {
public:
	TitleScreen(); virtual ~TitleScreen();
	virtual void Draw(Graphics* g);
	virtual void Update();
	virtual void UpdateFrames();
	Image* mTitleBackImage;
	Image* mBigMoneyLogoImage;
	Image* mStartTextImage;
	Image* mStartArrowsImage;
	int	mAnimTime;
	bool mShowStart;
};

class StartButton : public ButtonWidget {
public:
	StartButton(); virtual ~StartButton();
};

class MoneyDialog : public Dialog {
public:
	MoneyDialog(); virtual ~MoneyDialog();
	virtual void ButtonPress(int theId);
};

class HighScoreDialog : public MoneyDialog {
public:
	HighScoreDialog(); virtual ~HighScoreDialog();
	virtual void ButtonPress(int theId);
};

class HighScoreWidget : public WidgetContainer, public ButtonListener {
public:
	HighScoreWidget(); virtual ~HighScoreWidget();
	virtual void ButtonPress(int theId);
};

class HelpWidget : public WidgetContainer, public ButtonListener {
public:
	HelpWidget(); virtual ~HelpWidget();
	virtual void ButtonPress(int theId);
};

class OverlayWidget : public WidgetContainer {
public:
	OverlayWidget(); virtual ~OverlayWidget();
};

class LockableButton : public ButtonWidget {
public:
	LockableButton(); virtual ~LockableButton();
};

class OptionsDialog : public MoneyDialog, public SliderListener, public CheckboxListener {
public:
	OptionsDialog(); virtual ~OptionsDialog();
	virtual void SliderVal(int theId, double theVal);
	virtual void CheckboxChecked(int theId, bool checked);
	virtual void ButtonPress(int theId);
};

class RecordScoreDialog : public MoneyDialog, public ButtonListener, public EditListener {
public:
	RecordScoreDialog(); virtual ~RecordScoreDialog();
	virtual void ButtonPress(int theId);
	virtual void EditWidgetText(int theId, const std::string& theString);
};

class RegisterDialog : public Dialog, public ButtonListener, public EditListener {
public:
	RegisterDialog(); virtual ~RegisterDialog();
	virtual void ButtonPress(int theId);
	virtual void EditWidgetText(int theId, const std::string& theString);
};

class CreditsDialog : public Dialog, public ButtonListener {
public:
	CreditsDialog(); virtual ~CreditsDialog();
	virtual void ButtonPress(int theId);
};

class UpdateCheckDialog : public Dialog, public ButtonListener {
public:
	UpdateCheckDialog(); virtual ~UpdateCheckDialog();
	virtual void ButtonPress(int theId);
};

}

#endif // __GAME_CLASSES_H__