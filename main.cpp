#include <Windows.h>
#include <iostream>
#include <thread>
 
#include "detours.h"
 
HWND hwnd;
 
static LRESULT(WINAPI* OldSendMessage)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = SendMessage;
LRESULT WINAPI NewSendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    MessageBox(hwnd, TEXT("hook掉了SendMessage,就不关闭了!"), TEXT("关闭程序"), MB_OK | MB_ICONINFORMATION);
    return 0;
}
 
void Hook() {
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //这里可以连续多次调用DetourAttach，表明HOOK多个函数
    DetourAttach(&(PVOID&)OldSendMessage, NewSendMessage);
    DetourTransactionCommit();
}
 
void UnHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //这里可以连续多次调用DetourDetach,表明撤销多个函数HOOK
    DetourDetach(&(PVOID&)OldSendMessage, NewSendMessage);
    DetourTransactionCommit();
}
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        // MessageBox(hwnd, TEXT("关闭程序!"), TEXT("结束"), MB_OK | MB_ICONINFORMATION);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
 
int main(void) {
    Hook();
    static TCHAR szAppName[] = TEXT("TextWindow");
    MSG msg;
    WNDCLASS wndclass;
    // wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;                         //窗口样式
    wndclass.lpszClassName = szAppName;                               //窗口类名
    wndclass.lpszMenuName = NULL;                                     //窗口菜单:无
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //窗口背景颜色
    wndclass.lpfnWndProc = WndProc;                                   //窗口处理函数
    wndclass.cbWndExtra = 0;                                          //窗口实例扩展:无
    wndclass.cbClsExtra = 0;                                          //窗口类扩展:无
    wndclass.hInstance = GetModuleHandle(0);                                   //窗口实例句柄
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);               //窗口最小化图标:使用缺省图标
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 //窗口采用箭头光标
    UnregisterClass(wndclass.lpszClassName, GetModuleHandle(0));
    if (!RegisterClass(&wndclass)) {    
        MessageBox(NULL, TEXT("窗口注册失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }
 
    hwnd = CreateWindow(szAppName, TEXT("测试窗口"), WS_OVERLAPPEDWINDOW,
        0, 0, 500, 400, NULL, NULL, GetModuleHandle(0), NULL);
 
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    std::thread close_work = std::thread([&]() {
        for (int i = 0; i < 5; ++i) {
            std::cout << "close work: " << 5 - i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        ::SendMessage(hwnd, WM_CLOSE, 0, 0);
        });
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (close_work.joinable()) {
        close_work.join();
    }
    std::cout << "Finish" << std::endl;
    UnHook();
    getchar();
    return 0;
}