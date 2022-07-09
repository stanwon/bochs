%include "boot.inc"
section mbr vstart=0x7c00
  mov ax,cs
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov fs,ax
  mov sp,0x7c00
  mov ax,0xb800
  mov gs,ax

  mov ax,0x600
  mov bx,0x700
  mov cx,0
  mov dx,0x184f
  int 0x10

  mov ah,3
  mov bh,0
  int 0x10

 mov eax,LOADER_START_SECTOR
 mov bx,LOADER_BASE_ADDR
 mov cx,4
 call rd_disk_m_16

 jmp LOADER_BASE_ADDR + 0x300

rd_disk_m_16:
 mov esi,eax
 mov di,cx

 mov dx,0x1f2
 mov al,cl
 out dx,al
 
 mov dx,0x1f3
 mov eax,esi
 out dx,al

 mov dx,0x1f4
 mov cl,8
 shr eax,cl
 out dx,al

 mov dx,0x1f5
 shr eax,cl
 out dx,al

 mov dx,0x1f6
 shr eax,cl
 and al,0x0f
 or al,0xe0
 out dx,al

 mov dx,0x1f7
 mov al,0x20
 out dx,al
.not_read:
 nop
 in al,dx
 and al,0x88
 cmp al,0x08
 jnz .not_read

 mov ax,di
 mov dx,256
 mul dx
 mov cx,ax
 mov dx,0x1f0
.go_on_read:
 in ax,dx
 mov [bx],ax
 add bx,2
 loop .go_on_read
 ret


 jmp $
 times 510-($-$$) db 0
 db 0x55,0xaa

