#pragma once

#include "Proxy.h"

#ifdef PROXY_TYPE_DINPUT8

class Proxy_DInput8 : public Proxy {
public:
	enum class DInput8_Function {
		FN_DirectInput8Create,
		FN_DllCanUnloadNow,
		FN_DllGetClassObject,
		FN_DllRegisterServer,
		FN_DllUnregisterServer,
		FN_GetdfDIJoystick
	};

public:
	bool Initialize() override;
	static inline FARPROC& GetProxyFunction(DInput8_Function function) { return myProxyFunctions[(UINT)function]; }

protected:
	void GenerateProxyFunctions() override;

private:
	static std::array<FARPROC, 6> myProxyFunctions;
};

#endif // PROXY_TYPE_DINPUT8