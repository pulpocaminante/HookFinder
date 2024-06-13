// Very simple hook reporter/blocker using msdetours

#include "pch.h"
#pragma comment(lib, "detours.lib")
#include "detours.h"
#include <iostream>
#include <chrono>
#include <thread>

#define allowHooks 1

static HHOOK(WINAPI* originalSetWindowsHookExW)(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId) = SetWindowsHookExW;

HHOOK WINAPI hookedSetWindowsHookExW(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId)
{

    std::cout << std::endl;
    switch (idHook) {
    case WH_CALLWNDPROC:
        std::cout << "Host process hooked WH_CALLWNDPROC" << std::endl;
        break;
    case WH_CALLWNDPROCRET:
        std::cout << "Host process hooked WH_CALLWNDPROCRET" << std::endl;
        break;
    case WH_CBT:
        std::cout << "Host process hooked WH_CBT" << std::endl;
        break;
    case WH_DEBUG:
        std::cout << "Host process hooked WH_DEBUG" << std::endl;
        break;
    case WH_FOREGROUNDIDLE:
        std::cout << "Host process hooked WH_FOREGROUNDIDLE" << std::endl;
        break;
    case WH_JOURNALPLAYBACK:
        std::cout << "Host process hooked WH_JOURNALPLAYBACK" << std::endl;
        break;
    case WH_JOURNALRECORD:
        std::cout << "Host process hooked WH_JOURNALRECORD" << std::endl;
        break;
    case WH_MOUSE_LL:
        std::cout << "Host process hooked WH_MOUSE_LL" << std::endl;
        break;
    case WH_GETMESSAGE:
        std::cout << "Host process hooked WH_GETMESSAGE" << std::endl;
        break;
    case WH_KEYBOARD:
        std::cout << "Host process hooked WH_KEYBOARD" << std::endl;
        break;
    case WH_KEYBOARD_LL:
        std::cout << "Host process hooked WH_KEYBOARD_LL" << std::endl;
        break;
    case WH_MOUSE:
        std::cout << "Host process hooked WH_MOUSE" << std::endl;
        break;
    case WH_MSGFILTER:
        std::cout << "Host process hooked WH_MSGFILTER" << std::endl;
        break;
    case WH_SHELL:
        std::cout << "Host process hooked WH_SHELL" << std::endl;
        break;
    case WH_SYSMSGFILTER:
        std::cout << "Host process hooked WH_SYSMSGFILTER" << std::endl;
        break;
    default:
        std::cout << "Host process hooked unknown idHook " << idHook << std::endl;
        break;
    }

    std::cout << " - Hook procedure: " << lpfn << std::endl;
    std::cout << " - Instance: " << hMod << std::endl;
    std::cout << " - Thread id: " << dwThreadId << std::endl;
    if (allowHooks)
    {
        HHOOK tmpHandle = originalSetWindowsHookExW(idHook, lpfn, hMod, dwThreadId);
        return tmpHandle;
    }
    SetLastError(ERROR_HOOK_NOT_INSTALLED);
    return NULL;
}

BOOL installHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)originalSetWindowsHookExW, hookedSetWindowsHookExW);
    DetourTransactionCommit();
    return true;
}

BOOL uninstallHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)originalSetWindowsHookExW, hookedSetWindowsHookExW);
    DetourTransactionCommit();
    return true;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    if (installHooks())
    {
        std::cout << "Hooks installed successfully" << std::endl;
    }
    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (uninstallHooks())
    {
        std::cout << "Hooks removed successfully" << std::endl;
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    }
    return TRUE;
}

