#pragma once

#include <Windows.h>
#include <DbgEng.h>
#include <atlcomcli.h>

class Debugger : public IDebugEventCallbacks, public IDebugOutputCallbacks {
public:
	HRESULT Init();
	HRESULT AttachToProcess(DWORD pid);
	HRESULT WaitForEvent();
	bool IsInBreakpoint() const;
	HRESULT ExecuteCommand(PCSTR cmd);
	void DisplayPrompt();
	ULONG GetState() const;
	void DisplayState();
	void Break();

private:
	HRESULT __stdcall QueryInterface(REFIID InterfaceId, PVOID* Interface) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;
	HRESULT __stdcall GetInterestMask(PULONG Mask) override;
	HRESULT __stdcall Breakpoint(PDEBUG_BREAKPOINT Bp) override;
	HRESULT __stdcall Exception(PEXCEPTION_RECORD64 Exception, ULONG FirstChance) override;
	HRESULT __stdcall CreateThread(ULONG64 Handle, ULONG64 DataOffset, ULONG64 StartOffset) override;
	HRESULT __stdcall ExitThread(ULONG ExitCode) override;
	HRESULT __stdcall ExitProcess(ULONG ExitCode) override;
	HRESULT __stdcall LoadModule(ULONG64 ImageFileHandle, ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, PCSTR ImageName, ULONG CheckSum, ULONG TimeDateStamp) override;
	HRESULT __stdcall UnloadModule(PCSTR ImageBaseName, ULONG64 BaseOffset) override;
	HRESULT __stdcall SystemError(ULONG Error, ULONG Level) override;
	HRESULT __stdcall SessionStatus(ULONG Status) override;
	HRESULT __stdcall ChangeDebuggeeState(ULONG Flags, ULONG64 Argument) override;
	HRESULT __stdcall ChangeEngineState(ULONG Flags, ULONG64 Argument) override;
	HRESULT __stdcall ChangeSymbolState(ULONG Flags, ULONG64 Argument) override;
	HRESULT __stdcall Output(ULONG Mask, PCSTR Text) override;
	HRESULT __stdcall CreateProcessW(ULONG64 ImageFileHandle, ULONG64 Handle, ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, 
		PCSTR ImageName, ULONG CheckSum, ULONG TimeDateStamp, ULONG64 InitialThreadHandle, ULONG64 ThreadDataOffset, ULONG64 StartOffset)override;

private:
	CComPtr<IDebugClient6> m_Client;
	CComPtr<IDebugControl3> m_Control;
	bool m_Breakpoint{ false };
};

