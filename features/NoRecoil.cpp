#include "..\pch.h"
#include "..\Entity.h"
extern Cheat* cheat;

void NoRecoil()
{

	Vec3* viewAngles = cheat->GetViewAngles();
	int* iShotsFired = cheat->LocalPlayer->m_iShotsFired();
	Vec3* aimPunchAngle = cheat->LocalPlayer->m_aimPunchAngle();
	static Vec3 oPunch{ 0,0,0 };

	Vec3 punchAngle = *aimPunchAngle * 2;
	if (*iShotsFired > 1) {
		// calc rcs
		Vec3 newAngle = *viewAngles + oPunch - punchAngle;
		// normalize
		newAngle.Normalize();
		// set
		*viewAngles = newAngle;
	}
	// fix
	oPunch = punchAngle;
}