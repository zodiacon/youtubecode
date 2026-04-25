

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Mon Jan 18 22:14:07 2038
 */
/* Compiler settings for wslservice.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wslservice_h__
#define __wslservice_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __ILxssUserSession_FWD_DEFINED__
#define __ILxssUserSession_FWD_DEFINED__
typedef interface ILxssUserSession ILxssUserSession;

#endif 	/* __ILxssUserSession_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wslservice_0000_0000 */
/* [local] */ 

#define LXSS_INBOX_SERVICE_NAME L"lxssmanager"
#define LXSS_LEGACY_INSTALL_VALUE L"State"
#define LXSS_LEGACY_INSTALL_NAME L"Legacy"
const GUID LXSS_LEGACY_DISTRO_GUID = {0x12345678, 0x1234, 0x5678, {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}};
const GUID WSL2_SYSTEM_DISTRO_GUID = {0x20170608, 0x0542, 0x2019, {0x07, 0x29, 0x00, 0x11, 0x20, 0x12, 0x02, 0x14}};
typedef 
enum _LxssDistributionState
    {
        LxssDistributionStateInvalid	= 0,
        LxssDistributionStateInstalled	= ( LxssDistributionStateInvalid + 1 ) ,
        LxssDistributionStateRunning	= ( LxssDistributionStateInstalled + 1 ) ,
        LxssDistributionStateInstalling	= ( LxssDistributionStateRunning + 1 ) ,
        LxssDistributionStateUninstalling	= ( LxssDistributionStateInstalling + 1 ) ,
        LxssDistributionStateConverting	= ( LxssDistributionStateUninstalling + 1 ) ,
        LxssDistributionStateExporting	= ( LxssDistributionStateConverting + 1 ) 
    } 	LxssDistributionState;

typedef 
enum _LxssHandleType
    {
        LxssHandleConsole	= 0,
        LxssHandleInput	= ( LxssHandleConsole + 1 ) ,
        LxssHandleOutput	= ( LxssHandleInput + 1 ) 
    } 	LxssHandleType;

typedef 
enum _LxssExecutionContextFlags
    {
        LxssExecutionContextFlagsNone	= 0,
        LxssExecutionContextFlagsEnableContextualizedErrors	= 1,
        LxssExecutionContextFlagsEnableUserWarnings	= 2
    } 	LxssExecutionContextFlags;

#define LXSS_HANDLE_USE_CONSOLE 0
typedef struct _LXSS_HANDLE
    {
    ULONG Handle;
    LxssHandleType HandleType;
    } 	LXSS_HANDLE;

typedef struct _LXSS_HANDLE *PLXSS_HANDLE;

typedef struct _LXSS_STD_HANDLES
    {
    LXSS_HANDLE StdIn;
    LXSS_HANDLE StdOut;
    LXSS_HANDLE StdErr;
    } 	LXSS_STD_HANDLES;

typedef struct _LXSS_STD_HANDLES *PLXSS_STD_HANDLES;

typedef struct _LXSS_ENUMERATE_INFO
    {
    GUID DistroGuid;
    LxssDistributionState State;
    ULONG Version;
    ULONG Flags;
    WCHAR DistroName[ 257 ];
    } 	LXSS_ENUMERATE_INFO;

typedef struct _LXSS_ENUMERATE_INFO *PLXSS_ENUMERATE_INFO;

typedef struct _LXSS_ERROR_INFO
    {
    ULONG Flags;
    ULONGLONG Context;
    /* [string] */ LPWSTR Message;
    /* [string] */ LPWSTR Warnings;
    ULONG WarningsPipe;
    } 	LXSS_ERROR_INFO;

typedef struct _LXSS_ERROR_INFO *PLXSS_ERROR_INFO;

