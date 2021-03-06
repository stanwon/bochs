- 计算机启动流程
  - BIOS >> MBR >> loader >> kernel  
    计算机上电后，es:ip被强制初始化为0xF000:0xFFF0，这就是计算机上电后的入口地址，也就是计算机上电后执行的第一条指令。  
	  BIOS的代码存放在地址为0xF0000～0xFFFEF的ROM中，入口地址存放的指令是jmp 0xF000:0xe05b，上电后跳转到该地址执行BIOS代码。  
	  接下来BIOS会进行一些硬件自检行为，检查通过且初始化好硬件后，开始在0x000～0x3FF中建立数据结构，中断向量表IVT并填写中断例程。  
	  最后，BIOS会校验启动盘中0盘0道1扇区的内容，如果是是MBR（512字节，最后两字节为0x55、0xaa），将其加载进内存中，通过jmp 0:0x7c00跳转至MBR代码（这时会将cs的值修改成0）。  

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
      - 操作指令: pushf|popf
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
        - ds ss es fs gs不能用直接数赋值，需要用其他寄存器

- 实模式下cpu内存寻址方式
  - 寄存器寻址: 直接从寄存器中取数据
    mov ax,0x10
    mov dx,0x9
    mul dx  ;
  - 立即数寻址
    mov ax,0x10           ;立即数寻址 寄存器寻址
    mov ax,macro_selector ;立即数寻址 寄存器寻址
    mov ds,ax             ;寄存器寻址
  - 内存寻址: 
    - 内存访问方式 段基址:段内偏移地址
      - 代码段 cs:eip
      - 堆栈段 ss:sp
      - 数据段 ds:[直接数/符号地址/寄存器]
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

- call
  - 近调用: 同一段内，不用切段（64k）
    - 16位实模式相对近调用
      - 指令格式: call near 立即数地址，near可以省略，地址可以是函数名、标号、立即数
        call near 6
        call near program_name
      - 相对: 地址是相对地址，是增量，有正负。计算公式: 目标地址-当前地址-3
    - 16位实模式间接绝对近调用
      - 指令格式: call 寄存器寻址 | call 内存寻址，内存或寄存器里面的地址是绝对地址
      - 间接: 地址不是立即数
      - 绝对: 地址不是相对地址，而是绝对地址
        call ax
        call [0x1234]
  - 远调用: 不在同一段内，需要切段
    - 16位实模式直接绝对远调用: 对于这种调用，far可以省略
      - 指令格式: call far 段基址(立即数):段内偏移地址(立即数)
      - 直接: 地址是立即数
      - 绝对: 地址不是相对地址，而是绝对地址
    - 16位实模式间接绝对远调用
      - 指令格式: call far 内存寻址
      - 间接: 地址不是立即数，段基址和段内偏移地址在内存中，不储存在寄存器中是因为需要用到两个寄存器。低2存段内偏移地址，高2存段基址
      - 绝对: 地址不是相对地址，而是绝对地址

- 无条件转移: jmp
  - 短转移: 同一段内，不用切段（64k）
    - 16位实模式相对短转移
      - 指令格式: jmp short 立即数地址
      - 相对: 地址是相对地址，是增量，有正负。计算公式: 目标地址-当前地址-2
      - 范围: -128~127
  - 近转移
    - 16位实模式相对近转移
      - 指令格式: jmp near 立即数地址
      - 相对: 地址是相对地址，是增量，有正负。计算公式: 目标地址-当前地址-3
      - 范围: -32768~32767
    - 16位实模式间接绝对近转移
      - 指令格式: jmp near 寄存器寻址 | jmp near 内存寻址
      - 间接: 地址不是立即数
      - 绝对: 地址不是相对地址，而是绝对地址
  - 远转移: 不在同一段内，需要切段
    - 16位实模式直接绝对远转移
      - 指令格式: jmp 段基址(立即数):段内偏移地址(立即数)
      - 直接: 地址是立即数
      - 绝对: 地址不是相对地址，而是绝对地址
    - 16位实模式间接绝对远转移
      - 指令格式: jmp far 内存寻址
      - 间接: 地址不是立即数，段基址和段内偏移地址在内存中，不储存在寄存器中是因为需要用到两个寄存器。低2存段内偏移地址，高2存段基址
      - 绝对: 地址不是相对地址，而是绝对地址

- 有条件转移: jxx，目标地址只能是段内偏移地址，实模式转换成短转移或近转移，保护模式不做区分
  - eflags寄存器
    - 0 CF 进位、借位标志位
    - 2 PF 奇偶位标志位
    - 4 AF 辅助进位标志位
    - 6 ZF 零标志位
    - 7 SF 符号标志位
    - 8 TF 陷阱标志位
      单步调试
    - 9 IF 中断标志位
    - 10 DF 方向标志位
      用于字符串操作指令
    - 11 OF 溢出标志位
    - 12～13 IOPL 特权级标志位
      - 0
      - 1
      - 2
      - 3
    - 14 NT 任务嵌套标志位
    - 16 RF 恢复标志位
      程序调试
    - 17 VM 虚拟8086模式标志位
    - 18 AC 对齐检查标志位
    - 19 VIF 虚拟中断标志位
    - 20 VIP 虚拟中断挂起标志位
    - 21 ID 识别标志位
  - 见条件转移指令

- 硬盘操作
  - 基本步骤
    - 选通道，选择操作扇区数
    - 扇区操作起始地址
    - 选择LBA模式
    - 选择硬盘
    - 写commond
    - 读status
    - 取数据
  - 取数据方式
    - 无条件传送方式
    - 查询传送方式
    - 中断传送方式
    - 直接存储器存取方式
    - I/O处理机传送方式

- 保护模式与实模式区别
  - 寄存器: 实模式16位，保护模式除了段寄存器，通用寄存器、指令指针寄存器、标志寄存器都扩展到32位
  
- GDT 全局描述符表
  - 全局描述符，又成段描述符，存放着对内存段的描述信息，包括内存段的起始地址、大小、权限等
  - 全局描述符表存放在内存中，由GDTR指向它
  - 因此，段寄存器中存放的不是段基址，而是选择子，相当于数组下表，用来查找全局描述符表
  - 字段说明
    - G: 1b 23H
      - 段界限单位: (0)1字节 | (1)4kb
    - 段界限: 20b 0-15L 0-7H 16-19H
      - 段的边界，是一个单位量，单位是1字节或者4kb，所以最大最大边界值要么为1MB，要么为4GB
      - 实际段界限边界值=(描述符中段界限+1)*(段界限的粒度大小)-1
      - 段界限的粒度大小由G决定
    - 段基址: 32b 16-31L 0-7H 24-31H
    - type: 4b 8-11H
      - 指定本描述符的类型。
      - X 是否可执行
        - 0 不可执行，数据段
        - 1 可执行，代码段
      - R 是否可读
      - W 是否可写
        - 0 不可写，通常用于代码段
        - 1 可写，通常用于数据段
      - E 扩展方向
        - 0 向上扩展，代码段和数据段
        - 1 向下扩展，栈段
      - A 由cpu设置，访问过后置1
    - S: 1b 12H
      - 0表示系统段，1表示数据段
      - 系统段: 门结构，硬件系统需要的结构，如调用门、任务门等。门指向一段程序。
    - li: 2b 13-14H
      - 描述符特权级
    - P: 1b 15H
      - 段是否存在
        - 0 不存在
        - 1 存在
    - AVL 1b 20H
      - 给操作系统随意用，没有专门的用途
    - L 1b 21H
      - 用来设置是否是64为代码段
        - 0 32位
        - 1 64位
    - D/B 1b 22H
      - 用于指示有效地址(段内偏移地址)及操作数的大小
      - 代码段 D
        - 0 16 用ip寄存器
        - 1 32 用eip寄存器
      - 栈段 B
        - 0 16 用sp寄存器
        - 1 32 用esp寄存器
  - lgdt
    - 初始化GDTR，将全局描述符表初始地址赋给GDTR
    - 格式: lgdt 48位内存数据
      - 低16位: 以字节为单位的界限，GDT最大值: 65536/8=8192
      - 高32位: GDT的起始地址
  - 段的选择子
    - 相当于数组的索引，通过选择子可以选择对应的段
      - RPL 2b 0-1
        - 请求特权级
      - TI 1b 2
        - 表指示器，指示GDT或者LDT
        - 0 GDT
        - 1 LDT
      - 描述符索引值 13b 3-15
  - GDT第0个段描述符不可用，选择子初始值就是0，为了避免忘记初始化选择子而误选了第0个段描述符
  - 与实模式下寻址方式的对比
    - 实模式
      - 段寄存器(cs ds es fs gs ss)存放的是段基址
      - 段基址需要左移4位，(段基址 << 4)+段内偏移地址
    - 保护模式
      - 段寄存器(cs ds es fs gs ss)存放的是选择子，根据选择子查询GDT，找到对应的段描述符，段描述符里存放着段基址
      - 段基址不需要左移4位，段基址+段内偏移地址

- 获取内存容量
  - 通过BIOS中断0x15
    - EAX=0xE820
      - 遍历所有内存
    - AX=0xE801
      - 分别检测低 `15MB` 和 `16MB-4GB` 内存
    - AH=0x88
      - 最多检测64MB内存

- 分页机制
  - 不打开分页机制的情况下，`段部件` 输出的是物理地址，可以直接用来寻址，打开分页机制的情况下，段部件输出的不在是物理地址，而是虚拟地址，需要通过`页部件`在页表中查找到物理地址后再送上地址总线来访问内存
  - 分页机制的作用
    - 将线性地址转换成物理地址
    - 用大小相等的页代替大小不等的段
  - 分页机制下加载进程的过程
    - `线性空间` >> `虚拟空间`  `物理空间`
    - 线性地址 >> 虚拟地址(虚拟地址内存放的是物理地址) >> 物理地址
    - 线性地址如何转换成虚拟地址
      - 打开分页机制前需要将页表地址存放在cr3中（物理地址）
      - 页表一共有1M个页表项，因此线性地址的高20位可以作为页表项的索引来查询页表项。而页表本身是线性表,存放在内存中，每个表项大小是4字节，因此索引要乘4后加上cr3的页表物理地址，得到的就是页表项的物理地址，从该物理地址中可以得到实际的物理页的地址，在加上线性地址的低12位就可以得到实际的物理地址，这样就实现了从线性地址到物理地址的映射。
      - 页部件: 将虚拟地址转换成实际的物理地址
    - 加载进程时，现在虚拟空间中寻找可用的空间分配内存段，进程在逻辑上会被分成很多段，但在虚拟空间中还是连续的。接着由操作系统寻找可用的物理页分配给这些虚拟页，在物理空间中不一定是连续的。
  - 一级页表: 内存块个数 `1M` 内存块大小 `4KB` 

- 二级页表
  - 为什么要二级页表
    - 一级页表要提前建好，因为要给操作系统和用户进程分配虚拟空间
    - 每个程序都要有自己的页表，占空间
  - 页目录项 `PDE`
  - 页表项 `PTE` 
  - 虚拟地址转换物理地址  
    虚拟地址的前10位是页目录项的索引，可以相应查到页表项，中间10位作为页表项的索引，查到对应的物理页，最后12位作为物理页的索引，查到最终的物理地址，这些操作还是由页部件完成

- 启动分页步骤
  - 准备好页目录表和页表
  - 将页表地址写入控制寄存器cr3(又称页目录基址寄存器)
  - 寄存器cr0的PG位置1

- 其他
  - `$` 本行代码地址
  - `$$` 本section的起始地址
	- nasm基本使用
		- `nasm -f bin -o mbr.bin mbr.S`		纯二进制可执行文件
		- `nasm -f elf -o print.o print.S`	elf格式二进制可执行文件
	- bios中断手册
	- vstart
		vstart并不是告诉编译器将程序加载到指定地址的内存上，编译器并不具备加载程序到内存的能力。vstart的作用是告诉编译器，后面的以指定的地址作为起始地址来变址。使用vstart的前提是知道程序将会被加载到哪里。
  - ret: 取回ip，sp+2
  - retf: 取回ip+cs，sp+4
  - IO接口包含多个端口，即寄存器
  - 中断向量表只在实模式下有用
  - nm 列出可执行文件的符号表及其地址，只支持elf文件，不支持纯二进制文件
  - bochs调试命令
    - CPU and memory contents 查看cpu和内存内容
      - x 查看地址内容，接线性地址
      - xp 查看地址内容，接物理地址
        - b 1字节
        - h 2字节
        - w 4字节
        - g 8字节
    - Debugger control 调试控制
      - u 反汇编
      - set 
        - 设置寄存器内容
          set reg=val
        - 设置停止执行指令时，是否反汇编指令
          set u on|off
      - show
        - show mode 变换模式时提醒
        - show int 中断时提醒
        - show call 函数调用时提醒
      - trace on|off
  - loader加载到内存后不能被覆盖掉
  - 打开A20地址线，为了访问到1M以外的内存
  - 进入保护模式
    - 加载GDT
    - 打开A20
      in al,0x92
      or al,0x02
      out 0x92,al
    - 打开CR0寄存器PE位
      mov eax,cr0
      or eax,0x00000001
      mov cr0,eax
  - bochs查看保护模式下的信息
  - 选择子
  	|描述符索引值（3-15）|表指示器（2）|RPL-请求特权级（0-1）|
  - 
	
  




      
      
      
      
      
      
      
      
      
