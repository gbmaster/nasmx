;//////////////////////////////////////////////////////////////
;//
;// NASMX Preprocessor
;// Copyright (c) 2005-2010, The NASMX Project
;//
%ifndef _NASMX_INC_
%define _NASMX_INC_

;//////////////////////////////////////////////////////////////
;//
;// Check NASM Version
;//
%define __nxerrmsg "NASM v2.08 or later required"
%ifdef __NASM_VERSION_ID__
	%if __NASM_VERSION_ID__ < 0x02075000
		%fatal __nxerrmsg
	%endif
%else
	%fatal __nxerrmsg
%endif
%undef __nxerrmsg

;//////////////////////////////////////////////////////////////
;//
;// Set a NASMX Definition
;//
%macro NASMX_DEFINE 1-2
	%if %0 > 1
		%ixdefine __NASMX_%1__ %2
	%else
		%ixdefine __NASMX_%1__
	%endif
%endmacro


;//////////////////////////////////////////////////////////////
;//
;// Get a NASMX Definition
;//
%macro NASMX_GET_DEFINE 2
	%ifdef __NASMX_%1__
		%xdefine %2 __NASMX_%1__
	%elifdef %2
		%undef %2
	%endif
%endmacro

;//////////////////////////////////////////////////////////////
;//
;// Undefine a NASMX Definition
;//
%macro NASMX_UNDEF 1
	%ifdef __NASMX_%1__
		%undef __NASMX_%1__
	%else
		%warning NASMX Definition %1 is not defined
	%endif
%endmacro


;//////////////////////////////////////////////////////////////
;//
;// Define NASMX Version
;//
NASMX_DEFINE VERSION,"20100801"


;//////////////////////////////////////////////////////////////
;//
;// init NASMX context stack
;//
;%push __NASMX_CTX__


;//////////////////////////////////////////////////////////////
;//
;// Set a Context Stack Value
;//
%macro NASMX_SET_CONTEXT_VALUE 2
	%xdefine %$$%1 %2
%endmacro


;//////////////////////////////////////////////////////////////
;//
;// native sizes
;//
%define byte__nx_tag_size 1
%define word__nx_tag_size 2
%define dword__nx_tag_size 4
%define qword__nx_tag_size 8
%define tword__nx_tag_size 10
%define oword__nx_tag_size 16


;//////////////////////////////////////////////////////////////
;//
;// nasm reserves
;//
%define byte__nx_tag_reserve RESB
%define word__nx_tag_reserve RESW
%define dword__nx_tag_reserve RESD
%define qword__nx_tag_reserve RESQ
%define tword__nx_tag_reserve REST
%define oword__nx_tag_reserve RESO

;//////////////////////////////////////////////////////////////
;//
;// nasm storage types
;//
%define byte__nx_tag_storage DB
%define word__nx_tag_storage DW
%define dword__nx_tag_storage DD
%define qword__nx_tag_storage DQ
%define tword__nx_tag_storage DT
%define oword__nx_tag_storage DO

;//////////////////////////////////////////////////////////////
;//
;// nasmx classifcation of types
;//
%define byte__nx_tag_type BYTE
%define word__nx_tag_type WORD
%define dword__nx_tag_type DWORD
%define qword__nx_tag_type QWORD
%define tword__nx_tag_type TWORD
%define oword__nx_tag_type OWORD


%define sizeof(x) x %+ _size
%define _nx_sizeof(x) x %+ __nx_tag_size
%define _nx_reserve(x) x %+ __nx_tag_reserve
%define _nx_typeof(x) x %+ __nx_tag_type

;//////////////////////////////////////////////////////////////
;//
;// NASMX typedef macro
;//
;// usage:
;//   typedef dword, int32_t
%imacro typedef 2.nolist
	%ifndef %{1}__nx_tag_size
			%fatal unknown tag size for %1
	%endif
;   %xdefine %{2}__nx_tag_size %{1}__nx_tag_size
;   %xdefine %{2}__nx_tag_type %{1}__nx_tag_type
;   %xdefine %{2}__nx_tag_reserve %{1}__nx_tag_reserve
;   %xdefine %{2}__nx_tag_storage %{1}__nx_tag_storage
   %xdefine %{2} %{1}
%endm


