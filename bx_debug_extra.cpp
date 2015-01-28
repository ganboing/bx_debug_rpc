#include "bochs.h"
#include "cpu/cpu.h"
#include "bx_debug/debug.h"
#include "bx_debug_rpc/BochsRpcDataTypes.h"

#if BX_SUPPORT_SMP
#error "Bochs should be configured in non-SMP mode to allow rpc debug"
#endif

#if !BX_SUPPORT_X86_64
#error "Currently rpc debug assumes x86_64 arch"
#endif

#define STR(v) #v
#define TOSTR(v) STR(v)
#define MSR_NAME_MAX_STR	25

void bx_rpc_get_command();
extern bool bx_rpc_debugging;

bool bx_dbg_rpc_read_linear(unsigned int iproc, unsigned __int64 addr, unsigned __int32 len, unsigned char* buf)
{
	return !!bx_dbg_read_linear(iproc, addr, len, buf);
}

bool bx_dbg_rpc_read_physical(unsigned int iproc, unsigned __int64 addr, unsigned __int32 len, unsigned char* buf)
{
	return !!BX_MEM(0)->dbg_fetch_mem(BX_CPU(iproc), (bx_phy_address)addr, len, buf);
}

void bx_dbg_rpc_print_reg(unsigned int iproc)
{
	unsigned int dbg_cpu_old = dbg_cpu;
	dbg_cpu = iproc;
	bx_dbg_info_registers_command(BX_INFO_GENERAL_PURPOSE_REGS);
	dbg_cpu = dbg_cpu_old;
}

unsigned int bx_dbg_rpc_get_proc_cnt()
{
	return BX_SMP_PROCESSORS;
}

void bx_dbg_rpc_read_gprs(unsigned int iproc, PBochsGPRsContext pContext)
{
	pContext->RAX = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RAX);
	pContext->RCX = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RCX);
	pContext->RDX = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RDX);
	pContext->RBX = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RBX);
	pContext->RSP = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RSP);
	pContext->RBP = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RBP);
	pContext->RSI = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RSI);
	pContext->RDI = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_RDI);
	pContext->R8 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R8);
	pContext->R9 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R9);
	pContext->R10 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R10);
	pContext->R11 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R11);
	pContext->R12 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R12);
	pContext->R13 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R13);
	pContext->R14 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R14);
	pContext->R15 = BX_CPU(iproc)->get_reg64(BX_64BIT_REG_R15);
	pContext->RIP = BX_CPU(iproc)->get_instruction_pointer();
	pContext->EFLAGS = BX_CPU(iproc)->read_eflags();
}

static void print_char(char c)
{
	//assume char is signed!!
	if (c < ' ')
	{
		dbg_printf(".");
	}
	else
	{
		dbg_printf("%c", c);
	}
}

static void print_byte(char c)
{
	if (c)
	{
		dbg_printf("#");
	}
	else
	{
		dbg_printf(" ");
	}
}

static void print_Bit64u(Bit64u value)
{
	char* str = (char*)(&value);
	for (size_t i = sizeof(Bit64u); i-- > 0;)
	{
		print_byte(str[i]);
	}
}

#define PRINT_MSR(msr) \
	do{\
		const char name_local[] = #msr;\
		static_assert(sizeof(name_local) - 1 <= MSR_NAME_MAX_STR, "MSR string too long");\
		union {\
			struct{\
				Bit32u lo;\
				Bit32u hi;\
			}s;\
			Bit64u value;\
		}edx_eax; \
		edx_eax.value = BX_CONST64(0xDEADBEEFDEADBEEF);\
		if(BX_CPU(dbg_cpu)->rdmsr(msr, &edx_eax.value)){\
			dbg_printf("%-" TOSTR(MSR_NAME_MAX_STR) "s[%08X] = %08X:%08X ", name_local, msr, edx_eax.s.hi, edx_eax.s.lo);\
			print_Bit64u(edx_eax.value);\
			dbg_printf("\n");\
		}else{\
			dbg_printf("%-" TOSTR(MSR_NAME_MAX_STR) "s[%08X] read failed\n", name_local, msr);\
		}\
	} while (0);

static void bx_print_msr_value()
{
	// MSR registers 
	PRINT_MSR(BX_MSR_TSC);
	PRINT_MSR(BX_MSR_APICBASE);
	//PRINT_MSR(BX_MSR_TSC_ADJUST);
#if BX_CPU_LEVEL >= 6
	PRINT_MSR(BX_MSR_SYSENTER_CS);
	PRINT_MSR(BX_MSR_SYSENTER_ESP);
	PRINT_MSR(BX_MSR_SYSENTER_EIP);
#endif
	//PRINT_MSR(BX_MSR_DEBUGCTLMSR);
	//PRINT_MSR(BX_MSR_LASTBRANCHFROMIP);
	//PRINT_MSR(BX_MSR_LASTBRANCHTOIP);
	//PRINT_MSR(BX_MSR_LASTINTOIP);

#if BX_CPU_LEVEL >= 6
	PRINT_MSR(BX_MSR_MTRRCAP);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE0);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK0);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE1);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK1);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE2);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK2);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE3);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK3);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE4);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK4);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE5);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK5);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE6);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK6);
	PRINT_MSR(BX_MSR_MTRRPHYSBASE7);
	PRINT_MSR(BX_MSR_MTRRPHYSMASK7);
	PRINT_MSR(BX_MSR_MTRRFIX64K_00000);
	PRINT_MSR(BX_MSR_MTRRFIX16K_80000);
	PRINT_MSR(BX_MSR_MTRRFIX16K_A0000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_C0000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_C8000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_D0000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_D8000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_E0000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_E8000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_F0000);
	PRINT_MSR(BX_MSR_MTRRFIX4K_F8000);
	PRINT_MSR(BX_MSR_PAT);
	PRINT_MSR(BX_MSR_MTRR_DEFTYPE);
