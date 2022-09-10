#pragma once
#include "netvars.h"
typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);



class CCSWeaponInfo;

namespace sdk
{

	enum ItemDefinitionIndex
	{
		WEAPON_INVALID = -1,
		WEAPON_DEAGLE = 1,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALILAR = 13,
		WEAPON_M249,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5 = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_TASER,
		WEAPON_HKP2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG556,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1_SILENCER,
		WEAPON_USP_SILENCER,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};
	enum CSWeaponType
	{
		WEAPONTYPE_KNIFE = 0,
		WEAPONTYPE_PISTOL,
		WEAPONTYPE_SUBMACHINEGUN,
		WEAPONTYPE_RIFLE,
		WEAPONTYPE_SHOTGUN,
		WEAPONTYPE_SNIPER_RIFLE,
		WEAPONTYPE_MACHINEGUN,
		WEAPONTYPE_C4,
		WEAPONTYPE_PLACEHOLDER,
		WEAPONTYPE_GRENADE,
		WEAPONTYPE_UNKNOWN
	};



	template <typename Fn> __forceinline Fn GetVirtualFunction(void* pClassBase, int nFunctionIndex)
	{
		return (Fn)((PDWORD) * (PDWORD*)pClassBase)[nFunctionIndex];
	}


	void* GetInterface(HMODULE hModule, const char* interfacename);

	class IBaseClientDLL
	{
	public:
		virtual void fn0() = 0;
		virtual void fn1() = 0;
		virtual void fn2() = 0;
		virtual void fn3() = 0;
		virtual void fn4() = 0;
		virtual void fn5() = 0;
		virtual void fn6() = 0;
		virtual void fn7() = 0;

		virtual ClientClass* GetAllClasses() = 0;

		// Called once per level to re-initialize any hud element drawing stuff
		virtual int				HudVidInit(void) = 0;
		// Called by the engine when gathering user input
		virtual void			HudProcessInput(bool bActive) = 0;
		// Called oncer per frame to allow the hud elements to think
		virtual void			HudUpdate(bool bActive) = 0;
		// Reset the hud elements to their initial states
		virtual void			HudReset(void) = 0;
		// Display a hud text message
		virtual void			HudText(const char* message) = 0;

		// Mouse Input Interfaces
		// Activate the mouse (hides the cursor and locks it to the center of the screen)
		virtual void			IN_ActivateMouse(void) = 0;
		// Deactivates the mouse (shows the cursor and unlocks it)
		virtual void			IN_DeactivateMouse(void) = 0;
	};


	class IWeaponSystem
	{
		virtual void unused0() = 0;
		virtual void unused1() = 0;
	public:
		virtual CCSWeaponInfo* GetWpnData(unsigned ItemDefinitionIndex);
	};

	//https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h
	class IClientEntityList
	{
	public:
		// Get IClientNetworkable interface for specified entity
		virtual void* GetClientNetworkable(int entnum) = 0;
		virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
		virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

		// NOTE: This function is only a convenience wrapper.
		// It returns GetClientNetworkable( entnum )->GetIClientEntity().
		virtual void* GetClientEntity(int entnum) = 0;
		virtual void* GetClientEntityFromHandle(int hEnt) = 0;

		// Returns number of entities currently in use
		virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

		// Returns highest index actually used
		virtual int					GetHighestEntityIndex(void) = 0;

		// Sizes entity list to specified size
		virtual void				SetMaxEntities(int maxents) = 0;
		virtual int					GetMaxEntities() = 0;
	};



	typedef struct player_info_s
	{
		__int64         unknown;            //0x0000 
		union
		{
			__int64       steamID64;          //0x0008 - SteamID64
			struct
			{
				__int32     xuid_low;
				__int32     xuid_high;
			};
		};
		char            szName[128];        //0x0010 - Player Name
		int             userId;             //0x0090 - Unique Server Identifier
		char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
		char            pad_0x00A8[0x10];   //0x00A8
		unsigned long   iSteamID;           //0x00B8 - SteamID 
		char            szFriendsName[128];
		bool            fakeplayer;
		bool            ishltv;
		unsigned int    customfiles[4];
		unsigned char   filesdownloaded;
	} player_info_t;


