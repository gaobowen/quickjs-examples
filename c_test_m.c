#include "quickjs.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define JS_INIT_MODULE js_init_module
#define countof(x) (sizeof(x) / sizeof((x)[0]))

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
