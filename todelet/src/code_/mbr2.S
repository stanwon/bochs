SECTION MBR VSTART=0x7c00
  mov ax,cs
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov fs,ax
  mov sp,0x7c00
  mov ax,0xb800
  mov gs,ax

  mov ax, 0x600
  mov bx, 0x700
  mov cx, 0
  mov dx, 0x184f

  int 0x10

  mov byte [gs:0x012],'h'
  mov byte [gs:0x013],0xA4

  mov byte [gs:0x014],'e'
  mov byte [gs:0x015],0xA4

  mov byte [gs:0x016],'l'
  mov byte [gs:0x017],0xA4

  mov byte [gs:0x018],'l'
  mov byte [gs:0x019],0xA4

  mov byte [gs:0x01a],'o'
  mov byte [gs:0x01b],0xA4

  jmp $

  times 510-($-$$) db 0
  db 0x55,0xaa