#endif

#if BX_SUPPORT_PERFMON
	/*PRINT_MSR(BX_MSR_PMC0);
	PRINT_MSR(BX_MSR_PMC1);
	PRINT_MSR(BX_MSR_PMC2);
	PRINT_MSR(BX_MSR_PMC3);
	PRINT_MSR(BX_MSR_PMC4);
	PRINT_MSR(BX_MSR_PMC5);
	PRINT_MSR(BX_MSR_PMC6);
	PRINT_MSR(BX_MSR_PMC7);
	PRINT_MSR(BX_MSR_PERFEVTSEL0);
	PRINT_MSR(BX_MSR_PERFEVTSEL1);
	PRINT_MSR(BX_MSR_PERFEVTSEL2);
	PRINT_MSR(BX_MSR_PERFEVTSEL3);
	PRINT_MSR(BX_MSR_PERFEVTSEL4);
	PRINT_MSR(BX_MSR_PERFEVTSEL5);
	PRINT_MSR(BX_MSR_PERFEVTSEL6);
	PRINT_MSR(BX_MSR_PERFEVTSEL7);
	PRINT_MSR(BX_MSR_PERF_FIXED_CTR0);
	PRINT_MSR(BX_MSR_PERF_FIXED_CTR1);
	PRINT_MSR(BX_MSR_PERF_FIXED_CTR2);
	PRINT_MSR(BX_MSR_FIXED_CTR_CTRL);
	PRINT_MSR(BX_MSR_PERF_GLOBAL_CTRL);*/
#endif

	PRINT_MSR(BX_MSR_TSC_DEADLINE);

	//Intel MPX supervisor bound configuration register
	//PRINT_MSR(BX_MSR_BNDCFGS);

	PRINT_MSR(BX_MSR_MAX_INDEX);

#if BX_SUPPORT_VMX
	/*PRINT_MSR(BX_MSR_VMX_BASIC);
	PRINT_MSR(BX_MSR_VMX_PINBASED_CTRLS);
	PRINT_MSR(BX_MSR_VMX_PROCBASED_CTRLS);
	PRINT_MSR(BX_MSR_VMX_VMEXIT_CTRLS);
	PRINT_MSR(BX_MSR_VMX_VMENTRY_CTRLS);
	PRINT_MSR(BX_MSR_VMX_MISC);
	PRINT_MSR(BX_MSR_VMX_CR0_FIXED0);
	PRINT_MSR(BX_MSR_VMX_CR0_FIXED1);
	PRINT_MSR(BX_MSR_VMX_CR4_FIXED0);
	PRINT_MSR(BX_MSR_VMX_CR4_FIXED1);
	PRINT_MSR(BX_MSR_VMX_VMCS_ENUM);
	PRINT_MSR(BX_MSR_VMX_PROCBASED_CTRLS2);
	PRINT_MSR(BX_MSR_VMX_EPT_VPID_CAP);
	PRINT_MSR(BX_MSR_VMX_TRUE_PINBASED_CTRLS);
	PRINT_MSR(BX_MSR_VMX_TRUE_PROCBASED_CTRLS);
	PRINT_MSR(BX_MSR_VMX_TRUE_VMEXIT_CTRLS);
	PRINT_MSR(BX_MSR_VMX_TRUE_VMENTRY_CTRLS);
	PRINT_MSR(BX_MSR_VMX_VMFUNC);
	PRINT_MSR(BX_MSR_IA32_FEATURE_CONTROL);
	PRINT_MSR(BX_MSR_IA32_SMM_MONITOR_CTL);*/
#endif

	PRINT_MSR(BX_MSR_EFER);
	PRINT_MSR(BX_MSR_STAR);
	PRINT_MSR(BX_MSR_LSTAR);
	PRINT_MSR(BX_MSR_CSTAR);
	PRINT_MSR(BX_MSR_FMASK);
	PRINT_MSR(BX_MSR_FSBASE);
	PRINT_MSR(BX_MSR_GSBASE);
	PRINT_MSR(BX_MSR_KERNELGSBASE);
	PRINT_MSR(BX_MSR_TSC_AUX);
}

#undef PRINT_MSR
#undef MSR_NAME_MAX_STR
#undef TOSTR
#undef STR