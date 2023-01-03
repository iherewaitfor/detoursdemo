#include <Windows.h>
#include <iostream>
#include <thread>
 
#include "detours.h"
 
HWND hwnd;
 
static LRESULT(WINAPI* OldSendMessage)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = SendMessage;
LRESULT WINAPI NewSendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    MessageBox(hwnd, TEXT("hook����SendMessage,�Ͳ��ر���!"), TEXT("�رճ���"), MB_OK | MB_ICONINFORMATION);
    return 0;
}
 
void Hook() {
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //�������������ε���DetourAttach������HOOK�������
    DetourAttach(&(PVOID&)OldSendMessage, NewSendMessage);
    DetourTransactionCommit();
}
 
void UnHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //�������������ε���DetourDetach,���������������HOOK
    DetourDetach(&(PVOID&)OldSendMessage, NewSendMessage);
    DetourTransactionCommit();
}
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        // MessageBox(hwnd, TEXT("�رճ���!"), TEXT("����"), MB_OK | MB_ICONINFORMATION);
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
    wndclass.style = CS_HREDRAW | CS_VREDRAW;                         //������ʽ
    wndclass.lpszClassName = szAppName;                               //��������
    wndclass.lpszMenuName = NULL;                                     //���ڲ˵�:��
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //���ڱ�����ɫ
    wndclass.lpfnWndProc = WndProc;                                   //���ڴ�����
    wndclass.cbWndExtra = 0;                                          //����ʵ����չ:��
    wndclass.cbClsExtra = 0;                                          //��������չ:��
    wndclass.hInstance = GetModuleHandle(0);                                   //����ʵ�����
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);               //������С��ͼ��:ʹ��ȱʡͼ��
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 //���ڲ��ü�ͷ���
    UnregisterClass(wndclass.lpszClassName, GetModuleHandle(0));
    if (!RegisterClass(&wndclass)) {    
        MessageBox(NULL, TEXT("����ע��ʧ��"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }
 
    hwnd = CreateWindow(szAppName, TEXT("���Դ���"), WS_OVERLAPPEDWINDOW,
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