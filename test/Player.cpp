/*******************************************************************
* @file		Player.cpp
* @brief	player�N���X
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
* @brief �R���X�g���N�^
******************************************************************/
CPlayer::CPlayer(float x, float y, float dir) : CMover(SH->PlayerList, x, y, PLAYER_SIZE_HARF),
Dir(MOVE_DIR_NORMAL), Count(0), bHavePower(true), Speed(PLAYER_SPEED)
{
	vx = cosf(dir) * Speed;
	vy = -sinf(dir) * Speed;
}

/******************************************************************
* @brief player�ړ�
******************************************************************/
bool CPlayer::Move() {
	//// �΂߈ړ��̑��x�����p�萔
	//static const float sqrt2 = (1.0f / sqrtf(2.0f));

	// �ړ��͈�
	/*static const int MinX = 0, MinY = 0;
	static const int MaxX = SCREEN_WIDTH, MaxY = SCREEN_HEIGHT;*/
	static const int MinX = RLWALL_AND_PLAYER, MaxX = SCREEN_WIDTH - MinX;
	static const int MinY = TBWALL_AND_PLAYER, MaxY = SCREEN_WIDTH - MinY;

	//**************************************************

	//! �ړ�
	X += vx;
	Y += vy;

	// �ǂł̔��˔���
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

	// �ړ��͈͂̐���
	if (X < MinX)
		X = MinX;
	else if (X > MaxX)
		X = MaxX;
	if (Y < MinY)
		Y = MinY;
	else if (Y > MaxY)
		Y = MaxY;

	//--------------------------------------------------------------
	//! Bumper�Ƃ̓����蔻��
	for (CRemTaskIter i(SH->BumperList); i.HasNext();) {
		CBumper *Bumper = (CBumper*)i.Next();
		//! ���������Ƃ�
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
* @brief �`��
******************************************************************/
void CPlayer::Draw() {
	DrawGraphF(X - (PLAYER_SIZE_HARF), Y - (PLAYER_SIZE_HARF),
		SH->GHPlayer[(Dir * PLAYER_PATTERN) + Count],
		TRUE
	);
}

/******************************************************************
* @brief player�����蔻�蓙
******************************************************************/
CNormalPlayer::CNormalPlayer(float x, float y, float dir)
	: CPlayer(x, y, dir)
{
}

//=============================================================
// �ړ�
//=============================================================
bool CNormalPlayer::Move() {
	// ���@�ɋ��ʂ̈ړ�����
	CPlayer::Move();

	//! Bumper�͈ړ��Ƃ̌��ˍ��������邽�ߏ�ɏ����܂��B

	//! �|�[�^���Ƃ̓����蔻��
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

	//! �G�Ƃ̓����蔻��
	for (CRemTaskIter i(SH->EnemyList); i.HasNext();) {
		CEnemy *Enemy = (CEnemy*)i.Next();
		//! �A�^�b�N�{�[���������Ă��Ȃ��Ƃ�
		if (CCHit(Enemy) && !bHavePower) {
			//new CMyShipCrash(X, Y);
			//DeletePlayer();
			//DeleteBeam();
			//return false;
		}
		//! �A�^�b�N�{�[���������Ă���Ƃ�
		else if (CCHit(Enemy) && bHavePower) {
			Enemy->Vit -= 1 + Count;
			//! �A��
			if (Enemy->Vit <= 0) {
				//! ����
			}
			//! ���˕Ԃ�
			else {
				PEVal = atan2(Enemy->Y - Y, Enemy->X - X);
				EPVal = atan2(Y - Enemy->Y, X - Enemy->X);
				V1 = Disperse(vx, vy, PEVal);
				V2 = Disperse(Enemy->Vx, Enemy->Vy, EPVal);
				//! �s��v�Z
				vx = V1[0] + V2[2];
				vy = V1[1] + V2[3];
				Enemy->Vx = V1[2] + V2[0];
				Enemy->Vy = V1[3] + V2[1];
			}
		}
	}

	//// ����
	//if (Vit <= 0) {
	//	new CMyShipCrash(X, Y);
	//  DeletePlayer();
	//	DeleteBeam();
	//	return false;
	//}

	return true;
}

/*
* @brief ���˗p�֐�
*/
float *CNormalPlayer::Disperse(float Uxs, float Uys, float UVal1)
{
	SV0 = sqrt(Uxs * Uxs + Uys * Uys);//���x(x���x��y���x�̍����u�Ίp���̒����v�j
	UVal2 = atan2(Uys, Uxs);//�����i�s����
	kakuSA = UVal2 - UVal1;//�u�Փˌ㑊�苅�̐i�s�����v�ɑ΂���u�����ՓˑO�i�s�����v�̊p�x�̍�
	SV1 = fabs(SV0 * cos(kakuSA));//�Փˌ�̑��苅�̑��x
	SV2 = fabs(SV0 * sin(kakuSA));//�Փˌ�̎����̑��x
	float Vxs = SV1 * cos(UVal1);//�Փˌ�̑��苅��x���x
	float Vys = SV1 * sin(UVal1);//�Փˌ�̑��苅��y���x

	if (sin(kakuSA) < 0) {
		Uxs = SV2 * cos(UVal1 - PI / 2);//�Փˌ�̎�����x���x
		Uys = SV2 * sin(UVal1 - PI / 2);//�Փˌ�̎�����y���x
	}
	else {
		Uxs = SV2 * cos(UVal1 + PI / 2);//�Փˌ�̎�����x���x
		Uys = SV2 * sin(UVal1 + PI / 2);//�Փˌ�̎�����y���x
	}

	//*V3 = [Uxs, Uys, Vxs, Vys];

	V3[0] = Uxs;
	V3[1] = Uys;
	V3[2] = Vxs;
	V3[3] = Vys;

	return V3;//�o�́F�Փˌ�̎�����x���x�Ay���x�A���苅��x���x�Ay���x
}

//=============================================================
CRevivalMyShip::CRevivalMyShip(float x, float y, float dir)
	: CPlayer(x, y, dir), Time(0)
{
}

//=============================================================
// �ړ�
//=============================================================
bool CRevivalMyShip::Move() {
	// ���@�ɋ��ʂ̈ړ�����
	CPlayer::Move();

	Time++;
	if (Time > 120) {
		new CNormalPlayer(X, Y, PI * -0.25);
		return false;
	}


	return true;
}

//=============================================================
// �`��
//=============================================================
void CRevivalMyShip::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);
	DrawGraphF(X - (PLAYER_SIZE_HARF), Y - (PLAYER_SIZE_HARF),
		SH->GHPlayer[(Dir * PLAYER_PATTERN) + Count],
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}