	struct model_t;
	struct Vector;
	struct IMaterial;
	struct IMaterialSystem;
	typedef float vec_t;
	class QAngle;
	class VMatrix
	{
	public:
		float  m[4][4];
	};
	class IVEngineClient
	{
	public:
		virtual int                   GetIntersectingSurfaces(const model_t* model, const Vector& vCenter, const float radius, const bool bOnlyVisibleSurfaces, void* pInfos, const int nMaxInfos) = 0;
		virtual Vector                GetLightForPoint(const Vector& pos, bool bClamp) = 0;
		virtual IMaterial* TraceLineMaterialAndLighting(const Vector& start, const Vector& end, Vector& diffuseLightColor, Vector& baseColor) = 0;
		virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
		virtual bool                  CopyFile(const char* source, const char* destination) = 0;
		virtual void                  GetScreenSize(int& width, int& height) = 0;
		virtual void                  ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
		virtual void                  ClientCmd(const char* szCmdString) = 0;
		virtual bool                  GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;
		virtual int                   GetPlayerForUserID(int userID) = 0;
		virtual void* TextMessageGet(const char* pName) = 0; // 10
		virtual bool                  Con_IsVisible(void) = 0;
		virtual int                   GetLocalPlayer(void) = 0;
		virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;
		virtual float                 GetLastTimeStamp(void) = 0;
		virtual void* GetSentence(void* pAudioSource) = 0; // 15
		virtual float                 GetSentenceLength(void* pAudioSource) = 0;
		virtual bool                  IsStreaming(void* pAudioSource) const = 0;
		virtual void                  GetViewAngles(QAngle* va) = 0;
		virtual void                  SetViewAngles(QAngle* va) = 0;
		virtual int                   GetMaxClients(void) = 0; // 20
		virtual const char* Key_LookupBinding(const char* pBinding) = 0;
		virtual const char* Key_BindingForKey(int& code) = 0;
		virtual void                  Key_SetBinding(int, char const*) = 0;
		virtual void                  StartKeyTrapMode(void) = 0;
		virtual bool                  CheckDoneKeyTrapping(int& code) = 0;
		virtual bool                  IsInGame(void) = 0;
		virtual bool                  IsConnected(void) = 0;
		virtual bool                  IsDrawingLoadingImage(void) = 0;
		virtual void                  HideLoadingPlaque(void) = 0;
		virtual void                  Con_NPrintf(int pos, const char* fmt, ...) = 0; // 30
		virtual void                  Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
		virtual int                   IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
		virtual int                   IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
		virtual bool                  CullBox(const Vector& mins, const Vector& maxs) = 0;
		virtual void                  Sound_ExtraUpdate(void) = 0;
		virtual const char* GetGameDirectory(void) = 0;
		virtual const VMatrix* WorldToScreenMatrix() = 0;
		virtual const VMatrix& WorldToViewMatrix() = 0;
		// This version does NOT check against FCVAR_CLIENTCMD_CAN_EXECUTE.
		void ClientCmd_Unrestricted(const char* command, const bool delayed = false)
		{
			return GetVirtualFunction<void(__thiscall*)(IVEngineClient*, const char*, bool)>(this, 114)(this, command, delayed);
		}
	};

	class IInputSystem
	{
	public:

		void EnableInput(bool enable)
		{
			return GetVirtualFunction<void(__thiscall*)(IInputSystem*, bool)>(this, 11)(this, enable);
		}

		void resetInputState()
		{
			return GetVirtualFunction<void(__thiscall*)(IInputSystem*)>(this, 39)(this);
		}
	};

};