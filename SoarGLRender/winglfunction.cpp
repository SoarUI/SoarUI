#include"WinGLFunction.h"
static HMODULE libGL = NULL;

//define
winGL_ChoosePixelFormat glChoosePixelFormat;
winGL_wglCopyContext glCopyContext;
winGL_wglCreateContext glCreateContext;
winGL_wglCreateLayerContext glCreateLayerContext;
winGL_wglDeleteContext glDeleteContext;
winGL_wglGetCurrentContext glGetCurrentContext;
winGL_wglGetCurrentDC glGetCurrentDC;
winGL_wglGetProcAddress glGetProcAddress;
winGL_wglMakeCurrent glMakeCurrent;
winGL_wglShareLists glShareLists;
winGL_wglUseFontBitmapsA glUseFontBitmapsA;
winGL_wglUseFontBitmapsW glUseFontBitmapsW;
winGL_SetPixelFormat glSetPixelFormat;
winGL_SwapBuffers	glSwapBuffers;
bool loadOpenGL()
{
	libGL = LoadLibrary("opengl32.dll");
    return libGL != NULL;
}
bool initOpenGL()
{
    glChoosePixelFormat = (winGL_ChoosePixelFormat)getFunction("wglChoosePixelFormat");
    glCopyContext=(winGL_wglCopyContext)getFunction("wglCopyContext");
    glCreateContext= (winGL_wglCreateContext)getFunction("wglCreateContext");
    glCreateLayerContext=(winGL_wglCreateLayerContext)getFunction("wglCreateLayerContext");
    glDeleteContext = (winGL_wglDeleteContext)getFunction("wglDeleteContext");
    glGetCurrentContext = (winGL_wglGetCurrentContext)getFunction("wglGetCurrentContext");
    glGetCurrentDC = (winGL_wglGetCurrentDC)getFunction("wglGetCurrentDC");
    glGetProcAddress = (winGL_wglGetProcAddress)getFunction("wglGetProcAddress");
    glMakeCurrent = (winGL_wglMakeCurrent)getFunction("wglMakeCurrent");
    glShareLists = (winGL_wglShareLists)getFunction("wglShareLists");
    glUseFontBitmapsA = (winGL_wglUseFontBitmapsA)getFunction("wglUseFontBitmapsA");
    glUseFontBitmapsW = (winGL_wglUseFontBitmapsW)getFunction("wglUseFontBitmapsW");
    glSetPixelFormat = (winGL_SetPixelFormat)getFunction("wglSetPixelFormat");
    glSwapBuffers = (winGL_SwapBuffers)getFunction("wglSwapBuffers");
    //glCreateLayerContext = (winGL_wglCreateLayerContext)getFunction("wglCreateLayerContext");
    return true;
}
void* getFunction(const char* namez) {
    void* result = NULL;
    if (libGL == NULL) return NULL;
    if (result == NULL) {
     result = (void*)GetProcAddress((HMODULE)libGL, namez);
    }
    return result;
}
void closeOpenGL(void) {
    if (libGL != NULL) {
        FreeLibrary((HMODULE)libGL);
        libGL = NULL;
    }
}