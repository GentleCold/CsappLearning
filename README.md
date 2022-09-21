# CsappLearning

这是一个本人学习 csapp 的 learning 库

其中包括：
* labs - csapp 的各项来自官网的 lab 
* quiz - csapp 的部分练习与课堂作业
* csapp.txt - 由本人 E-note 项目生成的笔记

各 lab 得分
* Data Lab - 62 / 62
  * 项目目录下运行 ```./labs/data_lab/driver.pl -u GentleCold```
* Bomb Lab - 答案见 bomb_lab/answer

注：
* 本人运行环境: WINDOWS10 - WSL2
* 各 lab 由其自带的 Makefile 编译
  * ~~但为了保证 linux 环境，使用 cmake 调用 make 编译~~
  * cmake 调用失败，使用 wsl shell 手动 make
* quiz 由本人编写的 CMakeLists 编译
* 所有 lab 尽可能的不参考他人代码，参考处会有 //TODO 标记