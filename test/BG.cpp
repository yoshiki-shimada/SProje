#include "DxLib.h"
#include "ShoutingHockey.h"
#include "BG.h"
#include "Selector.h"

/*****************************************
* @brief �w�i�N���X
*****************************************/
// �w�i�̕ύX�@������������X�e�[�W�ǉ����邩��
bool CBGStage1::Move()
{
	return true;
}

// �`��
void CBGStage1::Draw()
{
	// ��
	DrawGraphF((SCREEN_WIDTH - BGSTAGE1_SIZE_X) * 0.5, 0,
		SH->GHStageFloor,
		TRUE
	);
	// ��
	DrawGraphF((SCREEN_WIDTH - BGSTAGE1_SIZE_X) * 0.5, 0,
		SH->GHStageWall,
		TRUE
	);
}