#define LXSS_DISTRO_VERSION_LEGACY 0
#define LXSS_DISTRO_VERSION_1 1
#define LXSS_DISTRO_VERSION_2 2
#define LXSS_DISTRO_VERSION_CURRENT LXSS_DISTRO_VERSION_2
#define LXSS_DISTRO_USES_WSL_FS(DistroVersion) (DistroVersion >= LXSS_DISTRO_VERSION_2)
#define LXSS_DISTRO_DEFAULT_ENVIRONMENT "HOSTTYPE=" DISTRO_HOSTTYPE "\0LANG=en_US.UTF-8\0PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games\0TERM=xterm-256color\0"
#define LXSS_DISTRO_DEFAULT_KERNEL_COMMAND_LINE "BOOT_IMAGE=/kernel init=/init"
#define LXSS_DISTRO_FLAGS_ENABLE_INTEROP 0x1
#define LXSS_DISTRO_FLAGS_APPEND_NT_PATH 0x2
#define LXSS_DISTRO_FLAGS_ENABLE_DRIVE_MOUNTING 0x4
#define LXSS_DISTRO_FLAGS_VM_MODE 0x8
#define LXSS_DISTRO_FLAGS_UNCHANGED 0xFFFFFFFE
#define LXSS_DISTRO_FLAGS_ALL (LXSS_DISTRO_FLAGS_ENABLE_INTEROP | LXSS_DISTRO_FLAGS_APPEND_NT_PATH | LXSS_DISTRO_FLAGS_ENABLE_DRIVE_MOUNTING | LXSS_DISTRO_FLAGS_VM_MODE)
#define LXSS_DISTRO_FLAGS_DEFAULT (LXSS_DISTRO_FLAGS_ENABLE_INTEROP | LXSS_DISTRO_FLAGS_APPEND_NT_PATH | LXSS_DISTRO_FLAGS_ENABLE_DRIVE_MOUNTING)
#define LXSS_ENUMERATE_FLAGS_DEFAULT 0x1
#define LXSS_GET_DISTRO_ID_LIST_ALL 0x1
#define LXSS_CREATE_INSTANCE_FLAGS_ALLOW_FS_UPGRADE 0x1
#define LXSS_CREATE_INSTANCE_FLAGS_OPEN_EXISTING 0x2
#define LXSS_CREATE_INSTANCE_FLAGS_IGNORE_CLIENT 0x4
#define LXSS_CREATE_INSTANCE_FLAGS_USE_SYSTEM_DISTRO 0x8
#define LXSS_CREATE_INSTANCE_FLAGS_SHELL_LOGIN 0x10
#define LXSS_CREATE_INSTANCE_FLAGS_ALL (LXSS_CREATE_INSTANCE_FLAGS_ALLOW_FS_UPGRADE | LXSS_CREATE_INSTANCE_FLAGS_OPEN_EXISTING | LXSS_CREATE_INSTANCE_FLAGS_IGNORE_CLIENT | LXSS_CREATE_INSTANCE_FLAGS_USE_SYSTEM_DISTRO | LXSS_CREATE_INSTANCE_FLAGS_SHELL_LOGIN)
#define LXSS_EXPORT_DISTRO_FLAGS_VHD 0x1
#define LXSS_EXPORT_DISTRO_FLAGS_GZIP 0x2
#define LXSS_EXPORT_DISTRO_FLAGS_XZIP 0x4
#define LXSS_EXPORT_DISTRO_FLAGS_VERBOSE 0x8
#define LXSS_EXPORT_DISTRO_FLAGS_ALL (LXSS_EXPORT_DISTRO_FLAGS_VHD | LXSS_EXPORT_DISTRO_FLAGS_GZIP | LXSS_EXPORT_DISTRO_FLAGS_XZIP | LXSS_EXPORT_DISTRO_FLAGS_VERBOSE)
#define LXSS_IMPORT_DISTRO_FLAGS_VHD 0x1
#define LXSS_IMPORT_DISTRO_FLAGS_CREATE_SHORTCUT 0x2
#define LXSS_IMPORT_DISTRO_FLAGS_NO_OOBE 0x4
#define LXSS_IMPORT_DISTRO_FLAGS_FIXED_VHD 0x8
#define LXSS_IMPORT_DISTRO_FLAGS_ALL (LXSS_IMPORT_DISTRO_FLAGS_VHD | LXSS_IMPORT_DISTRO_FLAGS_CREATE_SHORTCUT | LXSS_IMPORT_DISTRO_FLAGS_NO_OOBE | LXSS_IMPORT_DISTRO_FLAGS_FIXED_VHD)
#define LXSS_ATTACH_MOUNT_FLAGS_PASS_THROUGH 0x1
#define LXSS_ATTACH_MOUNT_FLAGS_VHD 0x2
#define LXSS_PLAN9_UNIX_SOCKET_A "fsserver"
#define LXSS_PLAN9_UNIX_SOCKET L"fsserver"
#define LXSS_VM_MODE_INITRD_NAME L"initrd.img"
#define LXSS_VM_MODE_KERNEL_NAME L"kernel"
#define LXSS_VM_MODE_VHD_NAME L"ext4.vhdx"
#define LXSS_TOOLS_DIRECTORY L"tools"
#define LXSS_REGISTRY_PATH L"Software\\Microsoft\\Windows\\CurrentVersion\\Lxss"
#define LXSS_DISK_MOUNTS_REGISTRY_PATH LXSS_REGISTRY_PATH L"\\DiskMounts"
#define LXSS_WSL_DEFAULT_VERSION L"DefaultVersion"
#define LXSS_NOTIFICATIONS_KEY L"Notifications"
#define LXSS_NOTIFICATION_DRVFS_PERF_DISABLED L"DrvFsPerfDisabled"
#define LXSS_WSL_VERSION_DEFAULT 0
#define LXSS_WSL_VERSION_1 1
#define LXSS_WSL_VERSION_2 2
#define WSL_DISTRO_NAME L"DistributionName"
#define LXSS_OOBE_COMPLETE_NAME L"OOBEComplete"
const GUID CLSID_LxssUserSession = {0xa9b7a1b9, 0x0671, 0x405c, {0x95, 0xf1, 0xe0, 0x61, 0x2c, 0xb4, 0xce, 0x7e}};
const GUID CLSID_LxssUserSessionInBox = {0x4f476546, 0xb412, 0x4579, {0xb6, 0x4c, 0x12, 0x3d, 0xf3, 0x31, 0xe3, 0xd6}};
#ifdef __cplusplus
class DECLSPEC_UUID("a9b7a1b9-0671-405c-95f1-e0612cb4ce7e") LxssUserSession;
class DECLSPEC_UUID("4f476546-b412-4579-b64c-123df331e3d6") LxssUserSessionInBox;
#endif