;//////////////////////////////////////////////////////////////
;//
;// NASMX int types
;//
typedef tword, tbyte  ; MASM Style TBYTE
typedef byte, int8_t
typedef byte, uint8_t
typedef word, int16_t
typedef word, uint16_t
typedef dword, int32_t
typedef dword, uint32_t
typedef qword, int64_t
typedef qword, uint64_t
typedef tbyte, int80_t
typedef tbyte, uint80_t
typedef oword, int128_t
typedef oword, uint128_t
typedef qword, long64_t
typedef qword, ulong64_t
typedef oword, long128_t
typedef oword, ulong128_t


;//////////////////////////////////////////////////////////////
;//
;// NASMX ABI size and pointer types
;//
typedef dword, ptr32_t
typedef qword, ptr64_t
;// use for 32->64 bit expansion of either int or ptr
%ixdefine NX_PTR ptr%[__BITS__]_t
typedef NX_PTR, size_t
%assign __NASMX_ABI_BITS__ _nx_sizeof(size_t) * 8
%ifnidn __BITS__,__NASMX_ABI_BITS__
	%fatal unknown size_t: __BITS__
%endif

;//////////////////////////////////////////////////////////////
;//
;// NASMX C types
;//
typedef byte, char_t
typedef byte, uchar_t
typedef word, short_t
typedef word, ushort_t
typedef dword, int_t
typedef dword, uint_t
typedef qword, long_t
typedef qword, ulong_t
typedef dword, float_t
typedef qword, double_t


;//////////////////////////////////////////////////////////////
;//
;// NASMX Unicode Transformation Format types
;//
typedef byte, utf8_t
typedef word, utf16_t
typedef dword, utf32_t


;//////////////////////////////////////////////////////////////
;//
;// NX_STRUC
;//
%imacro NX_STRUC 1-2.nolist 0
	struc %{1}

	%assign %$_nx_size 0

	;// do not define __nx_tag_type for structs
	%xdefine %{1}__nx_tag_name %{1}
	%xdefine %{1}__nx_tag_reserve RESB
	%xdefine %{1}__nx_tag_storage DB

%endmacro

;//////////////////////////////////////////////////////////////
;//
;// Set a NASMX field tag
;//
;// This macro creates a global define for structure fields
;//
;// usage:
;//    NX_DEFINE_FIELD_TAG struc_name, field_name, tag_name, tag args
;//
%imacro NX_DEFINE_FIELD_TAG 4.nolist
	%xdefine %{1}_field_%{2}__nx_tag_%{3} %{4}
;	%fatal tag %{3}: %{1}_field_%{2}__nx_tag_%{3} %{4}
%endmacro


;//////////////////////////////////////////////////////////////
;//
;// Set a NASMX field storage tag
;//
;// usage:
;//    NX_DEFINE_FIELD_STORAGE struc_name, var_name, storage_equivalent
%imacro NX_DEFINE_FIELD_STORAGE 3.nolist
	%ifndef %{3}__nx_tag_storage
		%fatal unknown storage tag for %{1}{%2}
	%endif
	NX_DEFINE_FIELD_TAG %{1}, %{2}, storage, %{3}__nx_tag_storage
%endmacro

;//////////////////////////////////////////////////////////////
;//
;// Set a NASMX field offset tag
;//
;// usage:
;//    NX_DEF_OFFSET struc_name, var_name, offset
%imacro NX_DEFINE_FIELD_OFFSET 3.nolist
	NX_DEFINE_FIELD_TAG %{1}, %{2}, offset, %{3}
%endmacro

%imacro NX_DEFINE_FIELD_NAME 2.nolist
	NX_DEFINE_FIELD_TAG %{1}, %{2}, name, %{2}
%endmacro

