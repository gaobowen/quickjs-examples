

# 1. QuickJS 快速入门 (QuickJS QuickStart)
<!-- TOC -->
- [1. QuickJS 快速入门 (QuickJS QuickStart)](#1-quickjs-%e5%bf%ab%e9%80%9f%e5%85%a5%e9%97%a8-quickjs-quickstart)
  - [1.1. 简介](#11-%e7%ae%80%e4%bb%8b)
  - [1.2. 安装](#12-%e5%ae%89%e8%a3%85)
  - [1.3. 简单使用](#13-%e7%ae%80%e5%8d%95%e4%bd%bf%e7%94%a8)
    - [1.3.1. 控制台执行](#131-%e6%8e%a7%e5%88%b6%e5%8f%b0%e6%89%a7%e8%a1%8c)
    - [1.3.2. js脚本执行](#132-js%e8%84%9a%e6%9c%ac%e6%89%a7%e8%a1%8c)
    - [1.3.3. 直接编译成二进制文件执行](#133-%e7%9b%b4%e6%8e%a5%e7%bc%96%e8%af%91%e6%88%90%e4%ba%8c%e8%bf%9b%e5%88%b6%e6%96%87%e4%bb%b6%e6%89%a7%e8%a1%8c)
  - [1.4. 全局对象](#14-%e5%85%a8%e5%b1%80%e5%af%b9%e8%b1%a1)
  - [1.5. `std` 模块](#15-std-%e6%a8%a1%e5%9d%97)
    - [1.5.1. 常见的导出函数](#151-%e5%b8%b8%e8%a7%81%e7%9a%84%e5%af%bc%e5%87%ba%e5%87%bd%e6%95%b0)
    - [1.5.2. FILE 原型](#152-file-%e5%8e%9f%e5%9e%8b)
    - [1.5.3. std代码示例](#153-std%e4%bb%a3%e7%a0%81%e7%a4%ba%e4%be%8b)
  - [1.6. `os` 模块](#16-os-%e6%a8%a1%e5%9d%97)
  - [1.7. 模块调用](#17-%e6%a8%a1%e5%9d%97%e8%b0%83%e7%94%a8)
    - [1.7.1. js数据类型在C中的定义](#171-js%e6%95%b0%e6%8d%ae%e7%b1%bb%e5%9e%8b%e5%9c%a8c%e4%b8%ad%e7%9a%84%e5%ae%9a%e4%b9%89)
    - [1.7.2. c模块编写](#172-c%e6%a8%a1%e5%9d%97%e7%bc%96%e5%86%99)
    - [1.7.3. 使用.so模块](#173-%e4%bd%bf%e7%94%a8so%e6%a8%a1%e5%9d%97)
 <!-- /TOC -->

## 1.1. 简介
    QuickJS是一个小型的可嵌入Javascript引擎。它支持ES2020规范，包括模块、异步生成器和代理。它还支持数学扩展，比如大整数(BigInt)、大浮点数(BigFloat)和操作符重载。

## 1.2. 安装
- Linux 直接下载 [源码](https://github.com/quickjs-zh/QuickJS) 
```bash 
make && make install
```

- MacOS X 下 makefile 有 [Bug](https://github.com/quickjs-zh/QuickJS/issues/7) ，可以直接使用 homebrew 安装
```bash
brew install quickjs 
```
- 执行 `qjs` 验证安装成功

## 1.3. 简单使用

### 1.3.1. 控制台执行  
`qjs` 进入quickjs环境，`-h` 获取帮助，`-q` 退出环境。  
直接执行js：  
```js
console.log(new Date())
```
输出：Wed Aug 14 2019 23:51:43 GMT+0800  
 &emsp;&emsp;&emsp;undefined
</br>
```js
(function(){ return 1+1;})()
```
输出：2
</br>

### 1.3.2. js脚本执行
新建一个js脚本，名为`hello.js`,内容为`console.log('hello world !')`, 在js目录下执行 
```bash
qjs hello.js
```
输出：hello world !
</br>

### 1.3.3. 直接编译成二进制文件执行
将 ```quickjs.h```、```quickjs-libc.h```、```libquickjs.a``` 拷贝到js文件同目录下。
```bash
qjsc -o hello hello.js
ls
./hello
```
输出：hello world !  
编译出来的可执行文件的大小只有569K(2019-9-18版本为900K)，没有任何外部依赖，非常适合嵌入式设备使用。


## 1.4. 全局对象
- `scriptArgs` 输入的命令行参数，第一个参数为脚本的名称。
- `print(...args)`、`console.log(...args)`打印由空格和尾随换行符分隔的参数。  

新建js脚本`globle_obj.js`
```js
(function(){
    if(typeof scriptArgs != 'undefined'){
        print(scriptArgs);
        console.log(scriptArgs[1]);
    }
})()
```
```bash
qjs globle_obj.js -a 123 1234
```
输出：  
globle_obj.js,-a,123,1234  
-a   

## 1.5. `std` 模块
`std`模块为`quickjs-libc`提供包装器`stdlib.h`和`stdio.h`和其他一些实用程序。  
### 1.5.1. 常见的导出函数  
- `exit(n)` 退出进程。  
- `evalScript(str)` 将字符串`str`以脚本方式运行（全局eval）。  
- `loadScript(filename)` 将文件`filename`以脚本方式运行（全局eval）。  
- `Error(errno)` 是 **std.Error**构造函数。错误实例包含字段`errno`（错误代码）和`message`（`std.Error.strerror(errno)`的结果）。  
构造函数包含以下字段：`EINVAL`、`EIO`、`EACCES`、`EEXIST`、`ENOSPC`、`ENOSYS`、`EBUSY`、`ENOENT`、`EPERM`、`EPIPE` 常见错误的整数值 （可以定义附加错误代码）。
- `strerror(errno)` 返回描述错误的字符串`errno`。
- `open(filename, flags)` 打开一个文件（libc的包装器`fopen()`）。在I/O错误时抛出 `std.Error`。
- `tmpfile()` 打开一个临时文件。在I/O错误时抛出`std.Error`。
- `puts(str)` 相当于`std.out.puts(str)`。
- `printf(fmt, ...args)` 相当于`std.out.printf(fmt, ...args)`、`sprintf(fmt, ...args)` 相当于libc的sprintf()。
- `in`、`out`、`err` 包装libc文件的`stdin`、`stdout`、`stderr`。
- `SEEK_SET`、`SEEK_CUR`、`SEEK_END` 是 **seek()** 的常量。
- `global` 引用全局对象。
- `gc()` 手动调用循环删除算法。循环移除算法在需要时自动启动，因此该功能在特定内存限制或测试时非常有用。
- `getenv(name)` 返回环境变量的值 `name` ，或未定义时返回 `undefined` .

### 1.5.2. FILE 原型
- `close()` 关闭文件。
- `puts(str)` 使用UTF-8编码输出字符串。
- `printf(fmt, ...args)` 格式化printf，与libc printf格式相同。
- `flush()` 刷新缓冲的文件。
- `seek(offset, whence)` 寻找特定文件位置 (从哪里`std.SEEK_*`)。在I/O错误时抛出 `std.Error`。
- `tell()` 返回当前文件位置。
- `eof()` 如果文件结束，则返回true。
- `fileno()` 返回关联的OS句柄。
- `read(buffer, position, length)`（封装 libc `fread`）。
- `write(buffer, position, length)` (封装 libc `fread`)。
- `getline()` 返回文件中的下一行，假设为UTF-8编码，不包括尾随换行符。
- `getByte()` 返回文件中的下一个字节。
- `putByte(c)` 将一个字节写入文件。  
### 1.5.3. std代码示例
创建文件`std_m.js`
```js
import * as std from 'std';
var file = std.open('std_open_file.js','w');
file.puts('var file = std.open(\"std_open_file.txt\",\"w\");\n');
file.puts('file.puts(\'std_open_file line1\\n\');\n');
file.puts('file.puts(\'std_open_file line2\\n\');\n');
file.puts('file.close();\n');
file.close();
std.loadScript('std_open_file.js');
var rdfile = std.open("std_open_file.txt","r");
do{
    console.log(rdfile.getline());
}while(!rdfile.eof());
rdfile.close();
```
执行`qjs std_m.js` ，目录下会生成2个新文件`std_open_file.js` `std_open_file.txt`。  
控制台输出：  
`std_open_file line1`  
`std_open_file line2`  
`null`

## 1.6. `os` 模块
 `os` 模块提供操作系统特定功能：底层文件访问、信号、计时器、异步 I/O。  
 如果是OK，OS函数通常返回0，或者OS返回特定的错误代码。

可用导出函数：
- `open(filename, flags, mode = 0o666)` 打开一个文件。如果错误，返回句柄或<0。 `O_RDONLY`、`O_WRONLY`、`O_RDWR`、`O_APPEND`、`O_CREAT`、`O_EXCL`、`O_TRUNC` POSIX打开标志。`O_TEXT`
 (Windows特定)。以文本模式打开文件。默认为二进制模式。
- `close(fd)` 关闭文件句柄`fd`。
- `seek(fd, offset, whence)` 寻找文件。使用 `std.SEEK_*` 或 `whence`。
- `read(fd, buffer, offset, length)` 读取指定长度的字节码。返回读取成功的字节码长度，失败返回 <0;
- `write(fd, buffer, offset, length)` 写入字节码，失败返回<0。
- `isatty(fd)` `fd` 是一个TTY (终端)句柄返`true` 。
- `ttyGetWinSize(fd)` 返回TTY大小 `[width, height]` 或者如果不可用返回 `null` 。
- `ttySetRaw(fd)` 在原始模式下设置TTY。
- `remove(filename)` 删除文件。如果正常则返回0，如果错误则返回<0。
- `rename(oldname, newname)` 重命名文件。如果正常则返回0，如果错误则返回<0。
- `setReadHandler(fd, func)` 将读处理程序添加到文件句柄`fd`。 `fd`每次有数据待增加处理时调用`func` 。支持每个文件句柄的单个读处理程序。使用 `func = null` 来删除句柄。
- `setWriteHandler(fd, func)` 将写处理程序添加到文件句柄`fd`。  `fd`每次有数据待写入处理时调用`func` . 支持每个文件句柄一个写处理程序。使用 `func = null来删除句柄。
- `signal(signal, func)` 当信号 `signal` 发生时调用 `func` 。 每个信号编号只支持一个处理程序。使用 `null` 设定的默认处理或 `undefined` 忽略的信号。
`SIGINT`
`SIGABRT`
`SIGFPE`
`SIGILL`
`SIGSEGV`
`SIGTERM`
POSIX 信号编号。
- `setTimeout(func, delay)` 在 `delay` 毫秒之后调用函数 `func` 。返回计时器的句柄。
- `clearTimer(handle)`取消计时器。
- `platform` 返回表示该平台的字符串： `"linux"`, `"darwin"`, `"win32"` or `"js"`。
  
```js
import * as os from 'os';
os.remove('hello');
os.remove('std_open_file.js');
os.remove('std_open_file.txt');
```
删除生成的测试文件

## 1.7. 模块调用
ES6模块完全支持。默认名称解析规则如下：
- 模块名称带有前导.或..是相对于当前模块的路径
- 模块名称没有前导.或..是系统模块，例如std或os
- 模块名称以.so结尾，是使用QuickJS C API的原生模块  

使用js文件模块和系统模块，参照引用原生js模块和上面的例子即可，这里就不多赘述。
这里着重讲解如何编写自己的原生C模块，并且以导入so文件的方式在js代码中使用。  
### 1.7.1. js数据类型在C中的定义
```c
typedef union JSValueUnion {
    int32_t int32;          //整数值
    double float64;         //double值
    void *ptr;              //QuickJS引用类型的指针
} JSValueUnion;             //存放于同一地址，且互斥

typedef struct JSValue {
    JSValueUnion u;         //存放真实数值或着其指针
    int64_t tag;            //JSValue类型的标示符（如 undefined 其 tag == JS_TAG_UNDEFINED）
} JSValue;
```
此结构定义在 **quickjs.h** 中。
### 1.7.2. c模块编写
流程如下：  
1. 自定义原生C函数
2. 定义 QuickJS C 函数 
3. 定义API的函数入口名称及列表
4. 定义初始化回调方法，将函数入口列表在模块中暴露
5. 定义初始化模块方法，由系统自动调用，且函数名称不可更改

创建编写c_test_m.c文件：
```c
// #include "quickjs.h"
// #include "stdio.h"
// #include "stdlib.h"
// #include "string.h"

// #define JS_INIT_MODULE js_init_module
//#define countof(x) (sizeof(x) / sizeof((x)[0]))

/* 自定义原生C函数 */
static double test_add(int a, double b)
{
    return a + b;
}

static char *test_add_str(const char *a, double b)
{
    /* 要有足够的空间来容纳要拼接的字符串，否则可能会造成缓冲溢出的错误情况 */
    char instr[64];
    sprintf(instr, "%.2f", b);
    char *dest = malloc(128);
    memset(dest, 0, 128);
    strcpy(dest, a);
    char *retdest = strcat(dest, instr);
    return dest;
}

/* 
    定义 QuickJS C 函数 
    *ctx     : 运行时上下文
    this_val : this对象
    argc     : 入参个数
    *argv    : 入参列表
*/
static JSValue js_test_add(JSContext *ctx, JSValueConst this_val,
                           int argc, JSValueConst *argv)
{
    int a;
    double b;
    if (JS_ToInt32(ctx, &a, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &b, argv[1]))
        return JS_EXCEPTION;
    printf("argc = %d \n", argc);
    printf("a = %d \n", a);
    printf("b = %lf \n", b);
    printf("argv[1].u.float64 = %lf \n", argv[1].u.float64);
    return JS_NewFloat64(ctx, test_add(a, b));
}

static JSValue js_test_add_str(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    if (!JS_IsString(argv[0]))
    {
        return JS_EXCEPTION;
    }
    double d;
    if (JS_ToFloat64(ctx, &d, argv[1]))
        return JS_EXCEPTION;
    const char *jscstr = JS_ToCString(ctx, argv[0]);
    printf("JS_ToCString(ctx, argv[0]) = %s \n", jscstr);
    printf("argv[1].u.float64 = %lf \n", argv[1].u.float64);
    char *jsret = test_add_str(jscstr, d);
    return JS_NewString(ctx, jsret);
}

/* 定义API的函数入口名称及列表 */
static const JSCFunctionListEntry js_test_funcs[] = {
    /* JS_CFUNC_DEF(函数入口名称，入参个数，QuickJS C 函数) */
    JS_CFUNC_DEF("testAdd", 2, js_test_add),
    JS_CFUNC_DEF("testAddStr", 2, js_test_add_str),
};

/* 定义初始化回调方法（由系统调用，入参格式固定），将函数入口列表 在模块中暴露 */
static int js_test_init(JSContext *ctx, JSModuleDef *m)
{
    return JS_SetModuleExportList(ctx, m, js_test_funcs,
                                  countof(js_test_funcs));
}

/* 定义初始化模块方法，由系统自动调用，且函数名称不可更改 */
JSModuleDef *JS_INIT_MODULE(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_test_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_test_funcs, countof(js_test_funcs));
    return m;
}
```
将 ```quickjs.h```、```quickjs-libc.h```、```libquickjs.a``` 拷贝到当前工程目录下。  
执行命令
```bash
gcc c_test_m.c libquickjs.a  -fPIC -shared -o libtest.so
```
生成```libtest.so```文件。  
### 1.7.3. 使用.so模块
创建js文件 c_test_m.js
```js
import { testAdd , testAddStr} from 'libtest.so'
console.log('\n')
console.log(`testAdd: ${testAdd(1, 0.5)}`)
console.log('\n')
console.log(`testAddStr: ${testAddStr('Pi equal to about ', 3.14159)}`)
console.log('\n')
```
```bash
qjs c_test_m.js
```
输出：  
argc = 2   
a = 1   
b = 0.500000   
argv[1].u.float64 = 0.500000   
testAdd: 1.5  
  
  
JS_ToCString(ctx, argv[0]) = Pi equal to about    
argv[1].u.float64 = 3.141590   
testAddStr: Pi equal to about 3.14  
<br>
<br>
[项目地址](https://github.com/gaobowen/quickjs-examples)
















