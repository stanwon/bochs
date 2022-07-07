- cpu组成部分
  - 控制单元
    - 指令寄存器IR(Instruction Register)
    - 指令译码器ID(Instruction Decoder)
    - 操作控制器OC(Operation Controller)
  - 存储单元
    - 寄存器
    - L1、L2、L3缓存
  - 运算单元:负责算术运算和逻辑运算

- cpu寄存器
  - 不可见寄存器
    - GDTR  全局描述符表寄存器
        通过lgdt指令指定全局描述符表的地址及偏移量
    - LDTR  本地描述符表寄存器
    - IDTR  中断描述符表寄存器
        通过lidt指令指定中断描述符表的地址及偏移量
    - TR    任务寄存器
        通过ltr指令指定制定一个任务状态段tss
    - CR0-3 控制寄存器
    - IP    指令指针寄存器
    - flags 标志寄存器
        pushf popf
    - DR0-7 调试寄存器
  - 可见寄存器
    - 通用寄存器
      - eax: 通常用来执行加法，函数调用的返回值一般也放在这里面
      - ebx: 机制寄存器，存储起始地址
      - ecx: 通常用来作为计数器，比如for循环
      - edx: 读写I/O端口时，edx用来存放端口号
      - esp: 栈顶指针，指向栈的顶部
      - ebp: 栈底指针，指向栈的底部，通常用ss:bp的形式来定位函数存放在栈中的局部变量
      - esi: 字符串操作时，用于存放数据源的地址
      - edi: 字符串操作时，用于存放目的地址的，和esi两个经常搭配一起使用，执行字符串的复制等操作
    - 段寄存器
      - cs: 代码段
      - ds: 数据段
      - ss: 栈段
      - es: 扩展段
      - fs: 数据段
      - gs: 数据段

- 实模式下cpu内存寻址方式
  - 寄存器寻址: 直接从寄存器中取数据
    mov ax,0x10
    mov dx,0x9
    mul dx  ;
  - 立即数寻址
    mov ax,0x10           ;立即数寻址 寄存器寻址
    mov ax,macro_selector ;立即数寻址 寄存器寻址
    mov ds,ax             ;寄存器寻址
  - 内存寻址: 内存访问方式 段基址:段内偏移地址
    - 直接寻址
      mov ax, [Ox1234]
      mov ax, [fs:Ox5678]
    - 基址寻址
      - bx的默认段寄存器是ds，bp的默认段寄存器是ss，bp和sp都是栈的有效地址
      add word[bx],0x1234
    - 变址寻址: si di
      mov [di],ax
      mov [si+0x1234],ax
      movsb
      movsw
      movsd
    - 基址变址寻址
      mov [bx+di],ax
      mov [bx+si],ax
