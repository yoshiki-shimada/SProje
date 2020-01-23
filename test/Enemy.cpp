/******************************************************************
* @file		Enemy.cpp
* @brief	敵のcpp
* @author	yshimada
* @data		20191221
*******************************************************************/
#include "DxLib.h"
#include "ShoutingHockey.h"
#include "Enemy.h"
#include "Player.h"
#include <math.h>


//=============================================================
// @burief コンストラクタ
//=============================================================
CZakoEnemy1::CZakoEnemy1(float x, float y, float speed)
	: CEnemy(x, y, ZENEMY_CHIP_HARF, 5, 100), Count(0), vx(0), vy(0), rad(0),Speed(speed)
{
}

//=============================================================
// 移動
//=============================================================
bool CZakoEnemy1::Move() {
	if (Count % 5 == 4) {
		CRemTaskIter i(SH->PlayerList);
		CPlayer *player = (CPlayer*)i.Next();
		rad = atan2(player->Y - Y, player->X - X);
		vx = cosf(rad) * Speed;
		vy = sinf(rad) * Speed;
		Vx = vx;
		Vy = vy;
	}

	X += vx;
	Y += vy;

		if (Time % 200 == 0) {
			int i = GetRand(180);
			//new CDirBullet(X, Y, 1.0f * i, 0.2f, 0.1f);
		}
	Time++;

	Count++;

	if (Vit <= 0) {
		//new CEnemyCrash(X, Y);
		return false;
	}

	return true;
}

//=============================================================
// 描画
//=============================================================
void CZakoEnemy1::Draw() {
	DrawGraphF(X - ZENEMY_CHIP_HARF, Y - ZENEMY_CHIP_HARF,
		SH->GHZEnemy[(Count / ZENEMY_ANIM_SPEED) % ZENEMY_PATTERN],
		TRUE
	);
}

CSpownEnemy::CSpownEnemy() {

}