;// usage:
;//   NX_RESERVE .hWnd, NX_PTR, 1
;//   NX_RESERVE .pt, POINT, 1
%imacro NX_RESERVE 2-3.nolist 1
; struc creates it's own ctx!
;	%ifnctx __CTX_NX_STRUC
;		%fatal corrupt context
;	%endif
	%ifndef %{2}__nx_tag_reserve
		%fatal unknown reserve tag in %{$name} definition: %{2}
	%endif
	%ifndef %{2}__nx_tag_size
		%fatal unknown size tag in %{$name} definition: %{2}
	%endif

	NX_DEFINE_FIELD_NAME %{$name}, %{1}
	NX_DEFINE_FIELD_OFFSET %{$name}, %{1}, %{$size}
	NX_DEFINE_FIELD_STORAGE %{$name}, %{1}, %{2}

	%ifndef %{2}__nx_tag_type
		;// this is a struct param
		%assign %%_argsize %{2}_size
		%assign %%_qty %{3}
		%assign %%_amt (%%_argsize * %%_qty)
	%else
		%assign %%_argsize %{2}__nx_tag_size
		%assign %%_amt %{3}
	%endif
	;// calc running struct size
	%assign %$_nx_size (%$_nx_size + %%_argsize)

	%if %%_amt < 0
		%fatal negative quanity
	%endif
	%xdefine %$resline %{1}: %{2}__nx_tag_reserve %%_amt
	;%fatal resline = %{$resline}
%{1}: %{2}__nx_tag_reserve %%_amt
%endmacro

%imacro NX_ENDSTRUC 1.nolist
	%xdefine %{1}__nx_tag_size %$_nx_size

	endstruc
%endmacro

%imacro NX_ISTRUC 1-2.nolist 0
	%ifndef %{1}__nx_tag_name
		%fatal unknown name tag: %1
	%endif
	istruc %{1}

	%xdefine %$name %{1}
%endmacro

%imacro NX_DECLARE_STORAGE 3
	%xdefine %$atline at %{1}%{2}, %{1}_field_%{2}__nx_tag_storage %{3}
	%fatal atline: %$atline
%endmacro

%imacro NX_AT 2
	NX_DECLARE_STORAGE %{$name}, %{1}, %{2}
%endmacro

%imacro NX_IENDSTRUC 1.nolist
%undef %$name
iend
%endmacro

%ifidn	 __OUTPUT_FORMAT__,elf
	NASMX_DEFINE UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORMAT__,elf32
	NASMX_DEFINE UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORMAT__,elf64
	NASMX_DEFINE UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORMAT__,macho
	NASMX_DEFINE UNDERSCORE
	NASMX_DEFINE CDECL_UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORMAT__,macho32
	NASMX_DEFINE UNDERSCORE
	NASMX_DEFINE CDECL_UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORAMT__,macho64
	NASMX_DEFINE UNDERSCORE
	NASMX_DEFINE CDECL_UNDERSCORE
	typedef dword, wchar_t
%elifidn __OUTPUT_FORMAT__,win32
	NASMX_DEFINE UNDERSCORE
	typedef word, wchar_t
%elifidn __OUTPUT_FORMAT__,win64
	NASMX_DEFINE UNDERSCORE
	typedef word, wchar_t
%else
	NASMX_DEFINE UNDERSCORE
	NASMX_DEFINE CDECL_UNDERSCORE
	typedef dword, wchar_t
%endif

%ifdef UNICODE
	%idefine NX_CHAR wchar_t
%else
	%idefine NX_CHAR char_t
%endif

%imacro IMPORT 1-2.nolist
;	%warning 'DEBUG MACRO ENTRY(import):' %0 %1 %2
	%ifndef __imp_defined_%1
		%if %0 = 1
			%ifdef __NASMX_CDECL_UNDERSCORE__
				%define __cdecl_defined_%1 _%1
				%define __imp_defined_%1 _%1
			%else
				%define __cdecl_defined_%1 %1
				%define __imp_defined_%1 %1
			%endif
		%else
			%ifdef __NASMX_UNDERSCORE__
				%define __imp_defined_%1 _%1@%2
			%else
				%define __imp_defined_%1 %1@%2
			%endif
		%endif
	%else
		%error Import symbol %1 has already been defined
	%endif
;	%warning 'DEBUG MACRO EXIT(import):' %0 %1 %2
%endmacro


%imacro INVOKE 1-100.nolist
;	%warning 'DEBUG MACRO ENTRY(invoke):' %0 %1 %2
%push invoke
	%assign %$args 0
	%assign %$arg_rot -1

%ifdef __imp_defined_%1
	%ifndef __imp_declared_%1
		EXTERN	__imp_defined_%1
		%define __imp_declared_%1 %1
	%endif
	%define	__cur_import__
	%define	_proc __imp_defined_%1
%else
		%define _proc %1
%endif

