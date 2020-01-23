/*******************************************************************
* @file		Player.cpp
* @brief	playerクラス
* @author	yshimada
* @data		2019120
*******************************************************************/

#include "DxLib.h"
#include "ShoutingHockey.h"
#include "Player.h"
#include "Enemy.h"
#include "Portal.h"
#include "Bumper.h"
#include <math.h>



/******************************************************************
* @brief コンストラクタ
******************************************************************/
CPlayer::CPlayer(float x, float y, float dir) : CMover(SH->PlayerList, x, y, PLAYER_SIZE_HARF),
Dir(MOVE_DIR_NORMAL), Count(0), bHavePower(true), Speed(PLAYER_SPEED)
{
	vx = cosf(dir) * Speed;
	vy = -sinf(dir) * Speed;
}

/******************************************************************
* @brief player移動
******************************************************************/
bool CPlayer::Move() {
	//// 斜め移動の速度調整用定数
	//static const float sqrt2 = (1.0f / sqrtf(2.0f));

	// 移動範囲
	/*static const int MinX = 0, MinY = 0;
	static const int MaxX = SCREEN_WIDTH, MaxY = SCREEN_HEIGHT;*/
	static const int MinX = RLWALL_AND_PLAYER, MaxX = SCREEN_WIDTH - MinX;
	static const int MinY = TBWALL_AND_PLAYER, MaxY = SCREEN_WIDTH - MinY;

	//**************************************************

	//! 移動
	X += vx;
	Y += vy;

	// 壁での反射判定
	if (X < MinX && vx < 0) {
		vx = -vx;
		Count++;
	}
	if (Y < MinY && vy < 0) {
		vy = -vy;
		Count++;
	}
	if (X > MaxX && vx > 0) {
		vx = -vx;
		Count++;
	}
	if (Y > MaxY && vy > 0) {
		vy = -vy;
		Count++;
	}
	if (Count > 3)
		Count = 3;

	// 移動範囲の制限
	if (X < MinX)
		X = MinX;
	else if (X > MaxX)
		X = MaxX;
	if (Y < MinY)
		Y = MinY;
	else if (Y > MaxY)
		Y = MaxY;

	//--------------------------------------------------------------
	//! Bumperとの当たり判定
	for (CRemTaskIter i(SH->BumperList); i.HasNext();) {
		CBumper *Bumper = (CBumper*)i.Next();
		//! 当たったとき
		if (CSHit(Bumper, PLAYER_SIZE_HARF, PLAYER_SIZE_HARF)) {
			////! TB
			//if (X >= 250 && X <= (SCREEN_WIDTH - 250)) {
			//	vx = vx * 1.5;
			//	vy = vy * 1.5;
			//}
			////! RL
			//else {
			//	vx = vx * 1.1;
			//	vy = vy * 1.1;
			//}
			if (Speed > 2) {
				Speed--;
				fRad = atan2(vy, vx);
				vx = cosf(fRad) * Speed;
				vy = sinf(fRad) * Speed;
			}
		}
	}

	return true;
}

/******************************************************************
* @brief 描画
******************************************************************/
void CPlayer::Draw() {
	DrawGraphF(X - (PLAYER_SIZE_HARF), Y - (PLAYER_SIZE_HARF),
		SH->GHPlayer[(Dir * PLAYER_PATTERN) + Count],
		TRUE
	);
}

/******************************************************************
* @brief player当たり判定等
******************************************************************/
CNormalPlayer::CNormalPlayer(float x, float y, float dir)
	: CPlayer(x, y, dir)
{
}

