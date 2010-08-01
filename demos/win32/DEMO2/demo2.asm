%include '..\..\..\inc\nasmx.inc'
%include '..\..\..\inc\win32\windows.inc'
%include '..\..\..\inc\win32\kernel32.inc'
%include '..\..\..\inc\win32\user32.inc'

entry demo2

[section .text]
proc    demo2

    invoke   GetModuleHandleA, dword NULL
    mov      [hInstance], eax
    invoke   WinMain, dword hInstance, dword NULL, dword NULL, dword SW_SHOWNORMAL
    invoke   ExitProcess, dword NULL
    ret

endproc

proc    WinMain
hinst    argd        ; Current instance handle
hpinst   argd        ; Previous instance handle
cmdln    argd        ; Command line arguments
dwshow   argd        ; Display style

    invoke   LoadIconA, dword NULL, dword IDI_APPLICATION
    mov      edx, eax
    mov      eax, dword argv(hinst)
    mov      ebx, dword szClass
    mov      ecx, dword WndProc
    mov      [wc + WNDCLASSEX.hInstance], eax
    mov      [wc + WNDCLASSEX.lpszClassName], ebx
    mov      [wc + WNDCLASSEX.lpfnWndProc], ecx
    mov      [wc + WNDCLASSEX.hIcon], edx
    mov      [wc + WNDCLASSEX.hIconSm], edx

    invoke   RegisterClassExA, dword wc

    invoke   CreateWindowExA, dword WS_EX_TOOLWINDOW, dword szClass, dword szTitle, dword WS_CAPTION + WS_SYSMENU + WS_VISIBLE, dword 100, dword 120, dword 200, dword 100, dword NULL, dword NULL, dword [wc + WNDCLASSEX.hInstance], dword NULL
    mov      [hWnd], eax

    invoke   ShowWindow, dword hWnd, dword argv(dwshow)
    invoke   UpdateWindow, dword hWnd

    .msgloop:
        invoke   GetMessageA, dword message, dword NULL, dword NULL, dword NULL
        cmp      eax, dword 0
        je       .exit
        invoke   TranslateMessage, dword message
        invoke   DispatchMessageA, dword message
        jmp      .msgloop
    .exit:

    mov      eax, dword [message + MSG.wParam]
    ret

endproc

proc    WndProc
hwnd    argd        ; Window handle
umsg    argd        ; Window message
wparam  argd        ; wParam
lparam  argd        ; lParam


.wm_create:
    cmp      argv(umsg), dword WM_CREATE
    jnz      .wm_destroy

    invoke   GetClientRect, dword argv(hwnd), dword rct
    invoke   CreateWindowExA, dword NULL, dword szStatic, dword szTitle, dword WS_CHILD + WS_VISIBLE + SS_CENTER, dword 0, dword 0, dword [rct + RECT.right], dword [rct + RECT.bottom], dword argv(hwnd), dword 500, dword [wc + WNDCLASSEX.hInstance], dword NULL
    jmp      .wm_default

.wm_destroy:
    cmp      argv(umsg), dword WM_DESTROY
    jnz      .wm_default

    invoke   PostQuitMessage, dword NULL

.wm_default:
    invoke   DefWindowProcA, dword argv(hwnd), dword argv(umsg), dword argv(wparam), dword argv(lparam)
    
.exit:
    ret

endproc

[section .bss]
    hInstance:   resd 1
    hWnd:        resd 1

[section .data]
    szStatic:   db    "STATIC", 0x0
    szTitle:    db    "Demo2", 0x0
    szClass:    db    "Demo2Class", 0x0

    wc:
    NX_ISTRUC WNDCLASSEX
		NX_AT .style, CS_VREDRAW + CS_HREDRAW
		NX_AT .cbSize, WNDCLASSEX_size
		NX_AT .lpfnWndProc,   NULL
		NX_AT .cbClsExtra,    NULL
		NX_AT .cbWndExtra,    NULL
		NX_AT .hInstance,     NULL
		NX_AT .hIcon,         NULL
		NX_AT .hCursor,       NULL
		NX_AT .hbrBackground, COLOR_BTNFACE + 1
		NX_AT .lpszMenuName,  NULL
		NX_AT .lpszClassName, NULL
		NX_AT .hIconSm,       NULL
    NX_IENDSTRUC WNDCLASSEX
	
    message:
    NX_ISTRUC MSG
        NX_AT .hwnd,        NULL
        NX_AT .message,     NULL
        NX_AT .wParam,      NULL
        NX_AT .lParam,      NULL
        NX_AT .time,        NULL
        NX_AT .pt,          NULL
    NX_IENDSTRUC MSG
	
    rct:
    NX_ISTRUC RECT
        NX_AT .left,        NULL
        NX_AT .top,         NULL
        NX_AT .right,       NULL
        NX_AT .bottom,      NULL
    NX_IENDSTRUC RECT
