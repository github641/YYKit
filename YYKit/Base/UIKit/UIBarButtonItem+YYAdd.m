//
//  UIBarButtonItem+YYAdd.m
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 13/10/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import "UIBarButtonItem+YYAdd.h"
#import "YYCategoriesMacro.h"
#import <objc/runtime.h>

YYSYNTH_DUMMY_CLASS(UIBarButtonItem_YYAdd)

/* lzy注170605：
 
 
 @interface UIBarButtonItem_YYAdd : NSObject @end
 
 @implementation UIBarButtonItem_YYAdd @end
 
 YYSYNTH_DUMMY_CLASS，这个类作用没有看懂，搜索一番，引用如下：
 
 这一段还是挺特别的， 但oc 编译静态库 要在build setting other linker flag设置 -all_load 和-Objc才可以将category 编进去
 使用这段宏定义他可以虚拟新建一个与名字category 相同.h.m 让编译器 编译通过
 但是每次要对所有category 都要申明这个的话可能会比较麻烦，直接加-all_load 据官方文档说，它可以使生成的可执行文件较大，并且产生一些不需要的文件
 
 http://www.jianshu.com/p/a5acbed59ef3
 
 */


static const int block_key;

@interface _YYUIBarButtonItemBlockTarget : NSObject

@property (nonatomic, copy) void (^block)(id sender);

- (id)initWithBlock:(void (^)(id sender))block;
- (void)invoke:(id)sender;

@end

@implementation _YYUIBarButtonItemBlockTarget

- (id)initWithBlock:(void (^)(id sender))block{
    self = [super init];
    if (self) {
        _block = [block copy];
    }
    return self;
}

- (void)invoke:(id)sender {
    if (self.block) self.block(sender);
}

@end


@implementation UIBarButtonItem (YYAdd)

- (void)setActionBlock:(void (^)(id sender))block {
    _YYUIBarButtonItemBlockTarget *target = [[_YYUIBarButtonItemBlockTarget alloc] initWithBlock:block];
    objc_setAssociatedObject(self, &block_key, target, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    
    /* lzy注170605：
     The object that receives an action when the item is selected.
     If nil, the action message is passed up the responder chain where it may be handled by any object implementing a method corresponding to the selector held by the action property. The default value is nil.
     */
    [self setTarget:target];

    /* lzy注170605：
     The selector defining the action message to send to the target object when the user taps this bar button item.
     当tap触发的时候，invoke:方法是要去target里面找的，target是_YYUIBarButtonItemBlockTarget对象
     */
    [self setAction:@selector(invoke:)];
    
}

- (void (^)(id)) actionBlock {
    _YYUIBarButtonItemBlockTarget *target = objc_getAssociatedObject(self, &block_key);
    return target.block;
}

@end
