/*******************************************************************
* @file		Selector.h
* @brief	�Z���N�^�[.h
* @author	yshimada
* @data		20191215
*******************************************************************/

#pragma once
//-------------------------------------------------------
#include "Mover.h"
//-------------------------------------------------------

class CSelector : public CMover
{
public:
	//�@�R���X�g���N�^
	CSelector() : CMover(SH->TitleList, 0, 0) {}
	
	void* operator new(size_t t) {
		return operator_new(t, SH->TitleList);
	}
	void operator delete(void* p) {
		operator_delete(p, SH->TitleList);
	}
};

//-------------------------------------------------------------
// @brief �萔
//-------------------------------------------------------------
enum {
	MENU_FIRST,
	MENU_SECOND,
	MENU_MAX
};

//=============================================================
// @brief �^�C�g���̃N���X
//=============================================================
class CTitle : public CSelector {
private:
	int nCount;
	bool bFade;

public:
	// ���j���[���ڂ̑I���ʒu
	int MenuPos;

	// �R���X�g���N�^
	CTitle();

	// �ړ��A�`��
	virtual bool Move();
	virtual void Draw();
};

//=============================================================
//  @brief �X�e�[�W�N���X
//=============================================================
class CStage : public CSelector {

protected:
	// �^�C�}�[
	int Time;

public:
	// �R���X�g���N�^
	CStage();

	// �ړ��A�`��
	virtual bool Move();
};

//=============================================================
//  @brief �Q�[���I�[�o�[�N���X
//=============================================================
class CGOver : public CSelector {

public:
	// ���j���[���ڂ̑I���ʒu
	int MenuPos;

	// �R���X�g���N�^
	CGOver();

	// �ړ��A�`��
	virtual bool Move();
	virtual void Draw();
};

//=============================================================
//  @brief �Q�[���N���A�N���X
//=============================================================
class CGCreal : public CSelector {

public:
	// ���j���[���ڂ̑I���ʒu
	int MenuPos;

	// �R���X�g���N�^
	CGCreal();

	// �ړ��A�`��
	virtual bool Move();
	virtual void Draw();
};