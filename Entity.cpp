#include "Entity.h"

Vec3 CCSPlayer::GetBonePosition(int BoneID)
{
	DWORD bonematrix = *this->m_dwBoneMatrix();
	Vec3 bonePos;
	bonePos.x = *(float*)(bonematrix + 0x30 * BoneID + 0x0C);
	bonePos.y = *(float*)(bonematrix + 0x30 * BoneID + 0x1C);
	bonePos.z = *(float*)(bonematrix + 0x30 * BoneID + 0x2C);
	return bonePos;
}


bool CCSPlayer::IsMoving()
{
	Vec3 playerVel = *this->m_vecVelocity();
	float velocity = playerVel.x + playerVel.y + playerVel.z;
	if (round(velocity) == 0)
		return false;

	return true;
}


bool CCSPlayer::IsDormant()
{
	return *(bool*)(this + cheat->offsets.m_bDormant);
}

bool CCSPlayer::IsValid()
{
	if (this == nullptr || this == cheat->LocalPlayer || this->IsDormant() || *this->m_iHealth() <= 0)
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

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{
	return cheat->interfaces.WeaponSystem->GetWpnData(*this->m_iItemDefinitionIndex());
}


bool C_BaseCombatWeapon::IsKnife()
{
	if (*this->m_iItemDefinitionIndex() == sdk::WEAPON_TASER) return false;
	return GetCSWeaponData()->iWeaponType == sdk::WEAPONTYPE_KNIFE;
}