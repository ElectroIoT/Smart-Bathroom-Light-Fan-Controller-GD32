/* Minimal Cortex-M3 startup: vector table, .data copy, .bss clear, main(). */
#include <stdint.h>

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);
void SysTick_Handler(void);

void Reset_Handler(void);
static void Default_Handler(void) { for (;;) { } }

__attribute__((section(".isr_vector"), used))
void (*const g_vectors[16])(void) = {
    (void (*)(void))&_estack,   /* 0  initial SP   */
    Reset_Handler,              /* 1  reset        */
    Default_Handler,            /* 2  NMI          */
    Default_Handler,            /* 3  HardFault    */
    Default_Handler,            /* 4  MemManage    */
    Default_Handler,            /* 5  BusFault     */
    Default_Handler,            /* 6  UsageFault   */
    0, 0, 0, 0,                 /* 7-10 reserved   */
    Default_Handler,            /* 11 SVCall       */
    Default_Handler,            /* 12 DebugMon     */
    0,                          /* 13 reserved     */
    Default_Handler,            /* 14 PendSV       */
    SysTick_Handler,            /* 15 SysTick      */
};

void Reset_Handler(void)
{
    uint32_t *s = &_sidata, *d = &_sdata;
    while (d < &_edata) *d++ = *s++;
    for (d = &_sbss; d < &_ebss; ) *d++ = 0;
    (void)main();
    for (;;) { }
}