%ifidn	__BITS__,64
	%ifidn	 __OUTPUT_FORMAT__,win64
		%ifdef __cur_import__
			%assign %$arg_rot 1
		%endif

		push rsp
		push QWORD[rsp]
		and spl,BYTE 0xF0

		%define %$arg_cur 0
		%define %$arg_typ 0

		%rep %0-1
			%rotate %$arg_rot
			%ifdef __cur_import__
				%if %$args < 32
					%if   %$args == 0
						%define %$arg_cur cx
						%define %$arg_typ 1
					%elif %$args == 8
						%define %$arg_cur dx
						%define %$arg_typ 1
					%elif %$args == 16
						%define %$arg_cur r8
						%define %$arg_typ 2
					%else
						%define %$arg_cur r9
						%define %$arg_typ 2
					%endif

					%ifnum %1
						%if %1 == -1
							%if %$arg_typ == 1
								or r %+ %$arg_cur, %1
							%else
								or %$arg_cur, %1
							%endif
						%elif %1 == 0
							%if %$arg_typ == 1
								xor e %+ %$arg_cur,e %+ %$arg_cur
							%else
								xor %$arg_cur %+ d, %$arg_cur %+ d
							%endif
						%else
							%if %1 > 2^32
								%if %$arg_typ == 1
									mov r %+ %$arg_cur, %1
								%else
									mov %$arg_cur, %1 
								%endif
							%else
								%if %$arg_typ == 1
									mov e %+ %$arg_cur, %1
								%else
									mov %$arg_cur %+ d, %1
								%endif
							%endif
						%endif
					%else
	        			%ifstr %1
							jmp %%endstr_%$args
							%%str_%$args: db %1, 0
							%%endstr_%$args:
							%if %$arg_typ == 1
								lea r %+ %$arg_cur,[rel %%str_%$args]
							%else
								lea %$arg_cur,[rel %%str_%$args]
							%endif
						%else
							%ifid %1
								%if %$arg_typ == 1
									lea r %+ %$arg_cur,[rel %1]
								%else
									lea %$arg_cur,[rel %1]
								%endif
							%else
								%if %$arg_typ == 1
									mov r %+ %$arg_cur,%1
								%else
									mov %$arg_cur,%1
								%endif
							%endif
						%endif
					%endif
		    	%else
					%ifstr %1
						jmp %%endstr_%$args
						%%str_%$args: db %1, 0
						%%endstr_%$args:
						push QWORD %%str_%$args
					%else
						push QWORD %1
					%endif
				%endif
			%else
				%ifstr %1
					jmp %%endstr_%$args
					%%str_%$args: db %1, 0
					%%endstr_%$args:
					push QWORD %%str_%$args
				%else
					push QWORD %1
				%endif
			%endif
			%assign %$args %$args+8
		%endrep
		sub	rsp,0x20
		call	_proc
		%if %$args <> 0
			%rotate %$arg_rot
			%ifdef %1_defined
				add	rsp, %$args
			%else
				%ifdef __cur_import__
					%if %$args > 32
						add	rsp,(0x28 + (%$args - 32))
					%else
						add	rsp,0x28
					%endif
				%else
					add	rsp,(0x28 + %$args)
				%endif
			%endif
		%else
			%ifndef %1_defined
				add	rsp,0x28
			%endif
		%endif
		pop	rsp
	%else
		%error "Unsupported 64-bit INVOKE"
	%endif
%else
	%rep %0-1
		%rotate %$arg_rot
		%ifstr	%1
			jmp	%%endstr_%$args
			%%str_%$args:	db %1, 0
			%%endstr_%$args:
			push	dword %%str_%$args
		%else
			push	dword %1
		%endif
		%assign %$args %$args+4
	%endrep
	call	_proc
	%if %$args <> 0
		%rotate %$arg_rot
		%ifdef __cdecl_defined_%1
			add	esp, %$args
		%endif
	%endif
%endif
%ifdef __cur_import__
	%undef __cur_import__
%endif
%undef	_proc
%pop
;%warning 'DEBUG MACRO EXIT(invoke):' %0 %1 %2
%endmacro


