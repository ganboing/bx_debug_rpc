extern "C" {
	void bx_dbg_quit_command(void);
	void bx_dbg_continue_command(void);
}
void bx_debug_break();
bool bx_dbg_rpc_read_linear(unsigned int iproc, unsigned __int64, unsigned __int32, unsigned char*);
bool bx_dbg_rpc_read_physical(unsigned int iproc, unsigned __int64, unsigned __int32, unsigned char*);
void bx_dbg_rpc_read_gprs(unsigned int iproc, PBochsGPRsContext pContext);
void bx_dbg_rpc_print_reg(unsigned int iproc);
unsigned int bx_dbg_rpc_get_proc_cnt();