//=============================================================
// 移動
//=============================================================
bool CNormalPlayer::Move() {
	// 自機に共通の移動処理
	CPlayer::Move();

	//! Bumperは移動との兼ね合いがあるため上に書きます。

	//! ポータルとの当たり判定
	for (CRemTaskIter i(SH->PortalList); i.HasNext();) {
		CPortal *Portal = (CPortal*)i.Next();
		if (CCHit(Portal) && !Portal->bSetPortal) {
			X = Portal->X;
			Y = Portal->Y;
			vx = 0;
			vy = 0;
			Portal->bSetPortal = true;
		}
		else if (CCHit(Portal) && Portal->bSetPortal) {
			if (SH->Key[KEY_INPUT_SPACE]) {
				vx = cosf(Portal->dPortaldir) * Speed;
				vy = sinf(Portal->dPortaldir) * Speed;
			}
		}
		else if (!CCHit(Portal) && Portal->bSetPortal) {
			Portal->bSetPortal = false;
		}
	}

	//! 敵との当たり判定
	for (CRemTaskIter i(SH->EnemyList); i.HasNext();) {
		CEnemy *Enemy = (CEnemy*)i.Next();
		//! アタックボールを持っていないとき
		if (CCHit(Enemy) && !bHavePower) {
			//new CMyShipCrash(X, Y);
			//DeletePlayer();
			//DeleteBeam();
			//return false;
		}
		//! アタックボールを持っているとき
		else if (CCHit(Enemy) && bHavePower) {
			Enemy->Vit -= 1 + Count;
			//! 連鎖
			if (Enemy->Vit <= 0) {
				//! 消す
			}
			//! 跳ね返る
			else {
				PEVal = atan2(Enemy->Y - Y, Enemy->X - X);
				EPVal = atan2(Y - Enemy->Y, X - Enemy->X);
				V1 = Disperse(vx, vy, PEVal);
				V2 = Disperse(Enemy->Vx, Enemy->Vy, EPVal);
				//! 行列計算
				vx = V1[0] + V2[2];
				vy = V1[1] + V2[3];
				Enemy->Vx = V1[2] + V2[0];
				Enemy->Vy = V1[3] + V2[1];
			}
		}
	}

	//// 爆発
	//if (Vit <= 0) {
	//	new CMyShipCrash(X, Y);
	//  DeletePlayer();
	//	DeleteBeam();
	//	return false;
	//}

	return true;
}

/*
* @brief 反射用関数
*/
float *CNormalPlayer::Disperse(float Uxs, float Uys, float UVal1)
{
	SV0 = sqrt(Uxs * Uxs + Uys * Uys);//速度(x速度とy速度の合成「対角線の長さ」）
	UVal2 = atan2(Uys, Uxs);//自球進行方向
	kakuSA = UVal2 - UVal1;//「衝突後相手球の進行方向」に対する「自球衝突前進行方向」の角度の差
	SV1 = fabs(SV0 * cos(kakuSA));//衝突後の相手球の速度
	SV2 = fabs(SV0 * sin(kakuSA));//衝突後の自球の速度
	float Vxs = SV1 * cos(UVal1);//衝突後の相手球のx速度
	float Vys = SV1 * sin(UVal1);//衝突後の相手球のy速度

	if (sin(kakuSA) < 0) {
		Uxs = SV2 * cos(UVal1 - PI / 2);//衝突後の自球のx速度
		Uys = SV2 * sin(UVal1 - PI / 2);//衝突後の自球のy速度
	}
	else {
		Uxs = SV2 * cos(UVal1 + PI / 2);//衝突後の自球のx速度
		Uys = SV2 * sin(UVal1 + PI / 2);//衝突後の自球のy速度
	}

	//*V3 = [Uxs, Uys, Vxs, Vys];

	V3[0] = Uxs;
	V3[1] = Uys;
	V3[2] = Vxs;
	V3[3] = Vys;

	return V3;//出力：衝突後の自球のx速度、y速度、相手球のx速度、y速度
}

//=============================================================
CRevivalMyShip::CRevivalMyShip(float x, float y, float dir)
	: CPlayer(x, y, dir), Time(0)
{
}

//=============================================================
// 移動
//=============================================================
bool CRevivalMyShip::Move() {
	// 自機に共通の移動処理
	CPlayer::Move();

	Time++;
	if (Time > 120) {
		new CNormalPlayer(X, Y, PI * -0.25);
		return false;
	}


	return true;
}

//=============================================================
// 描画
//=============================================================
void CRevivalMyShip::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);
	DrawGraphF(X - (PLAYER_SIZE_HARF), Y - (PLAYER_SIZE_HARF),
		SH->GHPlayer[(Dir * PLAYER_PATTERN) + Count],
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}