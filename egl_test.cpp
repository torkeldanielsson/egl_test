#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdio.h>

int main() {

  EGLDisplay display = EGL_NO_DISPLAY;

  {
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT =
        reinterpret_cast<PFNEGLQUERYDEVICESEXTPROC>(
            eglGetProcAddress("eglQueryDevicesEXT"));
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplay =
        reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
            eglGetProcAddress("eglGetPlatformDisplayEXT"));

    if (eglQueryDevicesEXT != nullptr && eglGetPlatformDisplay != nullptr) {
      constexpr int MaxDevices = 16;
      EGLDeviceEXT eglDevices[MaxDevices];
      int numDevices = 0;
      eglQueryDevicesEXT(MaxDevices, eglDevices, &numDevices);

      printf("Detected %d EGL devices (opening display on device 0)",
             numDevices);

      if (numDevices > 0) {
        display = eglGetPlatformDisplay(EGL_PLATFORM_DEVICE_EXT, eglDevices[0],
                                        nullptr);
      }
    }
  }

  if (display == EGL_NO_DISPLAY) {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  }

  if (display == EGL_NO_DISPLAY) {
    printf("No valid EGL Display available");
  }
}