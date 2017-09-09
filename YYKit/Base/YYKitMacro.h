//
//  YYKitMacro.h
//  YYKit <https://github.com/ibireme/YYKit>
//
//  Created by ibireme on 13/3/29.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>
#import <sys/time.h>
#import <pthread.h>

#ifndef YYKitMacro_h
#define YYKitMacro_h
/* lzy注170609：
 c++中定义了cplusplus，C语言中没有该定义。即这个是用来：识别是c代码还是c++代码
 在看EXTERN_C ，调用extern "C"会让c++编译器按照c的编译格式来编译。多用于c++库的头文件。 也即是说这个在c++的环境下，会强制编译器用 C 语言的规则去编译，而不是用 C++ 语言的规则。。因为 C++ 为了实现函数重载会把函数名和参数等联合起来合成一个中介的函数名，如果 C 函数也被这样编译会出问题，所以在我看来这个是为了兼容c++环境不会编译报错。
 */
#ifdef __cplusplus
#define YY_EXTERN_C_BEGIN  extern "C" {
#define YY_EXTERN_C_END  }
#else
#define YY_EXTERN_C_BEGIN
#define YY_EXTERN_C_END
#endif


YY_EXTERN_C_BEGIN


/* lzy注170609：
 if(x>high)
 return high;
 else
 {
 if(x<low)
 return  low;
 else
 return  x;
 }
 
 涉及到的参数有x high low
 x落在[low, high]闭区间之间，取x返回
 x超出某一个区间边界则取该边界。返回中间值。
 */
#ifndef YY_CLAMP // return the clamped（夹紧的） value
#define YY_CLAMP(_x_, _low_, _high_)  (((_x_) > (_high_)) ? (_high_) : (((_x_) < (_low_)) ? (_low_) : (_x_)))
#endif

/* lzy注170609：
 typeof 还有为什么要用do while 直接用if else 不是就行了吗
 typeof(var) 是gcc对C语言的一个扩展保留字，用于声明变量类型,var可以是数据类型（int， char*..),也可以是变量表达式。也即是说 变量a 还可以是个变量表达式。
 为什么使用do-while：
 设计一个宏，如果调用者在if或者else后面直接写宏而不把代码放到大括号中，会出问题：不论是不是一条语句，也不论是if后还是else后，都加上大括号；
 那么你会想，宏定义自己使用大括号括起来就好，这样一来的话我们的宏在不论if后面有没有大括号的情况下都能工作了！
 
 调用者会自然的把宏当做一个语句，然后在其调用的后面加上分号，
 即：
 if(errorHappen) macro;
 
 展开即是：
 if(errorHappen) { content of macro };
 
 然而在这个情况下：
 
 if(errorHappen)
     macro;
 else
   //Yep, no error, I am happy~ :)
 
 展开即是：
 
 if (errorHappen){
    cotent of macro
 }; else {
    //Yep, no error, I am happy~ :)

 }
 
 使用do-while
 if (errorHappend)
        do{
        content of macro
    } while (0);
 
 else {
 
 }
 
 
 
 
 把宏的代码块添加到do中，然后之后while(0)，在行为上没有任何改变，但是可以巧妙地吃掉那个悲剧的分号
 https://onevcat.com/2014/01/black-magic-in-macro/
 直接在文章中搜『do while语句』
 
 */
#ifndef YY_SWAP // swap two value
#define YY_SWAP(_a_, _b_)  do { __typeof__(_a_) _tmp_ = (_a_); (_a_) = (_b_); (_b_) = _tmp_; } while (0)
#endif

