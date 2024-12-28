#include "pch.h"
#include "Proxy_DInput8.h"

#ifdef PROXY_TYPE_DINPUT8

namespace {
	static const char* ourPathToDllFromWindows = "/SysWOW64/dinput8.dll";
}

typedef HRESULT(__stdcall* T_DInput8_DInput8Create)(HINSTANCE, DWORD, REFIID, LPVOID*, LPVOID*);
typedef HRESULT(__stdcall* T_DInput8_DllCanUnloadNow)(void);
typedef HRESULT(__stdcall* T_DInput8_DllGetClassObject)(REFCLSID, REFIID, LPVOID*);
typedef void(__stdcall* T_DInput8_DllRegisterServer)(void);
typedef void(__stdcall* T_DInput8_DllUnregisterServer)(void);
typedef LPVOID*(__stdcall* T_DInput8_GetdfDIJoystick)(void);

std::array<FARPROC, 6> Proxy_DInput8::myProxyFunctions = {};

bool Proxy_DInput8::Initialize()
{
	if (myModule)
	{
		return true;
	}

	char dllPath[MAX_PATH];
	UINT windowPathSize = GetWindowsDirectoryA(dllPath, MAX_PATH);

	if (windowPathSize == 0)
	{
		return false;
	}

	strcat_s(dllPath, ourPathToDllFromWindows);
	myModule = LoadLibraryA(dllPath);

	if (!myModule)
	{
		return false;
	}

	GenerateProxyFunctions();

	return true;
}

void Proxy_DInput8::GenerateProxyFunctions()
{
	myProxyFunctions[(UINT)DInput8_Function::FN_DirectInput8Create] = GetProcAddress(myModule, "DirectInput8Create");
	myProxyFunctions[(UINT)DInput8_Function::FN_DllCanUnloadNow] = GetProcAddress(myModule, "DllCanUnloadNow");
	myProxyFunctions[(UINT)DInput8_Function::FN_DllGetClassObject] = GetProcAddress(myModule, "DllGetClassObject");
	myProxyFunctions[(UINT)DInput8_Function::FN_DllRegisterServer] = GetProcAddress(myModule, "DllRegisterServer");
	myProxyFunctions[(UINT)DInput8_Function::FN_DllUnregisterServer] = GetProcAddress(myModule, "DllUnregisterServer");
	myProxyFunctions[(UINT)DInput8_Function::FN_GetdfDIJoystick] = GetProcAddress(myModule, "GetdfDIJoystick");
}

extern "C" {
	#pragma comment(linker, "/EXPORT:DirectInput8Create=_DirectInput8Create@20")
	__declspec(dllexport) HRESULT __stdcall DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPVOID* punkOuter)
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_DirectInput8Create);
		return ((T_DInput8_DInput8Create)funcPtr)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	}

	/*__declspec(dllexport) HRESULT __stdcall DllCanUnloadNow()
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_DllCanUnloadNow);
		return ((T_DInput8_DllCanUnloadNow)funcPtr)();
	}

	__declspec(dllexport) HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_DllGetClassObject);
		return ((T_DInput8_DllGetClassObject)funcPtr)(rclsid, riid, ppv);
	}

	__declspec(dllexport) void __stdcall DllRegisterServer()
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_DllRegisterServer);
		return ((T_DInput8_DllRegisterServer)funcPtr)();
	}

	__declspec(dllexport) void __stdcall DllUnregisterServer()
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_DllUnregisterServer);
		return ((T_DInput8_DllUnregisterServer)funcPtr)();
	}*/

	#pragma comment(linker, "/EXPORT:GetdfDIJoystick=_GetdfDIJoystick@0")
	__declspec(dllexport) LPVOID* __stdcall GetdfDIJoystick()
	{
		FARPROC& funcPtr = Proxy_DInput8::GetProxyFunction(Proxy_DInput8::DInput8_Function::FN_GetdfDIJoystick);
		return ((T_DInput8_GetdfDIJoystick)funcPtr)();
	}
}

#endif // PROXY_TYPE_DINPUT8