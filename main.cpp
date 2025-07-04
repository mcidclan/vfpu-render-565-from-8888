#include <pspgu.h>
#include <psppower.h>
#include <pspdisplay.h>
// #include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
// #include <malloc.h>
// #include <cstring>

#define hwp          volatile u32*
#define hw(addr)     (*((hwp)(addr)))
#define fhw(addr)    (*(float*)&hw(addr))

PSP_MODULE_INFO("vfpu-sr", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

u32 colors[8] = {
  0xffff0000,
  0xffffffff,
  0xff00ff00,
  0xffffffff,
  
  0xff0000ff,
  0xffffffff,
  0xff00ffff,
  0xffffffff,
};

void initColors() {
  asm volatile (
    "or   $t4, %0, $zero\n"
    "lw   $t0, 0x00($t4)\n"
    "lw   $t1, 0x04($t4)\n"
    "lw   $t2, 0x08($t4)\n"
    "lw   $t3, 0x0c($t4)\n"
    "mtv   $t0, S000\n"
    "mtv   $t1, S001\n"
    "mtv   $t2, S002\n"
    "mtv   $t3, S003\n"
    "lw   $t0, 0x10($t4)\n"
    "lw   $t1, 0x14($t4)\n"
    "lw   $t2, 0x18($t4)\n"
    "lw   $t3, 0x1c($t4)\n"
    "mtv   $t0, S100\n"
    "mtv   $t1, S101\n"
    "mtv   $t2, S102\n"
    "mtv   $t3, S103\n"
    :
    : "r" (colors)
    : "t0", "t1", "t2", "t3", "t4"
  );
}

void genPixels() {
  asm volatile (
    "vpfxs      [Y, Z, W, X]\n" // or "vpfxs      Y, Z, W, X\n" on other versions
    "vmov.q     c300, c000\n"
    "vmov.q     c000, c300\n"
    //
    //
    "vt5650.q   c200, c000\n"
    "vt5650.q   c202, c100\n"
    "sv.q       c200, 0x44010000\n"
    :
    :
    :
  );
}

int main(int argc, char **argv) {
  scePowerSetClockFrequency(333, 333, 166);

  pspDebugScreenInitEx(NULL, PSP_DISPLAY_PIXEL_FORMAT_565, 0);
  pspDebugScreenSetOffset(0);
  sceDisplaySetFrameBuf((void*)(0x44000000), 512,
    PSP_DISPLAY_PIXEL_FORMAT_565, PSP_DISPLAY_SETBUF_NEXTFRAME);

  SceCtrlData ctl;
  initColors();
  
  do {
    genPixels();
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(0, 1);
    pspDebugScreenPrintf("Random pixel color generation");
    sceDisplayWaitVblankStart();
    sceKernelDelayThread(1000000);
  } while(!(ctl.Buttons & PSP_CTRL_HOME));
  
  pspDebugScreenClear();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Exiting...");
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  return 0;
}
