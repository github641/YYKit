//
//  NSArray+YYAdd.h
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 13/4/4.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Provide some some common method for `NSArray`.
 */
@interface NSArray (YYAdd)

/**传入data，把plist文件的data，转换为数组，如果无法转成的格式不是数组，那么返回nil
 Creates and returns an array from a specified property list data.
 
 @param plist   A property list data whose root object is an array.
 @return A new array created from the plist data, or nil if an error occurs.
 */
+ (nullable NSArray *)arrayWithPlistData:(NSData *)plist;

/**传入字符串，把plist文件字符串，转为data，然后调用arrayWithPlistData方法尝试转为数组
 Creates and returns an array from a specified property list xml string.
 
 @param plist   A property list xml string whose root object is an array.
 @return A new array created from the plist string, or nil if an error occurs.
 */
+ (nullable NSArray *)arrayWithPlistString:(NSString *)plist;

/**把数组转为二进制格式的plist的data
 Serialize the array to a binary property list data.
 
 @return A bplist data, or nil if an error occurs.
 */
- (nullable NSData *)plistData;

/**把数组转为二进制格式的plist的data，若data存在，那么调用data.utf8String方法，转为plist字符串
 Serialize the array to a xml property list string.
 
 @return A plist xml string, or nil if an error occurs.
 */
- (nullable NSString *)plistString;

/**返回数组中随机下标的一个元素，如果数组为空，返回nil
 Returns the object located at a random index.
 
 @return The object in the array with a random index value.
 If the array is empty, returns nil.
 */
- (nullable id)randomObject;

/**返回数组中指定index的元素，如果index越界了，返回nil；而不是像默认的抛出异常
 Returns the object located at index, or return nil when out of bounds.
 It's similar to `objectAtIndex:`, but it never throw exception.
 
 @param index The object located at index.
 */
- (nullable id)objectOrNilAtIndex:(NSUInteger)index;

/**把对象转换为json字符串，代码逻辑中是，如果对象不是有效的json对象，那么返回nil
 Convert object to json string. return nil if an error occurs.
 NSString/NSNumber/NSDictionary/NSArray
 */
- (nullable NSString *)jsonStringEncoded;

/**把字符串转换为NSJSONWritingPrettyPrinted样式的json字符串，如果对象不是有效的json对象，那么返回nil
 Convert object to json string formatted. return nil if an error occurs.
 */
- (nullable NSString *)jsonPrettyStringEncoded;

@end


/**
 Provide some some common method for `NSMutableArray`.
 */
@interface NSMutableArray (YYAdd)

/** 与NSArray中的arrayWithPlistData中方法的区别是“NSPropertyListSerialization propertyListWithData”方法调用中的枚举参数不同，一个是可变的，一个是不可变的。
 Creates and returns an array from a specified property list data.
 
 @param plist   A property list data whose root object is an array.
 @return A new array created from the plist data, or nil if an error occurs.
 */
+ (nullable NSMutableArray *)arrayWithPlistData:(NSData *)plist;

/**把plist字符串转为data，在使用前一个方法转为 可变数组
 Creates and returns an array from a specified property list xml string.
 
 @param plist   A property list xml string whose root object is an array.
 @return A new array created from the plist string, or nil if an error occurs.
 */
+ (nullable NSMutableArray *)arrayWithPlistString:(NSString *)plist;

/**lzy注170601：这里第一次看到YY对于Foundation中原生类的分析。若系统提供的类中有该方法，但是没有公开，为了方法调用安全性考虑，在分类中重写一个同名方法，以下几个“Override for safe”方法同理。因为“如果分类和原来类出现同名的方法, 优先调用分类中的方法, 原来类中的方法会被忽略 ”。
 
 移除可变数组中索引最小的那个元素。如果可变数组为空，则此方法没有任何影响。
 Removes the object with the lowest-valued index in the array.
 If the array is empty, this method has no effect.
 
 @discussion Apple has implemented this method, but did not make it public.
 Override for safe.
 */
- (void)removeFirstObject;

/**移除可变数组中索引最大的那个元素。如果可变数组是空的，那么此方法将没有任何影响。
 Removes the object with the highest-valued index in the array.
 If the array is empty, this method has no effect.
 
 @discussion Apple's implementation said it raises an NSRangeException if the
 array is empty, but in fact nothing will happen. Override for safe.
 */
- (void)removeLastObject;

/**返回可变数组中，索引最小的那个元素，并把这个元素从可变数组中移除。如果可变数组为空，则返回nil。
 Removes and returns the object with the lowest-valued index in the array.
 If the array is empty, it just returns nil.
 
 @return The first object, or nil.
 */
- (nullable id)popFirstObject;

/**同上，元素为最后一个元素
 Removes and returns the object with the highest-valued index in the array.
 If the array is empty, it just returns nil.
 
 @return The first object, or nil.
 */
- (nullable id)popLastObject;

/**把给定的元素放到可变数组的最后面，这个元素为nil将抛出异常
 Inserts a given object at the end of the array.
 
 @param anObject The object to add to the end of the array's content.
 This value must not be nil. Raises an NSInvalidArgumentException if anObject is nil.
 */
- (void)appendObject:(id)anObject;

/**同上，位置在最前面
 Inserts a given object at the beginning of the array.
 
 @param anObject The object to add to the end of the array's content.
 This value must not be nil. Raises an NSInvalidArgumentException if anObject is nil.
 */
- (void)prependObject:(id)anObject;

/**把 给定数组中的元素，一一放到接收数组的最后面。若给定数组为空或者nil，方法调用没有任何结果。
 Adds the objects contained in another given array to the end of the receiving
 array's content.
 
 @param objects An array of objects to add to the end of the receiving array's
 content. If the objects is empty or nil, this method has no effect.
 */
- (void)appendObjects:(NSArray *)objects;

/**同上， 元素放置的位置在最前面。
 Adds the objects contained in another given array to the beginnin of the receiving
 array's content.
 
 @param objects An array of objects to add to the beginning of the receiving array's
 content. If the objects is empty or nil, this method has no effect.
 */
- (void)prependObjects:(NSArray *)objects;

/**同上，元素放置的位置在开发者指定的位置
 Adds the objects contained in another given array at the index of the receiving
 array's content.
 
 @param objects An array of objects to add to the receiving array's
 content. If the objects is empty or nil, this method has no effect.
 
 @param index The index in the array at which to insert objects. This value must
 not be greater than the count of elements in the array. Raises an
 NSRangeException if index is greater than the number of elements in the array.
 */
- (void)insertObjects:(NSArray *)objects atIndex:(NSUInteger)index;

/**反转可变数组中 元素的顺序。代码中是中间切一刀，二分后调换收尾，趋于中间的做法。
 Reverse the index of object in this array.
 Example: Before @[ @1, @2, @3 ], After @[ @3, @2, @1 ].
 */
- (void)reverse;

/** 打乱数组中元素的顺序，洗牌
 Sort the object in this array randomly.
 */
- (void)shuffle;

@end

NS_ASSUME_NONNULL_END