%imacro PROC 1-100.nolist
;	%warning 'DEBUG MACRO ENTRY(PROC):' %0 %1 %2
%push proc
	%ifidn __BITS__,64
		%assign %$arg 0x30
	%else
		%assign %$arg 8
	%endif
	%ifdef __entry_%1
		%ifndef __PROCEDURE__
			%ifdef __NASMX_UNDERSCORE__
				global	_main
				_main:
			%else
				global main
				main:
			%endif
			%define __PROCEDURE__ _end_%1
			%define _entry_
			%define __leave_present__
		%else
			%error "missing endproc directive."
		%endif
	%else
		%ifndef __PROCEDURE__
			%1:
			%ifidn __BITS__,64
				push rbp
				mov  rbp, rsp
			%else
				push ebp
				mov  ebp, esp
			%endif
			%define __PROCEDURE__ _end_%1
			%rep %0-1
				%rotate 1
				%1 argd
			%endrep
		%else
			%error "missing endproc directive."
		%endif
	%endif
;	%warning 'DEBUG MACRO EXIT(PROC):' %0 %1 %2
%endmacro


%imacro LOCALS 0.nolist
	%assign %$locnt 0
%endmacro


%imacro LOCAL 1-2.nolist
	%ifidni %2, qword
		%assign %$locnt 8+%$locnt
	%elifidni %2, dword
		%assign %$locnt 4+%$locnt
	%elifidni %2, word
		%assign %$locnt 2+%$locnt
	%elifidni %2, byte
		%assign %$locnt 1+%$locnt
	%endif
	%1 EQU %$locnt
%endmacro

%imacro ENDLOCALS 0.nolist
	%ifidn __BITS__,64
		sub rsp, %$locnt
	%else
		sub esp, %$locnt
	%endif
%endmacro


%imacro ARGD 0-1.nolist
	%ifdef __PROCEDURE__
		%00 equ %$arg
		%if %0 = 1
			%assign %$arg %1+%$arg
		%else
			%ifidn __BITS__,64
				%assign %$arg 8+%$arg
			%else
				%assign %$arg 4+%$arg
			%endif
		%endif
	%else
		%error "arguments must be defined within a proc/endproc block."
	%endif
%endmacro


%ifidn __BITS__,64
	%idefine VAR(v) [rbp-v]
	%idefine ARGV(v) [rbp+v]	;RCX/RDX/R8/R9...
%else
	%idefine VAR(v)	[ebp-v]
	%idefine ARGV(v) [ebp+v]
%endif


%idefine OFFSET


%imacro ENDPROC 0.nolist
;	%warning 'DEBUG MACRO ENTRY(ENDPROC):'
	%ifdef __PROCEDURE__
		__PROCEDURE__:
		%ifndef _entry_
			%pop
		%else
			%undef _entry_
		%endif
		%undef __PROCEDURE__
	%else
		%error "missing proc directive."
	%endif
;	%warning 'DEBUG MACRO ENTRY(ENDPROC):'
%endmacro


%imacro LEAVE 0.nolist
	%ifndef __leave_present__
		%define __leave_present__
	%endif
	%ifidn __BITS__,64
		mov rsp, rbp
		pop rbp
	%else
		mov esp, ebp
		pop ebp
	%endif
%endmacro


%imacro RET 0-1.nolist
	%ifdef __PROCEDURE__
		%ifndef __leave_present__
			%ifctx proc
				leave
				ret %1
				%undef __leave_present__
			%else
				ret %1
			%endif
		%else
			ret %1
			%undef __leave_present__
		%endif
	%else
		ret
	%endif
%endmacro


%imacro PROTO 1-2.nolist
	%if %0 = 1
		%ifdef __NASMX_CDECL_UNDERSCORE__
			global _%1
		%else
			global %1
		%endif
	%else
		%ifdef __NASMX_UNDERSCORE__
			global _%1
		%else
			global %1
		%endif
	%endif
%endmacro

%imacro ENTRY 1.nolist
	%ifdef __NASMX_UNDERSCORE__
		%define __entry_%1 _main
	%else
		%define __entry_%1 main
	%endif
%endmacro


