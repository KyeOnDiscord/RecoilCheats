#pragma once
#include <Windows.h>
#include <stdexcept>

namespace Hook
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);

	bool Detour32(BYTE* src, BYTE* dst, const intptr_t len);

	BYTE* TrampHook32(BYTE* src, BYTE* dst, const intptr_t len);

	class Hook
	{
	public:
		bool bStatus{ false };

		BYTE* src{ nullptr };
		BYTE* dst{ nullptr };
		BYTE* PtrToGatewayFnPtr{ nullptr };
		uintptr_t len{ 0 };

		BYTE originalBytes[10]{ 0 };

		void EnableHook();
		void DisableHook();

		Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len);
		Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len);
		Hook() {};
	};
}