

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Dec 25 09:27:30 2014
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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "BochsRpcDbg.h"

#define TYPE_FORMAT_STRING_SIZE   35                                
#define PROC_FORMAT_STRING_SIZE   257                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _BochsRpcDbg_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } BochsRpcDbg_MIDL_TYPE_FORMAT_STRING;

typedef struct _BochsRpcDbg_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } BochsRpcDbg_MIDL_PROC_FORMAT_STRING;

typedef struct _BochsRpcDbg_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } BochsRpcDbg_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const BochsRpcDbg_MIDL_TYPE_FORMAT_STRING BochsRpcDbg__MIDL_TypeFormatString;
extern const BochsRpcDbg_MIDL_PROC_FORMAT_STRING BochsRpcDbg__MIDL_ProcFormatString;
extern const BochsRpcDbg_MIDL_EXPR_FORMAT_STRING BochsRpcDbg__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: BochsDebug, ver. 1.0,
   GUID={0x38E5E1B7,0x4D07,0x4568,{0xB2,0x74,0xF3,0x12,0xB7,0xCE,0x57,0x75}} */



static const RPC_CLIENT_INTERFACE BochsDebug___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x38E5E1B7,0x4D07,0x4568,{0xB2,0x74,0xF3,0x12,0xB7,0xCE,0x57,0x75}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE BochsDebug_v1_0_c_ifspec = (RPC_IF_HANDLE)& BochsDebug___RpcClientInterface;

extern const MIDL_STUB_DESC BochsDebug_StubDesc;

static RPC_BINDING_HANDLE BochsDebug__MIDL_AutoBindHandle;


void Bochs_PauseSimulation( 
    /* [in] */ handle_t h1)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&h1);
    
}


void Bochs_ResumeSimulation( 
    /* [in] */ handle_t h1)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[28],
                  ( unsigned char * )&h1);
    
}


void Bochs_QuitSimulation( 
    /* [in] */ handle_t h1)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[56],
                  ( unsigned char * )&h1);
    
}


error_status_t Bochs_ReadLinearMemory( 
    /* [in] */ handle_t h1,
    /* [in] */ unsigned __int32 StartAddr,
    /* [in] */ unsigned __int32 Length,
    /* [size_is][ref][out] */ byte Buffer[  ])
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[84],
                  ( unsigned char * )&h1);
    return ( error_status_t  )_RetVal.Simple;
    
}


error_status_t Bochs_ReadPhysicalMemory( 
    /* [in] */ handle_t h1,
    /* [in] */ unsigned __int32 StartAddr,
    /* [in] */ unsigned __int32 Length,
    /* [size_is][ref][out] */ byte Buffer[  ])
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[136],
                  ( unsigned char * )&h1);
    return ( error_status_t  )_RetVal.Simple;
    
}


void Bochs_WaitForIdle( 
    /* [in] */ handle_t h1)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[188],
                  ( unsigned char * )&h1);
    
}


error_status_t Bochs_GetGPRs( 
    /* [in] */ handle_t h1,
    /* [ref][out] */ PBochsGPRsContext context)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&BochsDebug_StubDesc,
                  (PFORMAT_STRING) &BochsRpcDbg__MIDL_ProcFormatString.Format[216],
                  ( unsigned char * )&h1);
    return ( error_status_t  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const BochsRpcDbg_MIDL_PROC_FORMAT_STRING BochsRpcDbg__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Bochs_PauseSimulation */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x0 ),	/* 0 */
/* 18 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure Bochs_ResumeSimulation */


	/* Parameter h1 */

/* 28 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x1 ),	/* 1 */
/* 36 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 38 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 40 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 42 */	NdrFcShort( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 48 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure Bochs_QuitSimulation */


	/* Parameter h1 */

/* 56 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 58 */	NdrFcLong( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x2 ),	/* 2 */
/* 64 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 66 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 76 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure Bochs_ReadLinearMemory */


	/* Parameter h1 */

/* 84 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x3 ),	/* 3 */
/* 92 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 94 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 98 */	NdrFcShort( 0x10 ),	/* 16 */
/* 100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 102 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 104 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 106 */	NdrFcShort( 0x1 ),	/* 1 */
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter h1 */

/* 112 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 114 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StartAddr */

/* 118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 120 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Length */

/* 124 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 126 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 128 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Buffer */

/* 130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 132 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 134 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure Bochs_ReadPhysicalMemory */


	/* Return value */

/* 136 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0x4 ),	/* 4 */
/* 144 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 146 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 148 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 150 */	NdrFcShort( 0x10 ),	/* 16 */
/* 152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 154 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 156 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 158 */	NdrFcShort( 0x1 ),	/* 1 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter h1 */

/* 164 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 166 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StartAddr */

/* 170 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 172 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Length */

/* 176 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 178 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 180 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter Buffer */

/* 182 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 184 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 186 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure Bochs_WaitForIdle */


	/* Return value */

/* 188 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 190 */	NdrFcLong( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x5 ),	/* 5 */
/* 196 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 198 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 208 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure Bochs_GetGPRs */


	/* Parameter h1 */

/* 216 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x6 ),	/* 6 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 228 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 232 */	NdrFcShort( 0x54 ),	/* 84 */
/* 234 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 236 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter h1 */

/* 244 */	NdrFcShort( 0xa112 ),	/* Flags:  must free, out, simple ref, srv alloc size=40 */
/* 246 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 248 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter context */

/* 250 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 252 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 254 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

			0x0
        }
    };

static const BochsRpcDbg_MIDL_TYPE_FORMAT_STRING BochsRpcDbg__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/*  4 */	NdrFcShort( 0x1 ),	/* 1 */
/*  6 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/*  8 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 12 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 14 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 16 */	NdrFcShort( 0x2 ),	/* Offset= 2 (18) */
/* 18 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x28 ),	/* 40 */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 28 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 30 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 32 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short BochsDebug_FormatStringOffsetTable[] =
    {
    0,
    28,
    56,
    84,
    136,
    188,
    216
    };


static const MIDL_STUB_DESC BochsDebug_StubDesc = 
    {
    (void *)& BochsDebug___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &BochsDebug__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    BochsRpcDbg__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */
