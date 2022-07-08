%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
  mov byte [gs:0x0a],','
  mov byte [gs:0x0b],0xA4
  mov byte [gs:0x0c],'w'
  mov byte [gs:0x0d],0xA4
  mov byte [gs:0x0e],'o'
  mov byte [gs:0x0f],0xA4
  mov byte [gs:0x10],'r'
  mov byte [gs:0x11],0xA4
  mov byte [gs:0x12],'l'
  mov byte [gs:0x13],0xA4
  mov byte [gs:0x14],'d'
  mov byte [gs:0x15],0xA4

  jmp $