/* lzy注170612：
 主要是NSAssert 和 NSCAssert的区别。基本没有用过，不清楚，查到的资料放到这里备查找。
 在印象笔记这搜索『YYKit学习笔记 - Swift迷』
 
 YYAssertNil(condition, description, …)//当条件condition不满足时，抛出异常，并打印(description, …)。这个宏被展开之后持有了self，那么有可能就会出现引用不释放的问题。
 YYCAssertNil(condition, description, ...)//同上，但YYCAssertNil展开之后并没有持有self，不会出现引用不释放的问题。
 
 YYAssertNotNil(condition, description, ...)//当条件condition满足时，抛出异常，并打印(description, …)。这个宏被展开之后持有了self，那么有可能就会出现引用不释放的问题。
 
 YYCAssertNotNil(condition, description, …)//同上，但YYCAssertNotNil展开之后并没有持有self，不会出现引用不释放的问题。
 
 */

#define YYAssertNil(condition, description, ...) NSAssert(!(condition), (description), ##__VA_ARGS__)
#define YYCAssertNil(condition, description, ...) NSCAssert(!(condition), (description), ##__VA_ARGS__)

#define YYAssertNotNil(condition, description, ...) NSAssert((condition), (description), ##__VA_ARGS__)
#define YYCAssertNotNil(condition, description, ...) NSCAssert((condition), (description), ##__VA_ARGS__)

#define YYAssertMainThread() NSAssert([NSThread isMainThread], @"This method must be called on the main thread")
#define YYCAssertMainThread() NSCAssert([NSThread isMainThread], @"This method must be called on the main thread")


/**
 Add this macro before each category implementation, so we don't have to use
 -all_load or -force_load to load object files from static libraries that only
 contain categories and no classes.
 More info: http://developer.apple.com/library/mac/#qa/qa2006/qa1490.html .
 *******************************************************************************
 Example:
     YYSYNTH_DUMMY_CLASS(NSString_YYAdd)
 
 oc 编译静态库 要在build setting other linker flag设置 -all_load 和-Objc才可以将category 编进去
 使用这段宏定义他可以虚拟新建一个与名字category 相同.h.m 让编译器 编译通过
 但是每次要对所有category 都要申明这个的话可能会比较麻烦，然而直接加-all_load 据官方文档说，它可以使生成的可执行文件较大，并且产生一些不需要的文件。
 */
#ifndef YYSYNTH_DUMMY_CLASS
#define YYSYNTH_DUMMY_CLASS(_name_) \
@interface YYSYNTH_DUMMY_CLASS_ ## _name_ : NSObject @end \
@implementation YYSYNTH_DUMMY_CLASS_ ## _name_ @end
#endif


/**
 Synthsize a dynamic object property in @implementation scope.
 It allows us to add custom properties to existing classes in categories.
 
 @param association  ASSIGN / RETAIN / COPY / RETAIN_NONATOMIC / COPY_NONATOMIC
 @warning #import <objc/runtime.h>
 *******************************************************************************
 Example:
     @interface NSObject (MyAdd)
     @property (nonatomic, retain) UIColor *myColor;
     @end
     
     #import <objc/runtime.h>
     @implementation NSObject (MyAdd)
     YYSYNTH_DYNAMIC_PROPERTY_OBJECT(myColor, setMyColor, RETAIN, UIColor *)
     @end
 */
