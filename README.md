# caeser_ecnrypt-
一个凯撒加密程序，可使用命令行参数，偏移量限制在int类型范围内

# 程序输出结果 存储在 result.txt 内

## 命令行参数
-b [偏移量]
-i [要加密/解密的文件名]

### 示例,e.g.
加密
```
caeser_encrypt -b 123 -i target.txt
```
解密
```
caeser_encrypt -b -123 -i target.txt
```

也可以不加命令行参数，直接进入程序按照提示输入偏移量 和 文件名
