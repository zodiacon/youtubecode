#include "Debugger.h"
#include <stdio.h>

#define HR(x) { auto hr = (x); if(FAILED(hr)) return hr; }

#pragma comment(lib, "dbgeng")

HRESULT Debugger::Init() {
	HR(DebugCreate(__uuidof(IDebugClient6), (void**)&m_Client));
	HR(m_Client.QueryInterface(&m_Control));

	m_Client->SetEventCallbacks(this);
	m_Client->SetOutputCallbacks(this);

	return S_OK;
}

HRESULT Debugger::AttachToProcess(DWORD pid) {
	m_Control->AddEngineOptions(DEBUG_ENGOPT_INITIAL_BREAK);
	HR(m_Client->AttachProcess(0, pid, DEBUG_ATTACH_DEFAULT));
	return WaitForEvent();
}

HRESULT Debugger::WaitForEvent() {
	return m_Control->WaitForEvent(0, INFINITE);
}

bool Debugger::IsInBreakpoint() const {
	return m_Breakpoint;
}

HRESULT __stdcall Debugger::QueryInterface(REFIID riid, PVOID* ppv) {
	if (riid == __uuidof(IUnknown) || riid == __uuidof(IDebugEventCallbacks)) {
		*ppv = static_cast<IDebugEventCallbacks*>(this);
		AddRef();
		return S_OK;
	}
	if (riid == __uuidof(IDebugOutputCallbacks)) {
		*ppv = static_cast<IDebugOutputCallbacks*>(this);
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG __stdcall Debugger::AddRef(void) {
	return 2;
}

ULONG __stdcall Debugger::Release(void) {
	return 1;
}

HRESULT __stdcall Debugger::GetInterestMask(PULONG Mask) {
	*Mask = DEBUG_EVENT_BREAKPOINT | DEBUG_EVENT_CREATE_PROCESS |
		DEBUG_EVENT_CREATE_THREAD | DEBUG_EVENT_LOAD_MODULE | DEBUG_EVENT_EXCEPTION;

	return S_OK;
}

HRESULT __stdcall Debugger::Breakpoint(PDEBUG_BREAKPOINT Bp) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::Exception(PEXCEPTION_RECORD64 Exception, ULONG FirstChance) {
	m_Breakpoint = Exception->ExceptionCode == 0x80000003 ||
		Exception->ExceptionCode == 0x80000004;

	return m_Breakpoint ? DEBUG_STATUS_BREAK : DEBUG_STATUS_NO_CHANGE;
}

HRESULT __stdcall Debugger::CreateThread(ULONG64 Handle, ULONG64 DataOffset, ULONG64 StartOffset) {
	return S_OK;
}

HRESULT __stdcall Debugger::ExitThread(ULONG ExitCode) {
	return S_OK;
}

HRESULT __stdcall Debugger::ExitProcess(ULONG ExitCode) {
	return S_OK;
}

HRESULT __stdcall Debugger::LoadModule(ULONG64 ImageFileHandle, ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, PCSTR ImageName, ULONG CheckSum, ULONG TimeDateStamp) {
	printf("Module loaded: %s at 0x%llX\n", ImageName, BaseOffset);

	return S_OK;
}

HRESULT __stdcall Debugger::UnloadModule(PCSTR ImageBaseName, ULONG64 BaseOffset) {
	return S_OK;
}

HRESULT __stdcall Debugger::SystemError(ULONG Error, ULONG Level) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::SessionStatus(ULONG Status) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::ChangeDebuggeeState(ULONG Flags, ULONG64 Argument) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::ChangeEngineState(ULONG Flags, ULONG64 Argument) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::ChangeSymbolState(ULONG Flags, ULONG64 Argument) {
	return E_NOTIMPL;
}

HRESULT __stdcall Debugger::Output(ULONG Mask, PCSTR Text) {
	printf("%s", Text);
	return S_OK;
}

HRESULT Debugger::CreateProcessW(ULONG64 ImageFileHandle, ULONG64 Handle, ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, PCSTR ImageName, ULONG CheckSum, ULONG TimeDateStamp, ULONG64 InitialThreadHandle, ULONG64 ThreadDataOffset, ULONG64 StartOffset) noexcept {
	return S_OK;
}

