#include "pch.h"

#ifdef PROXY_TYPE_DINPUT8
#include "Proxy_DInput8.h"
#define PROXY_CLASS Proxy_DInput8
#endif

#ifdef DEBUG_CONSOLE

void InitializeConsole() {
    BOOL consoleAllocated = AllocConsole();
    if (!consoleAllocated)
    {
        return;
    }

    FILE* outputBuffer;
    freopen_s(&outputBuffer, "CONOUT$", "w", stdout);
    SetConsoleTitle(L"Proxy Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

#endif // DEBUG_CONSOLE

void CreateMinidump(EXCEPTION_POINTERS* exceptionInfo) {
    std::time_t currentTime = std::time(nullptr);

    std::stringstream fileNameStream;
    fileNameStream << "minidump_" << currentTime << ".mdmp";

    std::string dumpFileName = fileNameStream.str();

    HANDLE hFile = CreateFileA(
        dumpFileName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,  // No sharing
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create dump file: " << GetLastError() << std::endl;
        return;
    }

    MINIDUMP_EXCEPTION_INFORMATION expParam;
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = exceptionInfo;
    expParam.ClientPointers = FALSE;

    // Write the minidump
    BOOL result = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpNormal,
        &expParam,
        NULL,
        NULL
    );

    if (!result) {
        std::cout << "Failed to write minidump: " << GetLastError() << std::endl;
    }

    CloseHandle(hFile);
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    std::cout << "Unhandled exception occurred. Creating minidump..." << std::endl;
    CreateMinidump(exceptionInfo);

    return EXCEPTION_CONTINUE_SEARCH;  // This will pass the exception to the default handler
}

int APIENTRY TriggerException()
{
    int a = 0;
    int b = 5 / a;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    static PROXY_CLASS proxy;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

#ifdef DEBUG_CONSOLE
        InitializeConsole();
#endif // DEBUG_CONSOLE
        proxy.Initialize();

        SetUnhandledExceptionFilter(ExceptionHandler);

        //std::thread(TriggerException).detach();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        proxy.Shutdown();
        break;
    }
    return TRUE;
}

