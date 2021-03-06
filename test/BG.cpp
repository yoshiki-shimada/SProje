#include "DxLib.h"
#include "ShoutingHockey.h"
#include "BG.h"
#include "Selector.h"

/*****************************************
* @brief 背景クラス
*****************************************/
// 背景の変更　もしかしたらステージ追加するかも
bool CBGStage1::Move()
{
	return true;
}

// 描画
void CBGStage1::Draw()
{
	// 床
	DrawGraphF((SCREEN_WIDTH - BGSTAGE1_SIZE_X) * 0.5, 0,
		SH->GHStageFloor,
		TRUE
	);
	// 壁
	DrawGraphF((SCREEN_WIDTH - BGSTAGE1_SIZE_X) * 0.5, 0,
		SH->GHStageWall,
		TRUE
	);
}
