%include "boot.inc"
SECTION loader vstart=LOADER_BASE_ADDR
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

  GDT_LIMIT equ GDT_SIZE - 1

  times 60 dq 0
  SELECTOR_CODE equ (0X0001<<3) + TI_GDT + RPL0
  SELECTOR_DATA equ (0X0002<<3) + TI_GDT + RPL0
  SELECTOR_VIDEO equ (0X0003<<3) + TI_GDT + RPL0

  gdt_ptr
  dw GDT_LIMIT
  dd GDT_BASE

  loadermsg db 'welcome to loader zone!'
    
loader_start:
  mov sp,LOADER_BASE_ADDR
  mov bp,loadermsg
  mov cx,22
  mov ax,cs
  mov es,ax
  mov ax,0x1301
  mov bx,0x001f
  mov dx,0x1800
  int 0x10

; --------------------------------- 设置进入保护模式 -----------------------------
; 1 打开A20 gate
; 2 加载gdt
; 3 将cr0 的 pe位置1
    
  in al,0x92                 ;端口号0x92 中 第1位变成1即可
  or al,0000_0010b
  out 0x92,al

  lgdt [gdt_ptr] 

  mov eax,cr0                ;cr0寄存器第0位设置位1
  or  eax,0x00000001              
  mov cr0,eax

;------------------------------ 已经打开保护模式 ---------------------------------------
  jmp dword SELECTOR_CODE:p_mode_start                       ;刷新流水线

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
