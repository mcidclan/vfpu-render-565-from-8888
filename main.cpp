#include <pspgu.h>
#include <psppower.h>
#include <pspdisplay.h>
// #include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
// #include <malloc.h>
// #include <cstring>

PSP_MODULE_INFO("vfpu-sr", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

int main(int argc, char **argv) {
  scePowerSetClockFrequency(333, 333, 166);

  pspDebugScreenInitEx(NULL, PSP_DISPLAY_PIXEL_FORMAT_8888, 0);
  pspDebugScreenSetOffset(0);
  sceDisplaySetFrameBuf((void*)(0x44000000), 512, 3, PSP_DISPLAY_SETBUF_NEXTFRAME);

  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(0, 1);
    pspDebugScreenPrintf("Hello");
    sceDisplayWaitVblankStart();
  } while(!(ctl.Buttons & PSP_CTRL_HOME));
  
  pspDebugScreenClear();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Exiting...");
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  return 0;
}
