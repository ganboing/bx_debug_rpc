extern "C" {
	void bx_dbg_quit_command(void);
	void bx_dbg_continue_command(void);
}
void bx_debug_break();
bool bx_dbg_rpc_read_linear(unsigned __int64, unsigned __int32, unsigned char*);
bool bx_dbg_rpc_read_physical(unsigned __int64, unsigned __int32, unsigned char*);
void bx_dbg_rpc_read_gprs(PBochsGPRsContext pContext);
void bx_dbg_rpc_print_reg();