%imacro RJMP 2.nolist
; This macro preforms the opposite conditional jump
; than what the value suggests, it's used internally
; by other macros
	%ifidni %1, ==
		jne %2
	%elifidni %1, >
		jle %2
	%elifidni %1, <
		jge %2
	%elifidni %1, >=
		jl %2
	%elifidni %1, <=
		jg %2
	%elifidni %1, !=
		je %2
	%elifidni %1, !>
		jg %2
	%elifidni %1, !<
		jl %2
	%elifidni %1, CARRY
		jnc %2
	%elifidni %1, BELOW
		jnb %2
	%elifidni %1, ABOVE
		jna %2
	%elifidni %1, PARITY
		jnp %2
	%elifidni %1, SIGNED
		jns %2
	%elifidni %1, OVERFLOW
		jno %2
	%elifidni %1, !CARRY
		jc %2
	%elifidni %1, !BELOW
		jb %2
	%elifidni %1, !ABOVE
		ja %2
	%elifidni %1, !PARITY
		jp %2
	%elifidni %1, !SIGNED
		js %2
	%elifidni %1, !OVERFLOW
		jo %2
	%endif
%endmacro


%imacro SJMP 2.nolist
	%ifidni %1, ==
		je %2
	%elifidni %1, >
		jg %2
	%elifidni %1, <
		jl %2
	%elifidni %1, >=
		jge %2
	%elifidni %1, <=
		jle %2
	%elifidni %1, !=
		jne %2
	%elifidni %1, !>
		jng %2
	%elifidni %1, !<
		jnl %2
	%elifidni %1, CARRY
		jc %2
	%elifidni %1, BELOW
		jb %2
	%elifidni %1, ABOVE
		ja %2
	%elifidni %1, PARITY
		jp %2
	%elifidni %1, SIGNED
		js %2
	%elifidni %1, OVERFLOW
		jo %2
	%elifidni %1, !CARRY
		jnc %2
	%elifidni %1, !BELOW
		jnb %2
	%elifidni %1, !ABOVE
		jna %2
	%elifidni %1, !PARITY
		jnp %2
	%elifidni %1, !SIGNED
		jns %2
	%elifidni %1, !OVERFLOW
		jno %2
	%endif
%endmacro


%imacro BREAK 0.nolist
	jmp %$break
%endmacro


%imacro IF 3.nolist
%push IF
	%assign %$next 1
	cmp %1, %3
	RJMP %2, %$local_%$next
%endmacro


%imacro ELSIF 3.nolist
	%ifctx IF
		jmp %$endif
		%$local_%$next:
		%assign %$next 1+%$next
		cmp %1, %3
		RJMP %2, %$local_%$next
	%else
		%error "ELSIF directive must be within an IF block."
	%endif
%endmacro


%imacro ELSE 0.nolist
	%ifndef _else_
		%ifctx IF
			jmp %$endif
			%$local_%$next:
			%assign %$next 1+%$next
			%define _else_
		%else
			%error "ELSE directive must be within an IF block."
		%endif
	%else
		%error "There can only be one ELSE per IF block."
	%endif
%endmacro


%imacro ENDIF 0.nolist
	%ifctx IF
		%ifndef _else_
			%$local_%$next:
		%else
			%undef _else_
		%endif
		%$endif:
		%pop
	%else
		%error "ENDIF directive must be within an IF block."
	%endif
%endmacro


%imacro SWITCH 1.nolist
%push SWITCH
	mov EDX, %1
	%assign %$next 1
%endmacro


%imacro CASE 1.nolist
	%ifctx SWITCH
		%$local_%$next:
		%assign %$next 1+%$next
		%ifidn __BITS__,64
			cmp rdx, %1
		%else
			cmp edx, %1
		%endif
		jnz near %$local_%$next
	%else
		%error "CASE directive must be within a SWITCH block."
	%endif
%endmacro


%imacro DEFAULT 0.nolist
	%ifctx SWITCH
		%define _default_
		%$local_%$next:
	%else
		%error "DEFAULT directive must be within a SWITCH block."
	%endif
%endmacro


%imacro ENDSWITCH 0.nolist
	%ifndef _default_
		%$local_%$next:
	%else
		%undef _default_
	%endif
	%$break:
%pop
%endmacro

%imacro DO 0.nolist
%push DO
	%$begin:
%endmacro


%imacro UNTIL 3.nolist
	%ifctx DO
		cmp %1, %3
		RJMP %2, %$begin
		%$break:
	%else
		%error "UNTIL directive must be preceded by DO."
	%endif
%pop
%endmacro


%imacro WHILE 3.nolist
	%ifctx DO
		cmp %1, %3
		SJMP %2, %$begin
		%$break:
	%else
		%error "WHILE directive must be preceded by DO."
	%endif
%pop
%endmacro

%endif	;NASMX_INC