extern RPC_IF_HANDLE __MIDL_itf_wslservice_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wslservice_0000_0000_v0_0_s_ifspec;

#ifndef __ILxssUserSession_INTERFACE_DEFINED__
#define __ILxssUserSession_INTERFACE_DEFINED__

/* interface ILxssUserSession */
/* [object][unique][uuid] */ 


EXTERN_C const IID IID_ILxssUserSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("38541BDC-F54F-4CEB-85D0-37F0F3D2617E")
    ILxssUserSession : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterDistribution( 
            /* [unique][in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE FileHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [unique][in] */ LPCWSTR TargetDirectory,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG64 VhdSize,
            /* [unique][in] */ LPCWSTR PackageFamilyName,
            /* [out] */ LPWSTR *InstalledDistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterDistributionPipe( 
            /* [unique][in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE PipeHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [unique][in] */ LPCWSTR TargetDirectory,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG64 VhdSize,
            /* [unique][in] */ LPCWSTR PackageFamilyName,
            /* [out] */ LPWSTR *InstalledDistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDistributionId( 
            /* [in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TerminateDistribution( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterDistribution( 
            /* [in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfigureDistribution( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG DefaultUid,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDistributionConfiguration( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [out] */ LPWSTR *DistributionName,
            /* [out] */ ULONG *Version,
            /* [out] */ ULONG *DefaultUid,
            /* [out] */ ULONG *DefaultEnvironmentCount,
            /* [size_is][size_is][out] */ LPSTR **DefaultEnvironment,
            /* [out] */ ULONG *Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultDistribution( 
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ LPGUID pResultState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResizeDistribution( 
            /* [in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE OutputHandle,
            /* [in] */ ULONG64 NewSize,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDefaultDistribution( 
            /* [in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSparse( 
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ BOOLEAN Sparse,
            /* [in] */ BOOLEAN AllowUnsafe,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumerateDistributions( 
            /* [out] */ ULONG *DistributionCount,
            /* [size_is][size_is][out] */ LXSS_ENUMERATE_INFO **Distributions,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateLxProcess( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [unique][in] */ LPCSTR Filename,
            /* [in] */ ULONG CommandLineCount,
            /* [size_is][unique][in] */ LPCSTR *CommandLine,
            /* [unique][in] */ LPCWSTR CurrentWorkingDirectory,
            /* [unique][in] */ LPCWSTR NtPath,
            /* [size_is][unique][in] */ WCHAR *NtEnvironment,
            /* [in] */ ULONG NtEnvironmentLength,
            /* [unique][in] */ LPCWSTR Username,
            /* [in] */ SHORT Columns,
            /* [in] */ SHORT Rows,
            /* [in] */ ULONG ConsoleHandle,
            /* [in] */ PLXSS_STD_HANDLES StdHandles,
            /* [in] */ ULONG Flags,
            /* [out] */ GUID *DistributionId,
            /* [out] */ GUID *InstanceId,
            /* [system_handle][out] */ HANDLE *ProcessHandle,
            /* [system_handle][out] */ HANDLE *ServerHandle,
            /* [system_handle][out] */ HANDLE *StandardIn,
            /* [system_handle][out] */ HANDLE *StandardOut,
            /* [system_handle][out] */ HANDLE *StandardErr,
            /* [system_handle][out] */ HANDLE *CommunicationChannel,
            /* [system_handle][out] */ HANDLE *InteropSocket,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVersion( 
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExportDistribution( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE FileHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExportDistributionPipe( 
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE PipeHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AttachDisk( 
            /* [in] */ LPCWSTR Disk,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DetachDisk( 
            /* [unique][in] */ LPCWSTR Disk,
            /* [out] */ int *Result,
            /* [out] */ int *Step,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MountDisk( 
            /* [in] */ LPCWSTR Disk,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG PartitionIndex,
            /* [unique][in] */ LPCWSTR Name,
            /* [unique][in] */ LPCWSTR Type,
            /* [unique][in] */ LPCWSTR Options,
            /* [out] */ int *Result,
            /* [out] */ int *Step,
            /* [out] */ LPWSTR *MountName,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( 
            /* [in] */ BOOL Force) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ImportDistributionInplace( 
            /* [in] */ LPCWSTR DistributionName,
            /* [in] */ LPCWSTR VhdPath,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveDistribution( 
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ LPCWSTR DistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILxssUserSessionVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILxssUserSession * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILxssUserSession * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILxssUserSession * This);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, CreateInstance)
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, RegisterDistribution)
        HRESULT ( STDMETHODCALLTYPE *RegisterDistribution )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE FileHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [unique][in] */ LPCWSTR TargetDirectory,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG64 VhdSize,
            /* [unique][in] */ LPCWSTR PackageFamilyName,
            /* [out] */ LPWSTR *InstalledDistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, RegisterDistributionPipe)
        HRESULT ( STDMETHODCALLTYPE *RegisterDistributionPipe )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE PipeHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [unique][in] */ LPCWSTR TargetDirectory,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG64 VhdSize,
            /* [unique][in] */ LPCWSTR PackageFamilyName,
            /* [out] */ LPWSTR *InstalledDistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, GetDistributionId)
        HRESULT ( STDMETHODCALLTYPE *GetDistributionId )( 
            ILxssUserSession * This,
            /* [in] */ LPCWSTR DistributionName,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, TerminateDistribution)
        HRESULT ( STDMETHODCALLTYPE *TerminateDistribution )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, UnregisterDistribution)
        HRESULT ( STDMETHODCALLTYPE *UnregisterDistribution )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, ConfigureDistribution)
        HRESULT ( STDMETHODCALLTYPE *ConfigureDistribution )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG DefaultUid,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, GetDistributionConfiguration)
        HRESULT ( STDMETHODCALLTYPE *GetDistributionConfiguration )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [out] */ LPWSTR *DistributionName,
            /* [out] */ ULONG *Version,
            /* [out] */ ULONG *DefaultUid,
            /* [out] */ ULONG *DefaultEnvironmentCount,
            /* [size_is][size_is][out] */ LPSTR **DefaultEnvironment,
            /* [out] */ ULONG *Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, GetDefaultDistribution)
        HRESULT ( STDMETHODCALLTYPE *GetDefaultDistribution )( 
            ILxssUserSession * This,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ LPGUID pResultState);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, ResizeDistribution)
        HRESULT ( STDMETHODCALLTYPE *ResizeDistribution )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE OutputHandle,
            /* [in] */ ULONG64 NewSize,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, SetDefaultDistribution)
        HRESULT ( STDMETHODCALLTYPE *SetDefaultDistribution )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, SetSparse)
        HRESULT ( STDMETHODCALLTYPE *SetSparse )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ BOOLEAN Sparse,
            /* [in] */ BOOLEAN AllowUnsafe,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, EnumerateDistributions)
        HRESULT ( STDMETHODCALLTYPE *EnumerateDistributions )( 
            ILxssUserSession * This,
            /* [out] */ ULONG *DistributionCount,
            /* [size_is][size_is][out] */ LXSS_ENUMERATE_INFO **Distributions,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, CreateLxProcess)
        HRESULT ( STDMETHODCALLTYPE *CreateLxProcess )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [unique][in] */ LPCSTR Filename,
            /* [in] */ ULONG CommandLineCount,
            /* [size_is][unique][in] */ LPCSTR *CommandLine,
            /* [unique][in] */ LPCWSTR CurrentWorkingDirectory,
            /* [unique][in] */ LPCWSTR NtPath,
            /* [size_is][unique][in] */ WCHAR *NtEnvironment,
            /* [in] */ ULONG NtEnvironmentLength,
            /* [unique][in] */ LPCWSTR Username,
            /* [in] */ SHORT Columns,
            /* [in] */ SHORT Rows,
            /* [in] */ ULONG ConsoleHandle,
            /* [in] */ PLXSS_STD_HANDLES StdHandles,
            /* [in] */ ULONG Flags,
            /* [out] */ GUID *DistributionId,
            /* [out] */ GUID *InstanceId,
            /* [system_handle][out] */ HANDLE *ProcessHandle,
            /* [system_handle][out] */ HANDLE *ServerHandle,
            /* [system_handle][out] */ HANDLE *StandardIn,
            /* [system_handle][out] */ HANDLE *StandardOut,
            /* [system_handle][out] */ HANDLE *StandardErr,
            /* [system_handle][out] */ HANDLE *CommunicationChannel,
            /* [system_handle][out] */ HANDLE *InteropSocket,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, SetVersion)
        HRESULT ( STDMETHODCALLTYPE *SetVersion )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ ULONG Version,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, ExportDistribution)
        HRESULT ( STDMETHODCALLTYPE *ExportDistribution )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE FileHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, ExportDistributionPipe)
        HRESULT ( STDMETHODCALLTYPE *ExportDistributionPipe )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCGUID DistroGuid,
            /* [system_handle][in] */ HANDLE PipeHandle,
            /* [system_handle][in] */ HANDLE StderrHandle,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, AttachDisk)
        HRESULT ( STDMETHODCALLTYPE *AttachDisk )( 
            ILxssUserSession * This,
            /* [in] */ LPCWSTR Disk,
            /* [in] */ ULONG Flags,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, DetachDisk)
        HRESULT ( STDMETHODCALLTYPE *DetachDisk )( 
            ILxssUserSession * This,
            /* [unique][in] */ LPCWSTR Disk,
            /* [out] */ int *Result,
            /* [out] */ int *Step,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, MountDisk)
        HRESULT ( STDMETHODCALLTYPE *MountDisk )( 
            ILxssUserSession * This,
            /* [in] */ LPCWSTR Disk,
            /* [in] */ ULONG Flags,
            /* [in] */ ULONG PartitionIndex,
            /* [unique][in] */ LPCWSTR Name,
            /* [unique][in] */ LPCWSTR Type,
            /* [unique][in] */ LPCWSTR Options,
            /* [out] */ int *Result,
            /* [out] */ int *Step,
            /* [out] */ LPWSTR *MountName,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, Shutdown)
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            ILxssUserSession * This,
            /* [in] */ BOOL Force);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, ImportDistributionInplace)
        HRESULT ( STDMETHODCALLTYPE *ImportDistributionInplace )( 
            ILxssUserSession * This,
            /* [in] */ LPCWSTR DistributionName,
            /* [in] */ LPCWSTR VhdPath,
            /* [out][in] */ LXSS_ERROR_INFO *Error,
            /* [retval][out] */ GUID *pDistroGuid);
        
        DECLSPEC_XFGVIRT(ILxssUserSession, MoveDistribution)
        HRESULT ( STDMETHODCALLTYPE *MoveDistribution )( 
            ILxssUserSession * This,
            /* [in] */ LPCGUID DistroGuid,
            /* [in] */ LPCWSTR DistributionName,
            /* [out][in] */ LXSS_ERROR_INFO *Error);
        
        END_INTERFACE
    } ILxssUserSessionVtbl;

    interface ILxssUserSession
    {
        CONST_VTBL struct ILxssUserSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILxssUserSession_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILxssUserSession_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILxssUserSession_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILxssUserSession_CreateInstance(This,DistroGuid,Flags,Error)	\
    ( (This)->lpVtbl -> CreateInstance(This,DistroGuid,Flags,Error) ) 

#define ILxssUserSession_RegisterDistribution(This,DistributionName,Version,FileHandle,StderrHandle,TargetDirectory,Flags,VhdSize,PackageFamilyName,InstalledDistributionName,Error,pDistroGuid)	\
    ( (This)->lpVtbl -> RegisterDistribution(This,DistributionName,Version,FileHandle,StderrHandle,TargetDirectory,Flags,VhdSize,PackageFamilyName,InstalledDistributionName,Error,pDistroGuid) ) 

#define ILxssUserSession_RegisterDistributionPipe(This,DistributionName,Version,PipeHandle,StderrHandle,TargetDirectory,Flags,VhdSize,PackageFamilyName,InstalledDistributionName,Error,pDistroGuid)	\
    ( (This)->lpVtbl -> RegisterDistributionPipe(This,DistributionName,Version,PipeHandle,StderrHandle,TargetDirectory,Flags,VhdSize,PackageFamilyName,InstalledDistributionName,Error,pDistroGuid) ) 

#define ILxssUserSession_GetDistributionId(This,DistributionName,Flags,Error,pDistroGuid)	\
    ( (This)->lpVtbl -> GetDistributionId(This,DistributionName,Flags,Error,pDistroGuid) ) 

#define ILxssUserSession_TerminateDistribution(This,DistroGuid,Error)	\
    ( (This)->lpVtbl -> TerminateDistribution(This,DistroGuid,Error) ) 

#define ILxssUserSession_UnregisterDistribution(This,DistroGuid,Error)	\
    ( (This)->lpVtbl -> UnregisterDistribution(This,DistroGuid,Error) ) 

#define ILxssUserSession_ConfigureDistribution(This,DistroGuid,DefaultUid,Flags,Error)	\
    ( (This)->lpVtbl -> ConfigureDistribution(This,DistroGuid,DefaultUid,Flags,Error) ) 

#define ILxssUserSession_GetDistributionConfiguration(This,DistroGuid,DistributionName,Version,DefaultUid,DefaultEnvironmentCount,DefaultEnvironment,Flags,Error)	\
    ( (This)->lpVtbl -> GetDistributionConfiguration(This,DistroGuid,DistributionName,Version,DefaultUid,DefaultEnvironmentCount,DefaultEnvironment,Flags,Error) ) 

#define ILxssUserSession_GetDefaultDistribution(This,Error,pResultState)	\
    ( (This)->lpVtbl -> GetDefaultDistribution(This,Error,pResultState) ) 

#define ILxssUserSession_ResizeDistribution(This,DistroGuid,OutputHandle,NewSize,Error)	\
    ( (This)->lpVtbl -> ResizeDistribution(This,DistroGuid,OutputHandle,NewSize,Error) ) 

#define ILxssUserSession_SetDefaultDistribution(This,DistroGuid,Error)	\
    ( (This)->lpVtbl -> SetDefaultDistribution(This,DistroGuid,Error) ) 

#define ILxssUserSession_SetSparse(This,DistroGuid,Sparse,AllowUnsafe,Error)	\
    ( (This)->lpVtbl -> SetSparse(This,DistroGuid,Sparse,AllowUnsafe,Error) ) 

#define ILxssUserSession_EnumerateDistributions(This,DistributionCount,Distributions,Error)	\
    ( (This)->lpVtbl -> EnumerateDistributions(This,DistributionCount,Distributions,Error) ) 

#define ILxssUserSession_CreateLxProcess(This,DistroGuid,Filename,CommandLineCount,CommandLine,CurrentWorkingDirectory,NtPath,NtEnvironment,NtEnvironmentLength,Username,Columns,Rows,ConsoleHandle,StdHandles,Flags,DistributionId,InstanceId,ProcessHandle,ServerHandle,StandardIn,StandardOut,StandardErr,CommunicationChannel,InteropSocket,Error)	\
    ( (This)->lpVtbl -> CreateLxProcess(This,DistroGuid,Filename,CommandLineCount,CommandLine,CurrentWorkingDirectory,NtPath,NtEnvironment,NtEnvironmentLength,Username,Columns,Rows,ConsoleHandle,StdHandles,Flags,DistributionId,InstanceId,ProcessHandle,ServerHandle,StandardIn,StandardOut,StandardErr,CommunicationChannel,InteropSocket,Error) ) 

#define ILxssUserSession_SetVersion(This,DistroGuid,Version,StderrHandle,Error)	\
    ( (This)->lpVtbl -> SetVersion(This,DistroGuid,Version,StderrHandle,Error) ) 

#define ILxssUserSession_ExportDistribution(This,DistroGuid,FileHandle,StderrHandle,Flags,Error)	\
    ( (This)->lpVtbl -> ExportDistribution(This,DistroGuid,FileHandle,StderrHandle,Flags,Error) ) 

#define ILxssUserSession_ExportDistributionPipe(This,DistroGuid,PipeHandle,StderrHandle,Flags,Error)	\
    ( (This)->lpVtbl -> ExportDistributionPipe(This,DistroGuid,PipeHandle,StderrHandle,Flags,Error) ) 

#define ILxssUserSession_AttachDisk(This,Disk,Flags,Error)	\
    ( (This)->lpVtbl -> AttachDisk(This,Disk,Flags,Error) ) 

#define ILxssUserSession_DetachDisk(This,Disk,Result,Step,Error)	\
    ( (This)->lpVtbl -> DetachDisk(This,Disk,Result,Step,Error) ) 

#define ILxssUserSession_MountDisk(This,Disk,Flags,PartitionIndex,Name,Type,Options,Result,Step,MountName,Error)	\
    ( (This)->lpVtbl -> MountDisk(This,Disk,Flags,PartitionIndex,Name,Type,Options,Result,Step,MountName,Error) ) 

#define ILxssUserSession_Shutdown(This,Force)	\
    ( (This)->lpVtbl -> Shutdown(This,Force) ) 

#define ILxssUserSession_ImportDistributionInplace(This,DistributionName,VhdPath,Error,pDistroGuid)	\
    ( (This)->lpVtbl -> ImportDistributionInplace(This,DistributionName,VhdPath,Error,pDistroGuid) ) 

#define ILxssUserSession_MoveDistribution(This,DistroGuid,DistributionName,Error)	\
    ( (This)->lpVtbl -> MoveDistribution(This,DistroGuid,DistributionName,Error) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILxssUserSession_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wslservice_0000_0001 */
/* [local] */ 

// LXSS specific errors
#define WSL_E_BASE (0x0300)
#define WSL_E_DEFAULT_DISTRO_NOT_FOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 1) /* 0x80040301 */
#define WSL_E_DISTRO_NOT_FOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 2) /* 0x80040302 */
#define WSL_E_WSL1_NOT_SUPPORTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 3) /* 0x80040303 */
#define WSL_E_VM_MODE_NOT_SUPPORTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 4) /* 0x80040304 */
#define WSL_E_TOO_MANY_DISKS_ATTACHED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 5) /* 0x80040305 */
#define WSL_E_CONSOLE MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 6) /* 0x80040306 */
#define WSL_E_CUSTOM_KERNEL_NOT_FOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 7) /* 0x80040307 */
#define WSL_E_USER_NOT_FOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 8) /* 0x80040308 */
#define WSL_E_INVALID_USAGE MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 9) /* 0x80040309 */
#define WSL_E_EXPORT_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xa) /* 0x8004030a */
#define WSL_E_IMPORT_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xb) /* 0x8004030b */
#define WSL_E_DISTRO_NOT_STOPPED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xc) /* 0x8004030c */
#define WSL_E_TTY_LIMIT MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xd) /* 0x8004030d */
#define WSL_E_CUSTOM_SYSTEM_DISTRO_ERROR MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xe) /* 0x8004030e */
#define WSL_E_LOWER_INTEGRITY MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0xf) /* 0x8004030f */
#define WSL_E_HIGHER_INTEGRITY MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x10) /* 0x80040310 */
#define WSL_E_FS_UPGRADE_NEEDED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x11) /* 0x80040311 */
#define WSL_E_USER_VHD_ALREADY_ATTACHED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x12) /* 0x80040312 */
#define WSL_E_VM_MODE_INVALID_STATE MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x13) /* 0x80040313 */
#define WSL_E_VM_MODE_MOUNT_NAME_ALREADY_EXISTS MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x14) /* 0x80040314 */
#define WSL_E_ELEVATION_NEEDED_TO_MOUNT_DISK MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x15) /* 0x80040315 */
#define WSL_E_DISK_ALREADY_ATTACHED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x18) /* 0x80040318 */
#define WSL_E_DISK_ALREADY_MOUNTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x19) /* 0x80040319 */
#define WSL_E_DISK_MOUNT_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1A) /* 0x8004031A */
#define WSL_E_DISK_UNMOUNT_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1B) /* 0x8004031B */
#define WSL_E_WSL2_NEEDED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1C) /* 0x8004031C */
#define WSL_E_VM_MODE_INVALID_MOUNT_NAME MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1D) /* 0x8004031D */
#define WSL_E_GUI_APPLICATIONS_DISABLED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1E) /* 0x8004031E */
#define WSL_E_DISTRO_ONLY_AVAILABLE_FROM_STORE MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x1F) /* 0x8004031F */
#define WSL_E_WSL_MOUNT_NOT_SUPPORTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x20) /* 0x80040320 */
#define WSL_E_WSL_OPTIONAL_COMPONENT_REQUIRED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x21) /* 0x80040321 */
#define WSL_E_VMSWITCH_NOT_FOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x24) /* 0x80040324 */
#define WSL_E_VMSWITCH_NOT_SET MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x25) /* 0x80040325 */
#define WSL_E_NOT_A_LINUX_DISTRO MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x26) /* 0x80040326 */
#define WSL_E_OS_NOT_SUPPORTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x27) /* 0x80040327 */
#define WSL_E_INSTALL_PROCESS_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x28) /* 0x80040328 */
#define WSL_E_INSTALL_COMPONENT_FAILED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x29) /* 0x80040329 */
/* 0x8004032A is in use for WSL_E_PLUGIN_REQUIRES_UPDATE */
#define WSL_E_DISK_MOUNT_DISABLED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x2B) /* 0x8004032B */
#define WSL_E_WSL1_DISABLED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x2C) /* 0x8004032C */
#define WSL_E_VIRTUAL_MACHINE_PLATFORM_REQUIRED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x2D) /* 0x8004032D */
#define WSL_E_LOCAL_SYSTEM_NOT_SUPPORTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x2E) /* 0x8004032E */
#define WSL_E_DISK_CORRUPTED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x2F) /* 0x8004032F */
#define WSL_E_DISTRIBUTION_NAME_NEEDED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x30) /* 0x80040330 */
#define WSL_E_INVALID_JSON MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x31) /* 0x80040331 */
#define WSL_E_VM_CRASHED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, WSL_E_BASE + 0x32) /* 0x80040332 */


extern RPC_IF_HANDLE __MIDL_itf_wslservice_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wslservice_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


