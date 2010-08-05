;// DEMO10.ASM
;//
;// Copyright (C)2005-2010 The NASMX Project
;//
;// This is a fully UNICODE aware, typedefined demo that demonstrates
;// using NASMX typedef system to make your code truly portable between
;// 32 and 64-bit systems using either ASCII or UNICODE
;//
;// Contributors:
;//    Bryant Keller
;//    Rob Neff
;//
%include '..\..\..\inc\nasmx.inc'
%include '..\..\..\inc\win32\windows.inc'
%include '..\..\..\inc\win32\kernel32.inc'
%include '..\..\..\inc\win32\user32.inc'
%include '..\..\..\inc\win32\stdwin.inc'
;// You must include the following when using typedef function names
;// for either ASCII or Unicode
;// eg: MessageBox is an alias for MessageBoxW or MessageBoxA
;//     depending on whether UNICODE is defined or not
%include '..\..\..\inc\win32\unicode.inc'

entry    demo10

[section .text]
proc    demo10

    invoke   GetModuleHandle, NASMX_PTR NULL
    mov      [hInstance], eax
    invoke   WinMain, NASMX_PTR hInstance, NASMX_PTR NULL, NASMX_PTR NULL, int32_t SW_SHOWNORMAL
    invoke   ExitProcess, uint32_t NULL
    ret

endproc

proc    WinMain, hinst, hpinst, cmdln, dwshow

    invoke   LoadIcon, NASMX_PTR NULL, NASMX_PTR IDI_APPLICATION
    mov      edx, eax
    mov      eax, dword argv(hinst)
    mov      ebx, dword szClass
    mov      ecx, dword WndProc
    mov      [wc + WNDCLASSEX.hInstance], eax
    mov      [wc + WNDCLASSEX.lpszClassName], ebx
    mov      [wc + WNDCLASSEX.lpfnWndProc], ecx
    mov      [wc + WNDCLASSEX.hIcon], edx
    mov      [wc + WNDCLASSEX.hIconSm], edx

    invoke   RegisterClassEx, NASMX_PTR wc

    StdWindow szClass, szTitle, 100, 120, 212, 232, NULL, [wc + WNDCLASSEX.hInstance]
    mov      [hWnd], eax

    invoke   ShowWindow, NASMX_PTR hWnd, int32_t argv(dwshow)
    invoke   UpdateWindow, NASMX_PTR hWnd

    do
        invoke   TranslateMessage, NASMX_PTR message
        invoke   DispatchMessage, NASMX_PTR message
        invoke   GetMessage, NASMX_PTR message, NASMX_PTR NULL, uint32_t NULL, uint32_t NULL
    while eax, !=, 0


    mov      eax, dword [message + MSG.wParam]
    ret

endproc

proc    WndProc, hwnd, umsg, wparam, lparam


    switch dword argv(umsg)
    case dword WM_CREATE

        ButtonCtl szStringA, 500, 0, 0, 100, 40, argv(hwnd), [wc + WNDCLASSEX.hInstance]
        ButtonCtl szStringB, 501, 100, 0, 100, 40, argv(hwnd), [wc + WNDCLASSEX.hInstance]
        StaticCtl szContent, 502, 0, 40, 200, 40, argv(hwnd), [wc + WNDCLASSEX.hInstance]
        ComboCtl szContent, 503, 0, 80, 200, 100, argv(hwnd), [wc + WNDCLASSEX.hInstance]
        ListBoxCtl szContent, 504, 0, 106, 200, 100, argv(hwnd), [wc + WNDCLASSEX.hInstance]

    case dword WM_COMMAND
        if argv(wparam), ==, dword 500
            invoke   MessageBox, NASMX_PTR NULL, NASMX_PTR szMessageA, NASMX_PTR szTitle, uint32_t MB_OK
        elsif argv(wparam), ==, dword 501
            invoke	MessageBox, NASMX_PTR NULL, NASMX_PTR szMessageB, NASMX_PTR szTitle, uint32_t MB_OK
        else
            jmp near .defwndproc
        endif

    case dword WM_DESTROY

        invoke   PostQuitMessage, int32_t NULL

    default
        .defwndproc:
        invoke   DefWindowProc, NASMX_PTR argv(hwnd), uint32_t argv(umsg), size_t argv(wparam), size_t argv(lparam)

    endswitch
    ret

endproc

[section .bss]
    hInstance:   reserve(NASMX_PTR) 1
    hWnd:        reserve(NASMX_PTR) 1

[section .data]
    szStringA:  declare(NASMX_CHAR) NASMX_TEXT("Click Me!"), 0x0
    szStringB:  declare(NASMX_CHAR) NASMX_TEXT("Me Too!"), 0x0
    szContent:  declare(NASMX_CHAR) NASMX_TEXT("Win32Nasm Demo #10"), 0x0
    szMessageA: declare(NASMX_CHAR) NASMX_TEXT("With the NasmX Library..."), 0x0
    szMessageB: declare(NASMX_CHAR) NASMX_TEXT("Assembly is a cinche!"), 0x0
    szTitle:    declare(NASMX_CHAR) NASMX_TEXT("Demo10"), 0x0
    szClass:    declare(NASMX_CHAR) NASMX_TEXT("Demo10Class"), 0x0

    NASMX_ISTRUC wc, WNDCLASSEX
        NASMX_AT cbSize,           WNDCLASSEX_size
        NASMX_AT style,            CS_VREDRAW + CS_HREDRAW
        NASMX_AT lpfnWndProc,      NULL
        NASMX_AT cbClsExtra,       NULL
        NASMX_AT cbWndExtra,       NULL
        NASMX_AT hInstance,        NULL
        NASMX_AT hIcon,            NULL
        NASMX_AT hCursor,          NULL
        NASMX_AT hbrBackground,    COLOR_BTNFACE + 1
        NASMX_AT lpszMenuName,     NULL
        NASMX_AT lpszClassName,    NULL
        NASMX_AT hIconSm,          NULL
    NASMX_IENDSTRUC

    NASMX_ISTRUC message, MSG
        NASMX_AT hwnd,             NULL
        NASMX_AT message,          NULL
        NASMX_AT wParam,           NULL
        NASMX_AT lParam,           NULL
        NASMX_AT time,             NULL
		NASMX_ISTRUC pt, POINT
			NASMX_AT x,          NULL
			NASMX_AT y,          NULL
		NASMX_IENDSTRUC
    NASMX_IENDSTRUC
