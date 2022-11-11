#pragma once
#include "pch.h"
extern Cheat* cheat;

#define GetNetVar(funcname, Class, NetVarName, type, EndOffset) \
type* funcname() \
{ \
	ClientClass* clientClass = cheat->interfaces.BaseClientDLL->GetAllClasses(); \
 static auto offset = GetNetVarOffset(Class, NetVarName, clientClass); \
 auto address = uintptr_t(this) + offset + EndOffset; \
return reinterpret_cast<type*>(address); \
}


struct studiohdr_t
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	Vec3 eyeposition;
	Vec3 illumposition;
	Vec3 hull_min;
	Vec3 hull_max;
	Vec3 view_bbmin;
	Vec3 view_bbmax;
	int flags;
	int numbones;                // bones
	int boneindex;
	int numbonecontrollers;      // bone controllers
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
};


class CCSWeaponInfo { //xSeeker
public:
	int8_t pad0[20];
	int32_t iMaxClip1;
	int8_t pad1[12];
	int32_t iMaxReservedAmmo;
	int8_t pad2[96];
	char* szHudName;
	char* szWeaponName;
	int8_t pad3[56];
	int32_t iWeaponType;
	int8_t pad4[4];
	int32_t iWeaponPrice;
	int32_t iKillAward;
	int8_t pad5[20];
	uint8_t bFullAuto;
	int8_t pad6[3];
	int32_t iDamage;
	float_t flArmorRatio;
	int32_t iBullets;
	float_t flPenetration;
	int8_t pad7[8];
	float_t flRange;
	float_t flRangeModifier;
	int8_t pad8[16];
	uint8_t bHasSilencer;
	int8_t pad9[15];
	float_t flSpread;
	float_t flSpreadAlt;
	int8_t pad10[76];
	int32_t iRecoilSeed;
	int8_t pad11[32];
};





class C_BaseAttributableItem
{

public:

	GetNetVar(m_iItemDefinitionIndex, skCrypt("DT_BaseAttributableItem"), skCrypt("m_iItemDefinitionIndex"), int16_t, 0);
};


class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	bool IsKnife();


	CCSWeaponInfo* GetCSWeaponData();
};


class CCSPlayer
{
public:
	bool IsMoving();
	bool IsValid();
	Vec3 GetBonePosition(int BoneID);
	float* GetBoneBounds();
	studiohdr_t GetBoneInfo();
	bool IsDormant();
	GetNetVar(m_iHealth, skCrypt("DT_BasePlayer"), skCrypt("m_iHealth"), int, 0);
	GetNetVar(m_vecOrigin, skCrypt("DT_BasePlayer"), skCrypt("m_vecOrigin"), Vec3, 0);
	GetNetVar(m_vecVelocity, skCrypt("DT_CSPlayer"), skCrypt("m_vecVelocity[0]"), Vec3, 0);
	GetNetVar(m_iDefaultFOV, skCrypt("DT_CSPlayer"), skCrypt("m_iDefaultFOV"), int, 0);
	GetNetVar(m_fFlags, skCrypt("DT_CSPlayer"), skCrypt("m_fFlags"), BYTE, 0);
	GetNetVar(m_hActiveWeapon, skCrypt("DT_BaseCombatCharacter"), skCrypt("m_hActiveWeapon"), int, 0);
	GetNetVar(m_dwBoneMatrix, skCrypt("DT_BaseAnimating"), skCrypt("m_nForceBone"), int, 28);
	GetNetVar(m_aimPunchAngle, skCrypt("DT_BasePlayer"), skCrypt("m_aimPunchAngle"), Vec3, 0);
	GetNetVar(m_iShotsFired, skCrypt("DT_CSPlayer"), skCrypt("m_iShotsFired"), int, 0);
};

int GetClassID(CCSPlayer* ent);

