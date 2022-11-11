#include "Entity.h"


class BoneMatrix
{
public:
	float m[3][4];
};

Vec3 CCSPlayer::GetBonePosition(int BoneID)
{
	DWORD bonematrix = *this->m_dwBoneMatrix();
	BoneMatrix boneM = *(BoneMatrix*)(bonematrix + 0x30 * BoneID);
	return Vec3(boneM.m[0][3], boneM.m[1][3], boneM.m[2][3]);
}


studiohdr_t CCSPlayer::GetBoneInfo()
{
	uintptr_t StudioHdr = *(uintptr_t*)((uintptr_t(this) + cheat->offsets.m_pStudioHdr));
	studiohdr_t studiohdrreal = *(studiohdr_t*)(*((uintptr_t*)StudioHdr));
	return studiohdrreal;
}

bool IsTooFar(Vec3 origin, Vec3 bone)
{
	float distance = origin.Distance(bone);
	if (distance > 150)
		return true;
	return false;
}

float* CCSPlayer::GetBoneBounds()
{
	//First bone is the most left bone on screen, right bone is the most right bone on screen
	int numBones = this->GetBoneInfo().numbones;
	Vec3 origin = *this->m_vecOrigin();
	float SmallestBone = FLT_MAX;
	for (int i = 0; i < numBones; i++)
	{
		Vec3 bonePos = this->GetBonePosition(i);
		if (IsTooFar(origin, bonePos))
			continue;
		Vec2 screenPos;
		if (cheat->WorldToScreen(bonePos, screenPos) && screenPos.x < SmallestBone)
		{
			SmallestBone = screenPos.x;
		}
	}
	float LargestBone = 0;

	for (int i = 0; i < numBones; i++)
	{
		Vec3 bonePos = this->GetBonePosition(i);
		if (IsTooFar(origin, bonePos))
			continue;

		Vec2 screenPos;
		if (cheat->WorldToScreen(bonePos, screenPos) && screenPos.x > LargestBone)
			LargestBone = screenPos.x;

	}
	float result[2] = { SmallestBone, LargestBone };
	return result;
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