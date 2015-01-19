

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 19 18:49:47 2015
 */
/* Compiler settings for C:\Users\PROGMAN\workspace\bochs_trunk\vs2013\\..\bx_debug_rpc\BochsRpcDbg.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __BochsRpcDbg_h__
#define __BochsRpcDbg_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __BochsDebug_INTERFACE_DEFINED__
#define __BochsDebug_INTERFACE_DEFINED__

/* interface BochsDebug */
/* [version][uuid] */ 

typedef struct _BochsGPRsContext
    {
    unsigned __int64 RAX;
    unsigned __int64 RCX;
    unsigned __int64 RDX;
    unsigned __int64 RBX;
    unsigned __int64 RSP;
    unsigned __int64 RBP;
    unsigned __int64 RSI;
    unsigned __int64 RDI;
    unsigned __int64 R8;
    unsigned __int64 R9;
    unsigned __int64 R10;
    unsigned __int64 R11;
    unsigned __int64 R12;
    unsigned __int64 R13;
    unsigned __int64 R14;
    unsigned __int64 R15;
    unsigned __int64 RIP;
    unsigned __int32 EFLAGS;
    } 	BochsGPRsContext;

typedef struct _BochsGPRsContext *PBochsGPRsContext;

void Bochs_PauseSimulation( 
    /* [in] */ handle_t h1);

void Bochs_ResumeSimulation( 
    /* [in] */ handle_t h1);

void Bochs_QuitSimulation( 
    /* [in] */ handle_t h1);

boolean Bochs_ReadLinearMemory( 
    /* [in] */ handle_t h1,
    /* [in] */ unsigned __int64 StartAddr,
    /* [in] */ unsigned __int32 Length,
    /* [size_is][ref][out] */ byte Buffer[  ]);

boolean Bochs_ReadPhysicalMemory( 
    /* [in] */ handle_t h1,
    /* [in] */ unsigned __int64 StartAddr,
    /* [in] */ unsigned __int32 Length,
    /* [size_is][ref][out] */ byte Buffer[  ]);

void Bochs_WaitForIdle( 
    /* [in] */ handle_t h1);

void Bochs_PrintRegs( 
    /* [in] */ handle_t h1);

void Bochs_GetGPRs( 
    /* [in] */ handle_t h1,
    /* [ref][out] */ PBochsGPRsContext context);

/* [unique][string] */ char *Bochs_GetVersion( 
    /* [in] */ handle_t h1);



extern RPC_IF_HANDLE BochsDebug_v1_0_c_ifspec;
extern RPC_IF_HANDLE BochsDebug_v1_0_s_ifspec;
#endif /* __BochsDebug_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


