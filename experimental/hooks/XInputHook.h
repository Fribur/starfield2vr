#pragma once
#include <Xinput.h>
#include <dinput.h>
#include <hstring.h>
#include <intsafe.h>
#include <winrt/base.h>
#define DIRECTINPUT_VERSION 0x0800
//#pragma comment(lib, "Dxguid.lib")
//#pragma comment(lib, "Dinput8.lib")

namespace VRFramework{
    class XInputHook
    {
    public:
        void InitHooks();
        void HookGerRawInput();
        void hook_gaming_input();

        static XInputHook* Get()
        {
            static auto instance(new XInputHook);
            return instance;
        }

        XInputHook(const XInputHook&) = delete;
        XInputHook& operator=(const XInputHook&) = delete;

        DWORD XInputGetState(DWORD i, _XINPUT_STATE* pState);

        HRESULT RoGetActivationFactory(HSTRING, const IID& guid, void** pVoid);

    private:
        XInputHook() = default;
    };
}
