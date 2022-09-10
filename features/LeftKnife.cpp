#include "..\pch.h"
#include "..\sdk\sdk.h"
#include "..\Entity.h"

extern Cheat* cheat;
void LeftHandKnife()
{
	if (cheat->LocalPlayer == nullptr)
		return;

	C_BaseCombatWeapon* curItem = (C_BaseCombatWeapon*)cheat->interfaces.ClientEntityList->GetClientEntityFromHandle(*cheat->LocalPlayer->m_hActiveWeapon());


	static CCSWeaponInfo* LastWeaponData = curItem->GetCSWeaponData();
	CCSWeaponInfo* CurrentWeaponData = curItem->GetCSWeaponData();
	//If statement is true when the user changes their weapon
	if (LastWeaponData->szWeaponName != CurrentWeaponData->szWeaponName)
	{
		cheat->interfaces.EngineClient->ClientCmd_Unrestricted(curItem->IsKnife() ? skCrypt("cl_righthand 0") : skCrypt("cl_righthand 1"));
		LastWeaponData = curItem->GetCSWeaponData();
	}
}