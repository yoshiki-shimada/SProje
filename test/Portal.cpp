#include "DxLib.h"
#include "ShoutingHockey.h"
#include "Portal.h"
#include "Selector.h"

#define PI2 PI * 2

CPortal::CPortal(float x, float y, double dir)
	: CMover(SH->PortalList, x, y, PORTAL_SIZE_HARF),
	nCount(0), nRotSpeed(5), dPortaldir(dir), dRotdir(PI * 0.25 * 0.25), bSetPortal(false)
{
}


bool CPortal::Move() {
	nCount++;

	if (nCount >= nRotSpeed) {
		nCount = 0;
		dPortaldir += dRotdir;
		if (dPortaldir > PI2)
			dPortaldir -= PI2;
	}

	return true;
}

void CPortal::Draw() {
	DrawRotaGraph(X, Y, 1.0, dPortaldir,
		SH->GHPortal, TRUE);
}