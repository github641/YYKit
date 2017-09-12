//
//  NSThread+YYAdd.h
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 15/7/3.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
/* lzy注170605：
 这个类看不太懂，方法的整体流程过了一下，大体知道每一步的意思，但是对其运行机制不了解。
 */

#import <Foundation/Foundation.h>

@interface NSThread (YYAdd)

/**给当前线程所在的runloop中，添加一个自动释放池
 Add an autorelease pool to current runloop for current thread.
 
 @discussion If you create your own thread (NSThread/pthread), and you use 
 runloop to manage your task, you may use this method to add an autorelease pool
 to the runloop. Its behavior is the same as the main thread's autorelease pool.
 */
+ (void)addAutoreleasePoolToCurrentRunloop;

@end
