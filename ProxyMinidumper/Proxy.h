#pragma once

class Proxy
{
public:
	virtual bool Initialize() = 0;

	inline void Shutdown()
	{
		if (!myModule)
		{
			return;
		}

		FreeLibrary(myModule);
	}

protected:
	virtual void GenerateProxyFunctions() = 0;

protected:
	HMODULE myModule = NULL;
};