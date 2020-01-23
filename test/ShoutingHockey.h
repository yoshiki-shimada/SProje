/*******************************************************************
* @file		SH.h
* @brief	Mainclass.h
* @author	yshimada
* @data		20191215
*******************************************************************/

#pragma once

#include "RemTask.h"

//! �X�N���[���T�C�Y
#define SCREEN_WIDTH	864
#define SCREEN_HEIGHT	864
//! �ꉞ�F
#define SCREEN_COLOR	32

//! �΂߈ړ��̂��߂̕ϊ��v�Z���l
#define PI        3.14159265f
#define PI_HARF	  PI * 0.5f
#define RadToDeg  57.29577951f
#define DegToRad  0.017453293f

//! �t�F�[�h���o�p
#define FADEBG "res\\BlacBox.png"
#define FADE_WH 36

//--------------------------------------------------------
//! �^�C�g��	864x864
#define TITLEBG	"res\\TitleBG.png"
#define TITLE_SIZE_X  864
#define TITLE_SIZE_Y  864

//--------------------------------------------------------

//! �Q�[���w�i	908x864
//! ���T�C�h�݂̂͂����摜�T�C�Y�@22
#define BGSTAGEFLOOR	"res\\GameBG1.png"
#define BGSTAGEWALL		"res\\Wall.png"
#define BGSTAGE1_SIZE_X  908
#define BGSTAGE1_SIZE_Y  864
#define RLWALL_AND_PLAYER 60
#define TBWALL_AND_PLAYER 55

//! �o���p�[	230x13
#define BUMPER	"res\\Bumper.png"
#define BUMPER_SIZE_X  230
#define BUMPER_SIZE_Y  13

//! �v���C���[
#define PLAYER_CHIP	"res\\Player.png"
#define PLAYER_PATTERN 4
#define PLAYER_SIZE 34
#define PLAYER_SIZE_HARF 17

enum {
	MOVE_DIR_NORMAL,
	MOVE_DIR_LV2,
	MOVE_DIR_LV3,
	MOVE_DIR_MAX
};

#define PORTAL "res\\Portal.png"
#define PORTAL_SIZE 105
#define PORTAL_SIZE_HARF 52.5

#define ZENEMY_CHIP	"res\\ZEnemy1.png"
#define ZENEMY_PATTERN 4
#define ZENEMY_CHIP_SIZE_X 58
#define ZENEMY_CHIP_SIZE_Y 58
#define ZENEMY_CHIP_HARF 29
//#define ZENEMY_SE "res\\SE_Crash.wav"

/*
* @brief MainClass
*/
class CShoutingHockey
{
public:
	//! �L�[���͂���Ă���t���[�����L�^����
	int Key[256] = { 0 };
	//! �ꎞ��~
	bool Pause = false;
	//! �^�C��
	int time = 0;

	//! �O���t�B�b�N�n���h��
	int GHTitle = 0;
	int GHFade = 0;
	int GHStageFloor = 0;
	int GHStageWall = 0;
	int GHBumper = 0;
	int GHPlayer[MOVE_DIR_MAX * PLAYER_PATTERN] = { 0 };
	int GHZEnemy[ZENEMY_PATTERN] = { 0 };
	int GHPortal = 0;

	//! �T�E���h�n���h��

	//�@�R���X�g���N�^
	CShoutingHockey();
	//�@�f�X�g���N�^
	~CShoutingHockey();

	//! Run
	void Run();

	//! �ړ�
	virtual void Move();
	//! �`��
	virtual void Draw();

	//! �^�X�N���X�g
	CRemTaskList *TitleList,*FadeList, *BGList,*BumperList, *PlayerList, *EnemyList, *PortalList;
	void MoveTask(CRemTaskList* list);
	void DrawTask(CRemTaskList* list);
};

extern CShoutingHockey* SH;