//
//  Main.cpp
//  QuadrocopterBrain
//
//  Created by anton on 18.10.15.
//  Copyright © 2015 anton. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "Unity/IUnityGraphics.h"

#include "Main.h"
#include "ApiDiscreteDeepQ.hpp"

// --------------------------------------------------------------------------
// Include headers for the graphics APIs we support

#if SUPPORT_D3D9
	#include <d3d9.h>
	#include "Unity/IUnityGraphicsD3D9.h"
#endif
#if SUPPORT_D3D11
	#include <d3d11.h>
	#include "Unity/IUnityGraphicsD3D11.h"
#endif
#if SUPPORT_D3D12
	#include <d3d12.h>
	#include "Unity/IUnityGraphicsD3D12.h"
#endif

#if SUPPORT_OPENGLES
	#if UNITY_IPHONE
		#include <OpenGLES/ES2/gl.h>
	#elif UNITY_ANDROID
		#include <GLES2/gl2.h>
	#endif
#elif SUPPORT_OPENGL
	#if UNITY_WIN || UNITY_LINUX
		#include <GL/gl.h>
	#else
		#include <OpenGL/gl.h>
	#endif
#endif

using namespace std;

// Prints a string
void DebugLog (std::string str)
{
#if UNITY_WIN
    OutputDebugStringA (str.c_str ());
#else
    fprintf(stderr, "%s\n", str.c_str ());
#endif
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventID) {
	cerr << "--- OnRenderEvent" << endl;
//	processRenderEvent(eventID);
//	quadrocopterBrain.act();
}

// --------------------------------------------------------------------------
// UnitySetInterfaces

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

static IUnityInterfaces* s_UnityInterfaces = NULL;
static IUnityGraphics* s_Graphics = NULL;
static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    DebugLog("--- UnityPluginLoad");
    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
	
    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);

	initApiDiscreteDeepQ();
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

// --------------------------------------------------------------------------
// GraphicsDeviceEvent

// Actual setup/teardown functions defined below
#if SUPPORT_D3D9
static void DoEventGraphicsDeviceD3D9(UnityGfxDeviceEventType eventType);
#endif
#if SUPPORT_D3D11
static void DoEventGraphicsDeviceD3D11(UnityGfxDeviceEventType eventType);
#endif
#if SUPPORT_D3D12
static void DoEventGraphicsDeviceD3D12(UnityGfxDeviceEventType eventType);
#endif
#if SUPPORT_OPENGLES
static void DoEventGraphicsDeviceGLES(UnityGfxDeviceEventType eventType);
#endif

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
    UnityGfxRenderer currentDeviceType = s_DeviceType;
    
    switch (eventType)
    {
        case kUnityGfxDeviceEventInitialize:
        {
            DebugLog("OnGraphicsDeviceEvent(Initialize).\n");
            s_DeviceType = s_Graphics->GetRenderer();
            currentDeviceType = s_DeviceType;
            break;
        }
            
        case kUnityGfxDeviceEventShutdown:
        {
            DebugLog("OnGraphicsDeviceEvent(Shutdown).\n");
            s_DeviceType = kUnityGfxRendererNull;
            break;
        }
            
        case kUnityGfxDeviceEventBeforeReset:
        {
            DebugLog("OnGraphicsDeviceEvent(BeforeReset).\n");
            break;
        }
            
        case kUnityGfxDeviceEventAfterReset:
        {
            DebugLog("OnGraphicsDeviceEvent(AfterReset).\n");
            break;
        }
    };
    
#if SUPPORT_D3D9
    if (currentDeviceType == kUnityGfxRendererD3D9)
        DoEventGraphicsDeviceD3D9(eventType);
#endif
    
#if SUPPORT_D3D11
    if (currentDeviceType == kUnityGfxRendererD3D11)
        DoEventGraphicsDeviceD3D11(eventType);
#endif
    
#if SUPPORT_D3D12
    if (currentDeviceType == kUnityGfxRendererD3D12)
        DoEventGraphicsDeviceD3D12(eventType);
#endif
    
#if SUPPORT_OPENGLES
    if (currentDeviceType == kUnityGfxRendererOpenGLES20 ||
        currentDeviceType == kUnityGfxRendererOpenGLES30)
        DoEventGraphicsDeviceGLES(eventType);
#endif
}

// --------------------------------------------------------------------------
// GetRenderEventFunc, an example function we export which is used to get a rendering event callback function.
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
    return OnRenderEvent;
}
