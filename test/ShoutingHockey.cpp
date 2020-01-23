/*******************************************************************
* @file		SH.h
* @brief	MainClass.cpp
* @author	yshimada
* @data		20191215
*******************************************************************/

#include "DxLib.h"
#include "ShoutingHockey.h"
#include "Selector.h"
#include "BG.h"
#include "Bumper.h"
#include "Portal.h"
#include "Player.h"
#include "Enemy.h"

CShoutingHockey *SH;

//! �R���X�g���N�^
CShoutingHockey::CShoutingHockey()
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR);
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	// �^�X�N���X�g�̏�����
	TitleList = new CRemTaskList(sizeof(CTitle), 10);
	BGList = new CRemTaskList(sizeof(CBGStage1), 10);
	BumperList = new CRemTaskList(sizeof(CBumper), 10);
	PortalList = new CRemTaskList(sizeof(CPortal), 100);
	EnemyList = new CRemTaskList(sizeof(CZakoEnemy1), 100);
	PlayerList = new CRemTaskList(sizeof(CNormalPlayer), 10);
	

	// �O���t�B�b�N�n���h���̏�����
	//! �^�C�g��
	LoadDivGraph(TITLEBG, 1,
		1, 1,
		TITLE_SIZE_X, TITLE_SIZE_Y,
		&GHTitle
	);
	////! �t�F�[�h
	//LoadDivGraph(FADEBG, 1,
	//	1, 1,
	//	FADE_WH, FADE_WH,
	//	&GHFade
	//);
	//! �X�e�[�W�̏�
	LoadDivGraph(BGSTAGEFLOOR, 1,
		1, 1,
		BGSTAGE1_SIZE_X, BGSTAGE1_SIZE_Y,
		&GHStageFloor
	);
	//! �X�e�[�W��
	LoadDivGraph(BGSTAGEWALL, 1,
		1, 1,
		BGSTAGE1_SIZE_X, BGSTAGE1_SIZE_Y,
		&GHStageWall
	);
	//! �o���p�[
	LoadDivGraph(BUMPER, 1,
		1, 1,
		BUMPER_SIZE_X, BUMPER_SIZE_Y,
		&GHBumper
	);
	//! �|�[�^��
	LoadDivGraph(PORTAL, 1,
		1, 1,
		PORTAL_SIZE, PORTAL_SIZE,
		&GHPortal
	);
	//! �v���C���[
	LoadDivGraph(PLAYER_CHIP, MOVE_DIR_MAX * PLAYER_PATTERN,
		PLAYER_PATTERN, 1,
		PLAYER_SIZE, PLAYER_SIZE,
		GHPlayer
	);
	//! �G�l�~�[
	LoadDivGraph(ZENEMY_CHIP, ZENEMY_PATTERN,
		ZENEMY_PATTERN, 1,
		ZENEMY_CHIP_SIZE_X, ZENEMY_CHIP_SIZE_Y,
		GHZEnemy
	);


	//! �T�E���h�n���h���̏�����
}

//! �f�X�g���N�^
CShoutingHockey::~CShoutingHockey()
{
	DxLib_End();
}

void CShoutingHockey::Run() {
	while (!ProcessMessage()) {
		ClearDrawScreen();

		//! �������̏���
		char cTmpKey[256];
		GetHitKeyStateAll(cTmpKey);
 		for (int i = 0; i < 256; i++) {
			if (cTmpKey[i] != 0) {
				Key[i]++;
			}
			else {
				Key[i] = 0;
			}
		}

		if (Key[KEY_INPUT_ESCAPE])
			break;

		SH->Move();
		SH->Draw();

		ScreenFlip();
	}
}

/***************************************************************
* @brief �ړ�
***************************************************************/
void CShoutingHockey::Move() {
	if (Pause)
		return;

	MoveTask(TitleList);
	MoveTask(BGList);
	MoveTask(BumperList);
	MoveTask(PortalList);
	MoveTask(EnemyList);
	MoveTask(PlayerList);
}

/***************************************************************
* @brief �`��
***************************************************************/
void CShoutingHockey::Draw() {
	DrawTask(TitleList);
	DrawTask(BGList);
	DrawTask(BumperList);
	DrawTask(PortalList);
	DrawTask(EnemyList);
	DrawTask(PlayerList);
}

/****************************************************************
* @brief �^�X�N�̓���
*****************************************************************/
void CShoutingHockey::MoveTask(CRemTaskList* list) {
	for (CRemTaskIter i(list); i.HasNext(); ) {
		CMover* mover = (CMover*)i.Next();
		if (!mover->Move()) i.Remove();
	}
}


/****************************************************************
* �^�X�N�̕`��
*****************************************************************/
void CShoutingHockey::DrawTask(CRemTaskList* list) {
	for (CRemTaskIter i(list); i.HasNext(); ) {
		((CMover*)i.Next())->Draw();
	}
}

//=============================================================
// ���C�����[�`��
//=============================================================
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	SH = new CShoutingHockey();
	new CTitle();
	//! �Q�[���̎��s
	SH->Run();
	delete SH;
	return 0;
}