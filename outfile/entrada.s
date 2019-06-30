section	.	text
global _start
_start:
	push  eax
	mov  eax, [b	a]
	mov [a] ,eax
	pop  eax
	push eax
	mov eax, 4
	mov ebx, 1
	mov ecx, msg_input
	mov edx, tam_msgin
	int 80h
	mov eax, 3
	mov ebx, 0
	mov ecx, num_input
	mov edx, 12
	int 80h
	push  eax
	push num_input
	push eax
	call input
	mov [b], eax
	pop eax
	pop  eax
	push eax
	mov eax, 4
	mov ebx, 1
	mov ecx, msg_input
	mov edx, tam_msgin
	int 80h
	mov eax, 3
	mov ebx, 0
	mov ecx, num_input
	mov edx, 12
	int 80h
	push  eax
	push num_input
	push eax
	call input
	mov [h], eax
	pop eax
	pop  eax
	mov eax, [b]
	add eax, [h]
	mov [r], eax
	push  eax
	push eax
	push num_input
	push dword, [r]
	mov eax, 4
	mov ebx, 1
	mov ecx, msg_output
	mov edx, tam_msgout
	int 80h
	call output
	mov  eax,  4
	mov  ebx,  1
	mov  ecx,  endl
	mov  edx,  tam_endl
	int  80h
	pop  eax
	pop eax
fim_prog:
	mov eax,  1
	mov ebx, 0
	int 80h



input:
	enter 0,0
	mov ecx, [ebp+8]
	dec ecx
	xor esi, esi
	xor edi, edi
	xor eax, eax
	mov ebx, [ebp+12]
	cmp byte [ebx], 0x2D
	jnz loopinput
	dec ecx
	inc esi
	inc edi
loopinput:
	cmp ecx, 1
	jl fim_linput
converte:
	xor edx, edx
	mov dl, [ebx+esi]
	sub dl, 0x30
	push ebx
	mov ebx, eax
	add eax, ebx
	add eax, ebx
	add eax, ebx
	add eax, ebx
	sal eax, 1
	mov ebx, edx
	mov edx, ebx
	add eax, edx
	pop ebx
	inc esi
	dec ecx
	jmp loopinput
fim_linput:
	cmp edi, 1
	jnz positivo_in
negativo_in:
	neg eax
positivo_in:
	mov [ebp+16], esi
	leave
	ret 8



output:
	enter 0,0
	mov  eax,  [ebp+8]
	mov  edi,  [ebp+12]
	xor  edx,  edx
	xor  ecx,  ecx
	xor  esi,  esi
	mov  ebx,  10
	cmp  eax,  0
	jge  divide
	neg  eax
	add  esi,  1
divide:
	div  ebx
	add  edx,  0x30
	mov  byte  [edi+ecx],  dl
	mov  edx,  0
	cmp  eax,  0
	je  fim_d
	inc  ecx
	jmp  divide
fim_d:
	mov  [ebp+16],  ecx
	cmp  esi,  1
	jne  positivo_out
	inc  ecx
	mov  byte  [edi+ecx],  0x2D
positivo_out:
	inc  dword  [ebp+16]
	cmp  ecx,  0
	jl  fimfunc
	push  ecx
	mov  eax,  4
	mov  ebx,  1
	mov  esi,  ecx
	mov  ecx,  edi
	add  ecx, esi
	mov  edx,  1
	int  80h
	pop  ecx
	dec  ecx
	jmp  positivo_out
fimfunc:
	leave
	ret  8



s_input:
	enter 0,0
	mov  eax,  3
	mov  ebx,  0
	mov  ecx,  [ebp+16]
	mov  edx,  100
	int  80h
	mov  ebx,  [ebp+16]
	mov  eax,  [ebp+12]
	mov  ecx,  [ebp+8]
	xor  esi,  esi
transcreve:
	cmp  ecx,  1
	jl  fim_transcreve
	mov  dl,  [ebx+esi]
	mov  [eax+esi],  dl
	inc  esi
	dec  ecx
	jmp  transcreve
fim_transcreve:
	mov  byte  [eax+esi],  0ah
	mov  [ebp+20],  esi
	leave
	ret  12



s_output:
	enter  0,0
	mov  ebx,  [ebp+12]
	mov  esi,  [ebp+8]
	xor  ecx,  ecx
printa:
	cmp  byte  [ebx+ecx],  0ah
	jz  fim_printa
	cmp  esi,  1
	jl  fim_printa
	push  ecx
	push  ebx
	mov  eax,  4
	add  ecx,  ebx
	mov  ebx,  1
	mov  edx,  1
	int  80h
	pop  ebx
	pop  ecx
	dec  esi
	inc  ecx
	jmp  printa
fim_printa:
	mov  [ebp+16],  ecx
	leave
	ret  8



c_input:
	enter  0,0
	mov  eax,  3
	mov  ebx,  0
	mov  ecx,  [ebp+12]
	mov  edx,  100
	int  80h
	mov  ecx,  [ebp+12]
	mov  al,  [ecx]
	mov  ebx,  [ebp+8]
	mov  [ebx],  al
	mov  dword  [ebp+16],  1
	leave
	ret  8



c_output:
	enter  0,0
	mov  eax,  4
	mov  ebx,  1
	mov  ecx,  [ebp+8]
	mov  edx,  1
	int  80h
	mov  dword  [ebp+12],  1
	leave
	ret  4



multiplica:
	enter  0,0
	mov  eax,  [ebp+12]
	mov  ebx,  [ebp+8]
	imul  ebx
	cmp  edx,  0
	jnz  testa1
	jmp  nao_overflow
testa1:
	cmp  edx,  -1
	jz  nao_overflow
	mov  eax,  4
	mov  ebx,  1
	mov  ecx,  msgoverflow
	mov  edx,  tam_msgover
	int  80h
	jmp  fim_prog
nao_overflow:
	leave
	ret  8



section	.	bss
	num_input resb 10
	char_in	resb	1
	string_in	resb	100
	b	resd	1
	h	resd	1
	r	resd	2
section	.	data
	msg_input	db	'input: ' 
	tam_msgin	EQU	$-msg_input
	msg_output	db	'output: ' 
	tam_msgout	EQU	$-msg_output
	msg_cinput	db	'c_input: '
	tam_msgcin	EQU	$-msg_cinput
	msg_coutput	db	'c_output: '
	tam_msgcout	EQU	$-msg_coutput
	msg_sinput	db	's_input: '
	tam_msgsin	EQU	$-msg_sinput
	msg_soutput	db	's_output: '
	tam_msgsout	EQU	$-msg_soutput
	endl	db	0dh,0ah
	tam_endl	EQU	$-endl
	msgoverflow	db	'OVERFLOW!'
	tam_msgover	EQU	$-msgoverflow
	dois	dd	2
