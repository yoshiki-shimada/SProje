#include "DxLib.h"
#include "ShoutingHockey.h"
#include "Fade.h"

CFade::CFade()
{
}

bool CFade::Move() {
	for (int x = FADE_MAX; x <= 0; x++) {
		for (int y = FADE_MAX; y <= 0;) {
			nCount++;
			if (nCount >= 10) {
				new CFadeIn(x, y);
				nCount = 0;
			}
		}

	}
	return true;
}

CFadeIn::CFadeIn(int x, int y) : CMover(SH->FadeList, 0, 0)
{
	X = x;
	Y = y;
}

void CFadeIn::Draw() {

	DrawGraphF(FADE_WH * X, FADE_WH * Y,
		SH->GHFade,
		FALSE
	);
}