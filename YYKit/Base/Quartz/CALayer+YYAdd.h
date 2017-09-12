//
//  CALayer+YYAdd.h
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 14/5/10.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
/* lzy注170605：
 一直说category的局限是无法给一个类动态地添加属性。
 在YY的这些分类中，有很多属性的shortcut。category虽然没有办法添加完整的属性，但是可以生成全局变量，只要我们在.m中完善setter和getter就好了。
 */

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Provides extensions for `CALayer`.
 */
@interface CALayer (YYAdd)
/* lzy注170605：
 没有变换的截屏成image，图片尺寸等于它的bounds
 */

/**
 Take snapshot without transform, image's size equals to bounds.
 */
- (nullable UIImage *)snapshotImage;

/* lzy注170605：
 没有变换地截屏为PDF的data，页面的大小等于它的bounds
 */

/**
 Take snapshot without transform, PDF's page size equals to bounds.
 */
- (nullable NSData *)snapshotPDF;

/**
 Shortcut to set the layer's shadow
 设置layer的阴影的封装方法
 @param color  Shadow Color
 @param offset Shadow offset
 @param radius Shadow radius
 */
- (void)setLayerShadow:(UIColor *)color offset:(CGSize)offset radius:(CGFloat)radius;

/**移除所有的子layer
 Remove all sublayers.
 */
- (void)removeAllSublayers;

@property (nonatomic) CGFloat left;        ///< Shortcut for frame.origin.x.
@property (nonatomic) CGFloat top;         ///< Shortcut for frame.origin.y
@property (nonatomic) CGFloat right;       ///< Shortcut for frame.origin.x + frame.size.width
@property (nonatomic) CGFloat bottom;      ///< Shortcut for frame.origin.y + frame.size.height
@property (nonatomic) CGFloat width;       ///< Shortcut for frame.size.width.
@property (nonatomic) CGFloat height;      ///< Shortcut for frame.size.height.
@property (nonatomic) CGPoint center;      ///< Shortcut for center.
@property (nonatomic) CGFloat centerX;     ///< Shortcut for center.x
@property (nonatomic) CGFloat centerY;     ///< Shortcut for center.y
@property (nonatomic) CGPoint origin;      ///< Shortcut for frame.origin.
@property (nonatomic, getter=frameSize, setter=setFrameSize:) CGSize  size; ///< Shortcut for frame.size.


@property (nonatomic) CGFloat transformRotation;     ///< key path "tranform.rotation"
@property (nonatomic) CGFloat transformRotationX;    ///< key path "tranform.rotation.x"
@property (nonatomic) CGFloat transformRotationY;    ///< key path "tranform.rotation.y"
@property (nonatomic) CGFloat transformRotationZ;    ///< key path "tranform.rotation.z"
@property (nonatomic) CGFloat transformScale;        ///< key path "tranform.scale"
@property (nonatomic) CGFloat transformScaleX;       ///< key path "tranform.scale.x"
@property (nonatomic) CGFloat transformScaleY;       ///< key path "tranform.scale.y"
@property (nonatomic) CGFloat transformScaleZ;       ///< key path "tranform.scale.z"
@property (nonatomic) CGFloat transformTranslationX; ///< key path "tranform.translation.x"
@property (nonatomic) CGFloat transformTranslationY; ///< key path "tranform.translation.y"
@property (nonatomic) CGFloat transformTranslationZ; ///< key path "tranform.translation.z"

/**
 Shortcut for transform.m34, -1/1000 is a good value.
 It should be set before other transform shortcut.
 */
@property (nonatomic) CGFloat transformDepth;

/**layer的contentsGravity属性被包装为contentMode
 Wrapper for `contentsGravity` property.
 */
@property (nonatomic) UIViewContentMode contentMode;

/**
 Add a fade animation to layer's contents when the contents is changed.
 当layer的内容发生变化时，给内容添加一个淡入淡出过渡效果
 @param duration Animation duration
 @param curve    Animation curve.
 */
- (void)addFadeAnimationWithDuration:(NSTimeInterval)duration curve:(UIViewAnimationCurve)curve;

/**
 Cancel fade animation which is added with "-addFadeAnimationWithDuration:curve:".
 */
- (void)removePreviousFadeAnimation;

@end

NS_ASSUME_NONNULL_END
