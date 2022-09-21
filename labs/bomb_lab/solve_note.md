### 1. 使用 objdump 反编译 bomb

```objdump -d bomb > bomb.s```

### 2. 查找 main 函数

发现调用了函数 phase_1, phase_2 等等函数

猜测这些函数即用来验证输入字符串的正确性

### 3. 查看函数 phase_1

发现调用了函数 strings_not_equal

并且在调用之前为 %esi 赋值 0x402400

说明将其作为参数传进然后作为验证

猜测其为所需字符串的地址

### 4. 使用 gdb 查看字符串

为函数 phase_1 设置断点 ```break phase_1```

打印地址处字符串 ```print (char *) 0x402400```

得到 phase1

### 5. 查看函数 phase_2

发现调用函数 read_six_numbers

说明输入需要 6 个数字

查看后续汇编代码发现进行了循环控制

根据汇编代码得到 6 个数字

由于输入函数用的是 scanf，故不用考虑转为字符，直接输入 6 个数字

得到 phase2

### 6. 查看函数 phase_3

发现调用了 scanf 函数

观察参数寄存器 %rcx, %rdx, %rsi

使用 gdb 知需要输入两个数字

cmpl 得知第一个数字需要小于 0x7

使用 gdb 观察间接跳转指令 jmpq

指向后面的 switch 控制流

从而得知输入的两个数字相关联

即本题多解

得到其中一个 phase3

### 7. 查看函数 phase_4

与 phase_3 同样的输入

第一个数字小于等于 0xe

发现调用 func4 且返回值需为 0

观察调用后代码发现第二个数为 0

观察 func4 发现第一个数需为 7

得到 phase4

### 8. 查看函数 phase_5

