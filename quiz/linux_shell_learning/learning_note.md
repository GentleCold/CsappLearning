### 生成 50000 行随机字符串文件

使用 head 和 base64 命令

读取 50000 行 dev/urandom 文件，由于是二进制数据，通过管道输入给 base64 命令转码，再重定向到目标文件

输入命令 ```head -n 50000 /dev/urandom | base64 > random```

用 vim 查看 random 文件

![image-20220909211933123](C:/Users/15791/AppData/Roaming/Typora/typora-user-images/image-20220909211933123.png)

注意到实际行数达 22 万行

为达到要求，再使用 head 命令获取 50000 行数据，输入命令 ```head -n 50000 random > random2```

用 wc 命令查看 random2 文件

![image-20220909205002527](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909205002527.png)

替换 random 文件

![image-20220909205622666](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909205622666.png)

***

### 按字母排序文件，输出 unique 行

使用 sort 命令，覆盖写入

输入命令 ```sort --unique random > unique_sort_by_asc```

查看结果

![image-20220909212221043](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909212221043.png)

***

### 按数字排序文件，输出 unique 行

输入命令 ``` sort --unique --numeric-sort random > unique_sort_by_num``` 

查看结果

![image-20220909212307763](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909212307763.png)

***

### 追加写入文件 totall

使用 cat 命令

![image-20220909212434813](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909212434813.png)

通过行数判断追加成功

***

### 找内容确定的字符串

以 totall 文件为例，查找指定内容

使用 grep 命令

![image-20220909213439956](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909213439956.png)

***

### 统计匹配数目

将 grep 产生的数据通过管道输入给 wc 命令统计行数即匹配到的数目

![image-20220909213604144](C:\Users\15791\AppData\Roaming\Typora\typora-user-images\image-20220909213604144.png)

***

### Git 练习

1. 创建仓库并 push 到远程仓库

   git init

   git commit -m "first commit"

   git remote add shuishan git@gitea.shuishan.net.cn:10215501413/CsappLearning.git

   git push -u shuishan master

2. 创建分支并 push

   git checkout -b gitPractise

   git add ./quiz/linux_shell_learning/*

   git commit -m "feat: linux shell learning"

   git push shuishan gitPractise

3. 查看分支

   git branch -a

4. 合并分支

   git checkout master

   git merge gitPractise

5. push

   git push shuishan master

6. 删除新分支

   git push shuishan --delete gitPractise

***



### 使用命令小结

* tldr - 查看命令使用方法
* head - 获取文件从头开始的内容
* base64 - 将数据转为 base64 编码
* sort - 排序数据内容
* cat - 向文件追加数据
* grep - 匹配指定内容
* wc - 统计文件行数等信息
* mv - 重命名文件
* /dev/urandom - 包含随机数据的文件