#pragma once

#include <functional>

#include <Windows.h>
#include <utils/FunctionHook.h>

#include "utility/PointerHook.hpp"
#define RE_TOGGLE_CURSOR WM_APP + 1

// This type of hook replaces a windows message procedure so that it can intercept
// messages sent to the window.
class WindowsMessageHook {
public:
    std::function<bool(HWND, UINT, WPARAM, LPARAM)> on_message;
    std::function<bool(LPRECT* lpRect)> on_clip_cursor;

    WindowsMessageHook() = delete;
    WindowsMessageHook(const WindowsMessageHook& other) = delete;
    WindowsMessageHook(WindowsMessageHook&& other) = delete;
    WindowsMessageHook(HWND wnd);
    virtual ~WindowsMessageHook();

    // This gets called automatically by the destructor but you can call it
    // explicitly if you need to remove the message hook for some reason.
    bool remove();

    auto is_valid() const {
        return m_original_proc != nullptr;
    }

    auto get_original() const {
        return m_original_proc;
    }

    inline void window_toggle_cursor(bool show) {
        ::PostMessage(m_wnd, RE_TOGGLE_CURSOR, show, 1);
    }

    WindowsMessageHook& operator=(const WindowsMessageHook& other) = delete;
    WindowsMessageHook& operator=(const WindowsMessageHook&& other) = delete;

    static BOOL WINAPI onPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    static BOOL WINAPI onClipCursor(LPRECT lpRect);
    bool is_hook_intact();

private:
    HWND m_wnd;
    WNDPROC m_original_proc;
    std::unique_ptr<FunctionHook> m_post_message_hook{};
    std::unique_ptr<FunctionHook> m_clip_cursor_hook{};
};