/*
 * Copyright (c) 2021 Voysys AB / Torkel Danielsson <torkel@voysys.com>
 *
 * This file is MIT-licensed, to allow for modification and integration.
 * Note: the MIT license applies to this specific file ONLY.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

        printf("\nDetected %d EGL devices\n\n", numDevices);

        for (int deviceNum = 0; deviceNum < numDevices; ++deviceNum) {
          display = eglGetPlatformDisplay(EGL_PLATFORM_DEVICE_EXT,
                                          eglDevices[deviceNum], nullptr);

          int major = 0;
          int minor = 0;

          printf("Initializing EGL Device %d\n", deviceNum);

          if (eglInitialize(display, &major, &minor)) {
            printf("Successfully initialized EGL Device %d\n", deviceNum);

            printf("Displaying info for EGL Display on Device %d\n", deviceNum);

            if (display != EGL_NO_DISPLAY) {
              query_egl_display_data(display);
            }

          } else {
            printf("FAILED to initialize EGL Device %d\n", deviceNum);
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