#ifndef YYSYNTH_DYNAMIC_PROPERTY_OBJECT
#define YYSYNTH_DYNAMIC_PROPERTY_OBJECT(_getter_, _setter_, _association_, _type_) \
- (void)_setter_ : (_type_)object { \
    [self willChangeValueForKey:@#_getter_]; \
    objc_setAssociatedObject(self, _cmd, object, OBJC_ASSOCIATION_ ## _association_); \
    [self didChangeValueForKey:@#_getter_]; \
} \
- (_type_)_getter_ { \
    return objc_getAssociatedObject(self, @selector(_setter_:)); \
}
#endif


/**
 Synthsize a dynamic c type property in @implementation scope.
 It allows us to add custom properties to existing classes in categories.
 
 @warning #import <objc/runtime.h>
 *******************************************************************************
 Example:
     @interface NSObject (MyAdd)
     @property (nonatomic, retain) CGPoint myPoint;
     @end
     
     #import <objc/runtime.h>
     @implementation NSObject (MyAdd)
     YYSYNTH_DYNAMIC_PROPERTY_CTYPE(myPoint, setMyPoint, CGPoint)
     @end
 */
#ifndef YYSYNTH_DYNAMIC_PROPERTY_CTYPE
#define YYSYNTH_DYNAMIC_PROPERTY_CTYPE(_getter_, _setter_, _type_) \
- (void)_setter_ : (_type_)object { \
    [self willChangeValueForKey:@#_getter_]; \
    NSValue *value = [NSValue value:&object withObjCType:@encode(_type_)]; \
    objc_setAssociatedObject(self, _cmd, value, OBJC_ASSOCIATION_RETAIN); \
    [self didChangeValueForKey:@#_getter_]; \
} \
- (_type_)_getter_ { \
    _type_ cValue = { 0 }; \
    NSValue *value = objc_getAssociatedObject(self, @selector(_setter_:)); \
    [value getValue:&cValue]; \
    return cValue; \
}
#endif

/**
 Synthsize a weak or strong reference.
 
 Example:
    @weakify(self)
    [self doSomething^{
        @strongify(self)
        if (!self) return;
        ...
    }];
 避免Block拥有self而造成循环引用
 给Block里的self加上强引用，避免Block里的self提前释放掉了。
 */
#ifndef weakify
    #if DEBUG
        #if __has_feature(objc_arc)
        #define weakify(object) autoreleasepool{} __weak __typeof__(object) weak##_##object = object;
        #else
        #define weakify(object) autoreleasepool{} __block __typeof__(object) block##_##object = object;
        #endif
    #else
        #if __has_feature(objc_arc)
        #define weakify(object) try{} @finally{} {} __weak __typeof__(object) weak##_##object = object;
        #else
        #define weakify(object) try{} @finally{} {} __block __typeof__(object) block##_##object = object;
        #endif
    #endif
#endif

#ifndef strongify
    #if DEBUG
        #if __has_feature(objc_arc)
        #define strongify(object) autoreleasepool{} __typeof__(object) object = weak##_##object;
        #else
        #define strongify(object) autoreleasepool{} __typeof__(object) object = block##_##object;
        #endif
    #else
        #if __has_feature(objc_arc)
        #define strongify(object) try{} @finally{} __typeof__(object) object = weak##_##object;
        #else
        #define strongify(object) try{} @finally{} __typeof__(object) object = block##_##object;
        #endif
    #endif
#endif


/**
 Convert CFRange to NSRange
 @param range CFRange @return NSRange
 */
static inline NSRange YYNSRangeFromCFRange(CFRange range) {
    return NSMakeRange(range.location, range.length);
}

/**
 Convert NSRange to CFRange
 @param range NSRange @return CFRange
 */
static inline CFRange YYCFRangeFromNSRange(NSRange range) {
    return CFRangeMake(range.location, range.length);
}

/**
 Same as CFAutorelease(), compatible for iOS6
 @param arg CFObject @return same as input
 */
static inline CFTypeRef YYCFAutorelease(CFTypeRef CF_RELEASES_ARGUMENT arg) {
    if (((long)CFAutorelease + 1) != 1) {
        return CFAutorelease(arg);
    } else {
        id __autoreleasing obj = CFBridgingRelease(arg);
        return (__bridge CFTypeRef)obj;
    }
}

/**基准测试，是测试性能的方式。//执行block所消耗的时间，单位毫秒
 Profile time cost.
 @param block    code to benchmark
 @param complete code time cost (millisecond)
 
 Usage:
    YYBenchmark(^{
        // code
    }, ^(double ms) {
        NSLog("time cost: %.2f ms",ms);
    });
 
 */
static inline void YYBenchmark(void (^block)(void), void (^complete)(double ms)) {
    // <QuartzCore/QuartzCore.h> version
    /*
    extern double CACurrentMediaTime (void);
    double begin, end, ms;
    begin = CACurrentMediaTime();
    block();
    end = CACurrentMediaTime();
    ms = (end - begin) * 1000.0;
    complete(ms);
    */
    
    // <sys/time.h> version
    struct timeval t0, t1;
    gettimeofday(&t0, NULL);
    block();
    gettimeofday(&t1, NULL);
    double ms = (double)(t1.tv_sec - t0.tv_sec) * 1e3 + (double)(t1.tv_usec - t0.tv_usec) * 1e-3;
    complete(ms);
}

static inline NSDate *_YYCompileTime(const char *data, const char *time) {
    NSString *timeStr = [NSString stringWithFormat:@"%s %s",data,time];
    NSLocale *locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US"];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"MMM dd yyyy HH:mm:ss"];
    [formatter setLocale:locale];
    return [formatter dateFromString:timeStr];
}
/* lzy注170612：
 获取这行代码的编译时间（非执行时间）。
 */
/**
 Get compile timestamp.
 @return A new date object set to the compile date and time.
 */
#ifndef YYCompileTime
// use macro to avoid compile warning when use pch file
#define YYCompileTime() _YYCompileTime(__DATE__, __TIME__)
#endif

/**返回距离现在延迟x秒的时间
 Returns a dispatch_time delay from now.
 */
static inline dispatch_time_t dispatch_time_delay(NSTimeInterval second) {
    return dispatch_time(DISPATCH_TIME_NOW, (int64_t)(second * NSEC_PER_SEC));
}

/**同上，但当你的设备进入睡眠状态dispatch_time停止运行。dispatch_walltime继续运行
 Returns a dispatch_wall_time delay from now.
 */
static inline dispatch_time_t dispatch_walltime_delay(NSTimeInterval second) {
    return dispatch_walltime(DISPATCH_TIME_NOW, (int64_t)(second * NSEC_PER_SEC));
}

/**
 Returns a dispatch_wall_time from NSDate.
 */
static inline dispatch_time_t dispatch_walltime_date(NSDate *date) {
    NSTimeInterval interval;
    double second, subsecond;
    struct timespec time;
    dispatch_time_t milestone;
    
    interval = [date timeIntervalSince1970];
    subsecond = modf(interval, &second);
    time.tv_sec = second;
    time.tv_nsec = subsecond * NSEC_PER_SEC;
    milestone = dispatch_walltime(&time, 0);
    return milestone;
}

/**
 Whether in main queue/thread.
 */
static inline bool dispatch_is_main_queue() {
    return pthread_main_np() != 0;
}

/**调用主线程执行这个block且不等待，立即return
 Submits a block for asynchronous execution on a main queue and returns immediately.
 */
static inline void dispatch_async_on_main_queue(void (^block)()) {
    if (pthread_main_np()) {
        block();
    } else {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

/**在主线程调用一个block，且一直等待block执行完毕
 Submits a block for execution on a main queue and waits until the block completes.
 */
static inline void dispatch_sync_on_main_queue(void (^block)()) {
    if (pthread_main_np()) {
        block();
    } else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

/**初始化一个pthread mutex

 Initialize a pthread mutex.
 */
static inline void pthread_mutex_init_recursive(pthread_mutex_t *mutex, bool recursive) {
#define YYMUTEX_ASSERT_ON_ERROR(x_) do { \
__unused volatile int res = (x_); \
assert(res == 0); \
} while (0)
    assert(mutex != NULL);
    if (!recursive) {
        YYMUTEX_ASSERT_ON_ERROR(pthread_mutex_init(mutex, NULL));
    } else {
        pthread_mutexattr_t attr;
        YYMUTEX_ASSERT_ON_ERROR(pthread_mutexattr_init (&attr));
        YYMUTEX_ASSERT_ON_ERROR(pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE));
        YYMUTEX_ASSERT_ON_ERROR(pthread_mutex_init (mutex, &attr));
        YYMUTEX_ASSERT_ON_ERROR(pthread_mutexattr_destroy (&attr));
    }
#undef YYMUTEX_ASSERT_ON_ERROR
}


YY_EXTERN_C_END
#endif
