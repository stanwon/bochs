%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR

  jmp loader_start

GDT_BASE:
  dd 0x00000000
  dd 0x00000000
CODE_DESC:
  dd 0x0000FFFF
  dd DESC_CODE_HIGH4
DATA_STACK_DESC:
  dd 0x0000FFFF
  dd DESC_DATA_HIGH4
VIDEO_DESC:
  dd 0x80000007
  dd DESC_VIDEO_HIGH4

GDT_SIZE equ $ - GDT_BASE
GDT_LIMIT equ GDT_SIZE -1

  times 60 dq 0

SELECTOR_CODE equ (0x0001 << 3) + TI_GDT
SELECTOR_DATA equ (0x0002 << 3) + TI_GDT
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT

  gdt_ptr dw GDT_LIMIT
          dd GDT_BASE
loader_msg db "hello,gdt"


loader_start:
  mov sp,LOADER_BASE_ADDR
  mov bp,loader_msg
  mov cx,9
  mov ax,0x1301
  mov bx,0x001f
  mov dx,0x1800
  int 0x10
  
  in al,0x92
  or al,0x02
  out 0x92,al

  lgdt [gdt_ptr]

  mov eax,cr0
  or eax,0x01
  mov cr0,eax

  jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
  mov ax,SELECTOR_DATA
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov esp,LOADER_STACK_TOP
  mov ax,SELECTOR_VIDEO
  mov gs,ax

  mov byte [gs:0x00],'h'
  mov byte [gs:0x01],0xA4
  mov byte [gs:0x02],'e'
  mov byte [gs:0x03],0xA4
  mov byte [gs:0x04],'l'
  mov byte [gs:0x05],0xA4
  mov byte [gs:0x06],'l'
  mov byte [gs:0x07],0xA4
  mov byte [gs:0x08],'o'
  mov byte [gs:0x09],0xA4


  jmp $
