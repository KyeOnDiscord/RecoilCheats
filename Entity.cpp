#include "Entity.h"

bool CCSPlayer::IsMoving()
{
	Vec3 playerVel = *this->m_vecVelocity();
	float velocity = playerVel.x + playerVel.y + playerVel.z;
	if (round(velocity) == 0)
		return false;

	return true;
}

bool CCSPlayer::IsValid()
{
		if (this == nullptr || this == cheat->variables.LocalPlayer ||/* ent->isDormant ||*/ *this->m_iHealth() <= 0)
			return false;

		return true;
}

int GetClassID(CCSPlayer* ent) //Creates our class ID for the entity we put inside
{
	uintptr_t Entity = uintptr_t(ent);
	uintptr_t dwClientNetworkable = *(uintptr_t*)(Entity + 0x8);
	uintptr_t dwGetClientClassFn = *(uintptr_t*)(dwClientNetworkable + 2 * 0x4);
	uintptr_t dwEntityClientClass = *(uintptr_t*)(dwGetClientClassFn + 1);
	return *(int*)(dwEntityClientClass + 20);
}