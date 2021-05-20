#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdio.h>

static void query_egl_display_data(EGLDisplay display) {

  const char *eglVersion = eglQueryString(display, EGL_VERSION);
  const char *eglVendor = eglQueryString(display, EGL_VENDOR);
  const char *eglExtensions = eglQueryString(display, EGL_EXTENSIONS);
  const char *eglClientApis = eglQueryString(display, EGL_CLIENT_APIS);

  printf("EGL_VERSION: %s\n", eglVersion);
  printf("EGL_VENDOR: %s\n", eglVendor);
  printf("EGL_EXTENSIONS: %s\n", eglExtensions);
  printf("EGL_CLIENT_APIS: %s\n", eglClientApis);
}

int main() {

  EGLDisplay display = EGL_NO_DISPLAY;

  {
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevices =
        reinterpret_cast<PFNEGLQUERYDEVICESEXTPROC>(
            eglGetProcAddress("eglQueryDevicesEXT"));
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplay =
        reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
            eglGetProcAddress("eglGetPlatformDisplayEXT"));

    if (eglQueryDevices != nullptr && eglGetPlatformDisplay != nullptr) {

      printf("eglQueryDevicesEXT and eglGetPlatformDisplayEXT are available, "
             "attempting to query available EGL devices\n");

      constexpr int MaxDevices = 16;
      EGLDeviceEXT eglDevices[MaxDevices];
      int numDevices = 0;

      if (eglQueryDevices(MaxDevices, eglDevices, &numDevices)) {

        printf("Detected %d EGL devices\n", numDevices);

        for (int deviceNum = 0; deviceNum < numDevices; ++deviceNum) {
          display = eglGetPlatformDisplay(EGL_PLATFORM_DEVICE_EXT,
                                          eglDevices[deviceNum], nullptr);

          int major = 1;
          int minor = 1;

          eglInitialize(display, &major, &minor);

          printf("Displaying info for EGL Display on Device %d\n", deviceNum);

          if (display != EGL_NO_DISPLAY) {
            query_egl_display_data(display);
          }
          printf("\n");
        }

      } else {
        printf("Failed to query devices\n");
      }

    } else {
      printf("eglQueryDevicesEXT and eglGetPlatformDisplayEXT are NOT "
             "available\n");
    }
  }

  if (display == EGL_NO_DISPLAY) {
    printf("Attempting to get EGL_DEFAULT_DISPLAY\n");
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (display != EGL_NO_DISPLAY) {
      printf("EGL_DEFAULT_DISPLAY can be opened ok!\n");
    } else {
      printf("Failed to open EGL_DEFAULT_DISPLAY\n");
    }
  }

  if (display == EGL_NO_DISPLAY) {
    printf("No valid EGL Display available\n");
  }
}