#pragma once
//--------------------------------------------------------------
#include "Mover.h"

/*************************************************
* @brief �X�e�[�W�N���X
*************************************************/
class CBGStage1 : public CMover
{
protected:
	

public:
	CBGStage1() : CMover(SH->BGList, 0, 0) {}
	
	void* operator new(size_t t) {
		return operator_new(t, SH->BGList);
	}
	void operator delete(void* p) {
		operator_delete(p, SH->BGList);
	}

	// �ړ��A�`��
	virtual bool Move();
	virtual void Draw();
};