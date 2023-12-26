#pragma once
#include<windows.h>
extern bool loadOpenGL();
extern bool initOpenGL();
extern void closeOpenGL();
extern void* getFunction(const char* namez);
typedef int (WINAPI *winGL_ChoosePixelFormat)(_In_ HDC hdc, _In_ CONST PIXELFORMATDESCRIPTOR* ppfd);
typedef BOOL(WINAPI *winGL_wglCopyContext)(HGLRC, HGLRC, UINT);
typedef  HGLRC (WINAPI *winGL_wglCreateContext)(HDC);
typedef  HGLRC (WINAPI *winGL_wglCreateLayerContext)(HDC, int);
typedef  BOOL(WINAPI *winGL_wglDeleteContext)(HGLRC);
typedef  HGLRC(WINAPI *winGL_wglGetCurrentContext)(VOID);
typedef  HDC(WINAPI *winGL_wglGetCurrentDC)(VOID);
typedef  PROC(WINAPI *winGL_wglGetProcAddress)(LPCSTR);
typedef  BOOL(WINAPI *winGL_wglMakeCurrent)(HDC, HGLRC);
typedef  BOOL(WINAPI *winGL_wglShareLists)(HGLRC, HGLRC);
typedef  BOOL(WINAPI *winGL_wglUseFontBitmapsA)(HDC, DWORD, DWORD, DWORD);
typedef  BOOL(WINAPI *winGL_wglUseFontBitmapsW)(HDC, DWORD, DWORD, DWORD);
typedef BOOL(WINAPI *winGL_SetPixelFormat)(_In_ HDC hdc, _In_ int format, _In_ CONST PIXELFORMATDESCRIPTOR* ppfd);
typedef BOOL(WINAPI *winGL_SwapBuffers)(HDC);


extern winGL_ChoosePixelFormat glChoosePixelFormat;
extern winGL_wglCopyContext glCopyContext;
extern winGL_wglCreateContext glCreateContext;
extern winGL_wglCreateLayerContext glCreateLayerContext;
extern winGL_wglDeleteContext glDeleteContext;
extern winGL_wglGetCurrentContext glGetCurrentContext;
extern winGL_wglGetCurrentDC glGetCurrentDC;
extern winGL_wglGetProcAddress glGetProcAddress;
extern winGL_wglMakeCurrent glMakeCurrent;
extern winGL_wglShareLists glShareLists;
extern winGL_wglUseFontBitmapsA glUseFontBitmapsA;
extern winGL_wglUseFontBitmapsW glUseFontBitmapsW;
extern winGL_SetPixelFormat glSetPixelFormat;
extern winGL_SwapBuffers	glSwapBuffers;