set(DDR mig_0)
set(mig_0 "0x80000000;0x8000000")
set(lmb_bram_0 "0x50;0xffb0")
set(TOTAL_MEM_CONTROLLERS "mig_0;lmb_bram_0")
set(MEMORY_SECTION "MEMORY
{
	mig_0 : ORIGIN = 0x80000000, LENGTH = 0x8000000
	lmb_bram_0 : ORIGIN = 0x0, LENGTH = 0x10000
}")
set(STACK_SIZE 0x400)
set(HEAP_SIZE 0x800)
