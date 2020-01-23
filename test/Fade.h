#pragma once

#include "Mover.h"

//フェードの配列カウント用
#define FADE_MAX 24

/************************
* @brief フェードクラス
************************/
class CFade
{
private:
	int nCount = 0;
	int nFadeCount[FADE_MAX][FADE_MAX];

public:
	CFade();

	

	virtual bool Move();
};

/************************
* @brief フェード描画クラス
************************/
class CFadeIn : public CMover
{
public:
	CFadeIn(int x,int y);

	void* operator new(size_t t) {
		return operator_new(t, SH->FadeList);
	}
	void operator delete(void* p) {
		operator_delete(p, SH->FadeList);
	}

	virtual void Draw();
};