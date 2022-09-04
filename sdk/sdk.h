#pragma once
#include "netvars.h"

typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);





namespace sdk
{

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
		virtual const VMatrix& WorldToScreenMatrix() = 0;
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