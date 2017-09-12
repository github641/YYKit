//
//  NSObject+YYAddForKVO.h
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 14/10/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
/* lzy注170602：
 在疑惑不知道本类方法做了什么的时候，搜索了方法名，找到一个类似的三方类库，http://www.jianshu.com/p/70b2503d5fd1
 他的实现因为hook了 dealloc方法，弊端很大，之前也记得有个Facebook开源的KVOController，评论也提起，要看下。
 不过通过他的博客，大概知道这个类做了什么。
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Observer with block (KVO).
 */
@interface NSObject (YYAddForKVO)

/**过Block方式注册一个KVO，监听路径， KVO回调block，obj为监听对象，oldVal为旧值，newVal为新值
 Registers a block to receive KVO notifications for the specified key-path 
 relative to the receiver.
 
 @discussion The block and block captured objects are retained. Call
 `removeObserverBlocksForKeyPath:` or `removeObserverBlocks` to release.
 
 @param keyPath The key path, relative to the receiver, of the property to 
 observe. This value must not be nil.
 
 @param block   The block to register for KVO notifications.
 */
- (void)addObserverBlockForKeyPath:(NSString*)keyPath
                             block:(void (^)(id _Nonnull obj, id _Nonnull oldVal, id _Nonnull newVal))block;

/**移除监听指定keyPath的对象
 Stops all blocks (associated by `addObserverBlockForKeyPath:block:`) from
 receiving change notifications for the property specified by a given key-path 
 relative to the receiver, and release these blocks.
 
 @param keyPath A key-path, relative to the receiver, for which blocks is 
 registered to receive KVO change notifications.
 */
- (void)removeObserverBlocksForKeyPath:(NSString*)keyPath;

/**移除所有监听
 Stops all blocks (associated by `addObserverBlockForKeyPath:block:`) from
 receiving change notifications, and release these blocks.
 */
- (void)removeObserverBlocks;

@end

NS_ASSUME_NONNULL_END
