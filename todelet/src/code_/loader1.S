%include "boot.inc"
SECTION loader vstart=LOADER_BASE_ADDR
  mov byte [gs:0x0a],'h'
  mov byte [gs:0x0b],0xA4
  mov byte [gs:0x0c],'e'
  mov byte [gs:0x0d],0xA4
  mov byte [gs:0x0e],'l'
  mov byte [gs:0x0f],0xA4
  mov byte [gs:0x10],'l'
  mov byte [gs:0x11],0xA4
  mov byte [gs:0x12],'e'
  mov byte [gs:0x13],0xA4

  jmp $
