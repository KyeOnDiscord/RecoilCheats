#include "pch.h"
#include "hook.h"


namespace Hook
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size)
	{
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	bool Detour32(BYTE* src, BYTE* dst, const intptr_t len)
	{

		if (len < 5) return false;

		DWORD curProtection;
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);
		memset(src, 0x90, len);//Just incase, set the address we write to nop

		intptr_t relativeAddress = (intptr_t)(dst - (intptr_t)src) - 5;//Where our function is


		//Better to do it like this so we write the jmp & address at the same time (less room for error)
		BYTE instructions[5] = {};
		*(BYTE*)((intptr_t)instructions) = (char)'\xE9';
		*(intptr_t*)((intptr_t)instructions + 1) = relativeAddress;
		memcpy(src, instructions, len);

		VirtualProtect(src, len, curProtection, &curProtection);
		return true;
	}

	BYTE* TrampHook32(BYTE* src, BYTE* dst, const intptr_t len)
	{
		// Make sure the length is greater than 5
		if (len < 5) return 0;

		// Create the gateway (len + 5 for the overwritten bytes + the jmp)
		BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		//Write the stolen bytes into the gateway
		if (gateway)
		{
			memcpy(gateway, src, len);

			// Get the gateway to destination addy
			intptr_t gatewayRelativeAddr = ((intptr_t)src - (intptr_t)gateway) - 5;

			// Add the jmp opcode to the end of the gateway
			*(char*)((intptr_t)gateway + len) = (char)0xE9;

			// Add the address to the jmp
			*(intptr_t*)((intptr_t)gateway + len + 1) = gatewayRelativeAddr;

			// Perform the detour
			Detour32(src, dst, len);

			return gateway;
		}
		return NULL;
	}

	void Hook::EnableHook()
	{
		if (this->bStatus == false)
		{
			memcpy(this->originalBytes, this->src, this->len);
			*(uintptr_t*)this->PtrToGatewayFnPtr = (uintptr_t)TrampHook32(this->src, this->dst, this->len);
			this->bStatus = true;
		}
	}

	void Hook::DisableHook()
	{
		if (this->bStatus == true)
		{
			Patch(this->src, this->originalBytes, this->len);
			this->bStatus = false;
		}
	}

	Hook::Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		this->src = src;
		this->dst = dst;
		this->len = len;
		this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
	}

	Hook::Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		HMODULE hMod = GetModuleHandleA(modName);
		if (hMod)
		{
			this->src = (BYTE*)GetProcAddress(hMod, exportName);
			this->dst = dst;
			this->len = len;
			this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
		}
		else
		{
			throw std::invalid_argument("Could not find module");
		}
	}
}
