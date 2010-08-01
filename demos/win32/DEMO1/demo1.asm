%include '..\..\..\inc\nasmx.inc'
%include '..\..\..\inc\win32\windows.inc'
%include '..\..\..\inc\win32\kernel32.inc'
%include '..\..\..\inc\win32\user32.inc'

entry    demo1

[section .text]
proc     demo1
%ifidn __BITS__,32
    invoke    my_p, NX_PTR szContentTwo, NX_PTR szTitleTwo
%endif
    invoke    MessageBoxA, NX_PTR NULL, NX_PTR szContent, NX_PTR szTitle, uint32_t MB_OK
    invoke    ExitProcess, NX_PTR NULL
    ret

endproc

%ifidn __BITS__,32
proc     my_p
sz_Content    argd
sz_Title      argd

    invoke    MessageBoxA, NX_PTR NULL, NX_PTR argv(sz_Content), NX_PTR argv(sz_Title), uint32_t MB_OK
    ret

endproc
%endif

_data
    szTitle:       db   'Demo1', 0x0
    szTitleTwo:    db   'Demo1 Procedure', 0x0
    szContent:     db   'Hello from the Application!', 0x0
    szContentTwo:  db   'Hello from the Procedure!', 0x0