namespace EntID
{
	enum EntID
	{
		CAI_BaseNPC,
		CAK47,
		CBaseAnimating,
		CBaseAnimatingOverlay,
		CBaseAttributableItem,
		CBaseButton,
		CBaseCombatCharacter,
		CBaseCombatWeapon,
		CBaseCSGrenade,
		CBaseCSGrenadeProjectile,
		CBaseDoor,
		CBaseEntity,
		CBaseFlex,
		CBaseGrenade,
		CBaseParticleEntity,
		CBasePlayer,
		CBasePropDoor,
		CBaseTeamObjectiveResource,
		CBaseTempEntity,
		CBaseToggle,
		CBaseTrigger,
		CBaseViewModel,
		CBaseVPhysicsTrigger,
		CBaseWeaponWorldModel,
		CBeam,
		CBeamSpotlight,
		CBoneFollower,
		CBRC4Target,
		CBreachCharge,
		CBreachChargeProjectile,
		CBreakableProp,
		CBreakableSurface,
		CBumpMine,
		CBumpMineProjectile,
		CC4,
		CCascadeLight,
		CChicken,
		CColorCorrection,
		CColorCorrectionVolume,
		CCSGameRulesProxy,
		CCSPlayer,
		CCSPlayerResource,
		CCSRagdoll,
		CCSTeam,
		CDangerZone,
		CDangerZoneController,
		CDEagle,
		CDecoyGrenade,
		CDecoyProjectile,
		CDrone,
		CDronegun,
		CDynamicLight,
		CDynamicProp,
		CEconEntity,
		CEconWearable,
		CEmbers,
		CEntityDissolve,
		CEntityFlame,
		CEntityFreezing,
		CEntityParticleTrail,
		CEnvAmbientLight,
		CEnvDetailController,
		CEnvDOFController,
		CEnvGasCanister,
		CEnvParticleScript,
		CEnvProjectedTexture,
		CEnvQuadraticBeam,
		CEnvScreenEffect,
		CEnvScreenOverlay,
		CEnvTonemapController,
		CEnvWind,
		CFEPlayerDecal,
		CFireCrackerBlast,
		CFireSmoke,
		CFireTrail,
		CFish,
		CFists,
		CFlashbang,
		CFogController,
		CFootstepControl,
		CFunc_Dust,
		CFunc_LOD,
		CFuncAreaPortalWindow,
		CFuncBrush,
		CFuncConveyor,
		CFuncLadder,
		CFuncMonitor,
		CFuncMoveLinear,
		CFuncOccluder,
		CFuncReflectiveGlass,
		CFuncRotating,
		CFuncSmokeVolume,
		CFuncTrackTrain,
		CGameRulesProxy,
		CGrassBurn,
		CHandleTest,
		CHEGrenade,
		CHostage,
		CHostageCarriableProp,
		CIncendiaryGrenade,
		CInferno,
		CInfoLadderDismount,
		CInfoMapRegion,
		CInfoOverlayAccessor,
		CItem_Healthshot,
		CItemCash,
		CItemDogtags,
		CKnife,
		CKnifeGG,
		CLightGlow,
		CMapVetoPickController,
		CMaterialModifyControl,
		CMelee,
		CMolotovGrenade,
		CMolotovProjectile,
		CMovieDisplay,
		CParadropChopper,
		CParticleFire,
		CParticlePerformanceMonitor,
		CParticleSystem,
		CPhysBox,
		CPhysBoxMultiplayer,
		CPhysicsProp,
		CPhysicsPropMultiplayer,
		CPhysMagnet,
		CPhysPropAmmoBox,
		CPhysPropLootCrate,
		CPhysPropRadarJammer,
		CPhysPropWeaponUpgrade,
		CPlantedC4,
		CPlasma,
		CPlayerPing,
		CPlayerResource,
		CPointCamera,
		CPointCommentaryNode,
		CPointWorldText,
		CPoseController,
		CPostProcessController,
		CPrecipitation,
		CPrecipitationBlocker,
		CPredictedViewModel,
		CProp_Hallucination,
		CPropCounter,
		CPropDoorRotating,
		CPropJeep,
		CPropVehicleDriveable,
		CRagdollManager,
		CRagdollProp,
		CRagdollPropAttached,
		CRopeKeyframe,
		CSCAR17,
		CSceneEntity,
		CSensorGrenade,
		CSensorGrenadeProjectile,
		CShadowControl,
		CSlideshowDisplay,
		CSmokeGrenade,
		CSmokeGrenadeProjectile,
		CSmokeStack,
		CSnowball,
		CSnowballPile,
		CSnowballProjectile,
		CSpatialEntity,
		CSpotlightEnd,
		CSprite,
		CSpriteOriented,
		CSpriteTrail,
		CStatueProp,
		CSteamJet,
		CSun,
		CSunlightShadowControl,
		CSurvivalSpawnChopper,
		CTablet,
		CTeam,
		CTeamplayRoundBasedRulesProxy,
		CTEArmorRicochet,
		CTEBaseBeam,
		CTEBeamEntPoint,
		CTEBeamEnts,
		CTEBeamFollow,
		CTEBeamLaser,
		CTEBeamPoints,
		CTEBeamRing,
		CTEBeamRingPoint,
		CTEBeamSpline,
		CTEBloodSprite,
		CTEBloodStream,
		CTEBreakModel,
		CTEBSPDecal,
		CTEBubbles,
		CTEBubbleTrail,
		CTEClientProjectile,
		CTEDecal,
		CTEDust,
		CTEDynamicLight,
		CTEEffectDispatch,
		CTEEnergySplash,
		CTEExplosion,
		CTEFireBullets,
		CTEFizz,
		CTEFootprintDecal,
		CTEFoundryHelpers,
		CTEGaussExplosion,
		CTEGlowSprite,
		CTEImpact,
		CTEKillPlayerAttachments,
		CTELargeFunnel,
		CTEMetalSparks,
		CTEMuzzleFlash,
		CTEParticleSystem,
		CTEPhysicsProp,
		CTEPlantBomb,
		CTEPlayerAnimEvent,
		CTEPlayerDecal,
		CTEProjectedDecal,
		CTERadioIcon,
		CTEShatterSurface,
		CTEShowLine,
		CTesla,
		CTESmoke,
		CTESparks,
		CTESprite,
		CTESpriteSpray,
		CTest_ProxyToggle_Networkable,
		CTestTraceline,
		CTEWorldDecal,
		CTriggerPlayerMovement,
		CTriggerSoundOperator,
		CVGuiScreen,
		CVoteController,
		CWaterBullet,
		CWaterLODControl,
		CWeaponAug,
		CWeaponAWP,
		CWeaponBaseItem,
		CWeaponBizon,
		CWeaponCSBase,
		CWeaponCSBaseGun,
		CWeaponCycler,
		CWeaponElite,
		CWeaponFamas,
		CWeaponFiveSeven,
		CWeaponG3SG1,
		CWeaponGalil,
		CWeaponGalilAR,
		CWeaponGlock,
		CWeaponHKP2000,
		CWeaponM249,
		CWeaponM3,
		CWeaponM4A1,
		CWeaponMAC10,
		CWeaponMag7,
		CWeaponMP5Navy,
		CWeaponMP7,
		CWeaponMP9,
		CWeaponNegev,
		CWeaponNOVA,
		CWeaponP228,
		CWeaponP250,
		CWeaponP90,
		CWeaponSawedoff,
		CWeaponSCAR20,
		CWeaponScout,
		CWeaponSG550,
		CWeaponSG552,
		CWeaponSG556,
		CWeaponShield,
		CWeaponSSG08,
		CWeaponTaser,
		CWeaponTec9,
		CWeaponTMP,
		CWeaponUMP45,
		CWeaponUSP,
		CWeaponXM1014,
		CWeaponZoneRepulsor,
		CWorld,
		CWorldVguiText,
		DustTrail,
		MovieExplosion,
		ParticleSmokeGrenade,
		RocketTrail,
		SmokeTrail,
		SporeExplosion,
		SporeTrail,
	};
}