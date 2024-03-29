%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR

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

  total_mem_bytes dd 0

SELECTOR_CODE equ (0x0001 << 3) + TI_GDT
SELECTOR_DATA equ (0x0002 << 3) + TI_GDT
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT

  gdt_ptr dw GDT_LIMIT
          dd GDT_BASE
  success db "ok"
  fail db "fail"
  ards_buf times 238 db 0
  ards_nr dw 0

loader_start:
  xor ebx,ebx
  mov edx,0x534d4150
  mov di,ards_buf
.e820_mem_get_loop:
  mov eax,0x0000e820
  mov ecx,20
  int 0x15
  jc .e820_failed_so_try_e801
  add di,cx
  inc word [ards_nr]
  cmp ebx,0
  jnz .e820_mem_get_loop

  ;find out the max
  mov cx,[ards_nr]
  mov ebx,ards_buf
  xor edx,edx
.find_max_mem_area:
  mov eax,[ebx]
  add eax,[ebx+8]
  add ebx,20
  cmp edx,eax
  jge .next_ards
  mov edx,eax
.next_ards:
  loop .find_max_mem_area
  jmp .mem_get_ok

.e820_failed_so_try_e801:
  mov ax,0xe801
  int 0x15
  jc .e801_failed_so_try_88

  mov cx,0x400
  mul cx
  shl edx,16
  and eax,0x0000FFFF
  or edx,eax
  add edx,0x100000
  mov esi,edx

  xor eax,eax
  mov ax,bx
  mov ecx,0x1000
  mul ecx
  
  add esi,eax
  mov edx,esi
  jmp .mem_get_ok

.e801_failed_so_try_88:
  mov ah,0x88
  int 0x15
  jc .error_hlt
  and eax,0x0000FFFF

  mov cx,0x400
  mul cx
  shl edx,16
  or edx,eax
  add edx,0x100000
  jmp .mem_get_ok

.error_hlt:
  ;print message
  mov sp,LOADER_BASE_ADDR
  mov bp,fail
  mov cx,4
  mov ax,0x1301
  mov bx,0x001f
  mov dx,0x1800
  int 0x10
  jmp $

.mem_get_ok:
  mov [total_mem_bytes],edx

  ;print message
  mov sp,LOADER_BASE_ADDR
  mov bp,success
  mov cx,2
  mov ax,0x1301
  mov bx,0x001f
  mov dx,0x1800
  int 0x10

  ;enter protect mode
  in al,0x92
  or al,0x02
  out 0x92,al

  lgdt [gdt_ptr]

  mov eax,cr0
  or eax,0x01
  mov cr0,eax

  jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
setup_page:
  mov ecx,4096
  mov esi,0
.clear_page_dir:
  mov byte [PAGE_DIR_TABLE_POS + esi],0
  inc esi
  loop .clear_page_dir

  mov eax,PAGE_DIR_TABLE_POS
  add eax,0x1000
  mov ebx,eax
  or eax,PG_US_U | PG_RW_W | PG_P
  mov [PAGE_DIR_TABLE_POS + 0x0],eax
  mov [PAGE_DIR_TABLE_POS + 0xc00],eax
  sub eax,0x1000
  mov [PAGE_DIR_TABLE_POS + 4092],eax

  mov ecx,256
  mov esi,0
  mov edx,PG_US_U | PG_RW_W | PG_P
.create_pte:
  mov [ebx+esi*4],edx
  add edx,4096
  inc esi
  loop .create_pte

  mov eax,PAGE_DIR_TABLE_POS
  add eax,0x2000
  or eax, PG_US_U | PG_RW_W | PG_P
  mov ebx,PAGE_DIR_TABLE_POS
  mov ecx,254
  mov esi,769
.create_kernel_pde:
  mov [ebx+esi*4],eax
  inc esi
  add eax,0x1000
  loop .create_kernel_pde
  ret

p_mode_start:
  mov ax,SELECTOR_DATA
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov esp,LOADER_STACK_TOP
  mov ax,SELECTOR_VIDEO
  mov gs,ax

  call setup_page
  sgdt [gdt_ptr]
  mov ebx,[gdt_ptr + 2]
  or dword [ebx+0x18+4],0xc0000000
  add dword [gdt_ptr+2],0xc0000000
  add esp,0xc0000000
  mov eax,PAGE_DIR_TABLE_POS
  mov cr3,eax
  mov eax,cr0
  or eax,0x80000000
  mov cr0,eax
  lgdt [gdt_ptr]

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

