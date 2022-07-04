%include "boot.inc"
SECTION loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR
jmp start

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

  GDT_LIMIT equ GDT_SIZE - 1

  times 60 dq 0
  SELECTOR_CODE equ (0X0001<<3) + TI_GDT + RPL0
  SELECTOR_DATA equ (0X0002<<3) + TI_GDT + RPL0
  SELECTOR_VIDEO equ (0X0003<<3) + TI_GDT + RPL0

  total_mem_bytes dd 0

  gdt_ptr dw GDT_LIMIT
          dd GDT_BASE

  ards_buf times 244 db 0
  ards_nr dw 0

start:
  


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

  mov byte [gs:160],'P'

  jmp $
