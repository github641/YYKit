//
//  UIView+YYAdd.m
//  YYCategories <https://github.com/ibireme/YYCategories>
//
//  Created by ibireme on 13/4/3.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import "UIView+YYAdd.h"
#import <QuartzCore/QuartzCore.h>
#import "YYCategoriesMacro.h"

YYSYNTH_DUMMY_CLASS(UIView_YYAdd)


@implementation UIView (YYAdd)

- (UIImage *)snapshotImage {
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, self.opaque, 0);
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *snap = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return snap;
}

- (UIImage *)snapshotImageAfterScreenUpdates:(BOOL)afterUpdates {
    if (![self respondsToSelector:@selector(drawViewHierarchyInRect:afterScreenUpdates:)]) {
        return [self snapshotImage];
    }
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, self.opaque, 0);
    /* lzy注170609：
     这是UIView在iOS7+提供的API：
     使用这个方法，在当前的上下文中渲染一个视图层级的截图，当截图缺少图片data返回No，如果截图完成返回YES。
     当在layoutSubviews方法内部，调用这个方法
     Use this method to render a snapshot of the view hierarchy into the current context. 方法返回BOOL值。Returns NO if the snapshot is missing image data, YES if the snapshot is complete.
     
     Calling this method from layoutSubviews while the current transaction is committing
     will capture what is currently displayed
     regardless if afterUpdates is YES.
     
     当afterUpdates参数为NO，
     当前的事务将要提交，此时，在layoutSubviews方法中调用drawViewHierarchyInRect：afterScreenUpdates：方法，
     将会捕捉到的是，当前的显示内容。
     
     而当afterUpdates参数为YES的时候，
     将等待更新完毕，再捕捉。
     
     所以YY说，It's faster than "snapshotImage", but may cause screen updates.
     这个方法比上一个方法snapshotImage更加快速。
     但是可能引起屏幕更新。
     
     */
    [self drawViewHierarchyInRect:self.bounds afterScreenUpdates:afterUpdates];
    UIImage *snap = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return snap;
}

- (NSData *)snapshotPDF {
    CGRect bounds = self.bounds;
    NSMutableData* data = [NSMutableData data];
    CGDataConsumerRef consumer = CGDataConsumerCreateWithCFData((__bridge CFMutableDataRef)data);
    CGContextRef context = CGPDFContextCreate(consumer, &bounds, NULL);
    CGDataConsumerRelease(consumer);
    if (!context) return nil;
    CGPDFContextBeginPage(context, NULL);
    CGContextTranslateCTM(context, 0, bounds.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    [self.layer renderInContext:context];
    CGPDFContextEndPage(context);
    CGPDFContextClose(context);
    CGContextRelease(context);
    return data;
}

- (void)setLayerShadow:(UIColor*)color offset:(CGSize)offset radius:(CGFloat)radius {
    self.layer.shadowColor = color.CGColor;
    self.layer.shadowOffset = offset;
    self.layer.shadowRadius = radius;
    self.layer.shadowOpacity = 1;
    self.layer.shouldRasterize = YES;
    self.layer.rasterizationScale = [UIScreen mainScreen].scale;
}

- (void)removeAllSubviews {
    //[self.subviews makeObjectsPerformSelector:@selector(removeFromSuperview)];
    while (self.subviews.count) {
        [self.subviews.lastObject removeFromSuperview];
    }
}


- (UIViewController *)viewController {
    for (UIView *view = self; view; view = view.superview) {
        UIResponder *nextResponder = [view nextResponder];
        if ([nextResponder isKindOfClass:[UIViewController class]]) {
            return (UIViewController *)nextResponder;
        }
    }
    return nil;
}

- (CGFloat)visibleAlpha {
    if ([self isKindOfClass:[UIWindow class]]) {
        if (self.hidden) return 0;
        return self.alpha;
    }
    if (!self.window) return 0;
    CGFloat alpha = 1;
    UIView *v = self;
    while (v) {
        if (v.hidden) {
            alpha = 0;
            break;
        }
        /* lzy注170609：
         这个语句看出了这个属性的具体做用，
         是算self的可视透明度，self的透明不仅和自身的透明度有关系，和其所有的父视图都有关系。且子视图的透明度是在父视图的透明度而基础上的。
         */
        alpha *= v.alpha;
        v = v.superview;
    }
    return alpha;
}

/* lzy注170609：
 这个方法看起来很混乱，调用很乱。坐标也乱。
 各种一通转换。主要还是不熟悉机制。
 
 */

- (CGPoint)convertPoint:(CGPoint)point toViewOrWindow:(UIView *)view {
    
    if (!view) {
        /* lzy注170609：
         目标view不存在
         */
        
        if ([self isKindOfClass:[UIWindow class]]) {
//            view不存在且self是窗口。是窗口就调用窗口类的转换方法。
            return [((UIWindow *)self) convertPoint:point toWindow:nil];
        } else {
//            view不存在，self是普通的view。调用view的转换方法。
            return [self convertPoint:point toView:nil];
        }
    }
    
    /* lzy注170609：
     目标view或者目标窗口存在，会执行到这里。
     */
//    确定起始窗口和目标窗口
    UIWindow *from = [self isKindOfClass:[UIWindow class]] ? (id)self : self.window;// self是窗口取self，self不是窗口取self所在的窗口
    UIWindow *to = [view isKindOfClass:[UIWindow class]] ? (id)view : view.window;// 目标view或者目标window是窗口，就取它，不是也取它所在的窗口。
    
    /* lzy注170609：
     from to 都存在，且不是同一个视图时，才能执行到return下面的语句，不然就直接执行return语句了。
     */
    if ((!from || !to) || (from == to)) return [self convertPoint:point toView:view];
    /* lzy注170609：
     point在之前的坐标系的坐标确认一下
     */
    point = [self convertPoint:point toView:from];
    /* lzy注170609：
     将point从当前窗口坐标系，转到目标窗口坐标系。
     */
    point = [to convertPoint:point fromWindow:from];
    
    point = [view convertPoint:point fromView:to];
    return point;
    
    /* lzy注170609：
     后面几个方法的转换机制都是一样的规范，这个规范目前还是没有理解。
     - (CGPoint)convertPoint:(CGPoint)point fromViewOrWindow:(nullable UIView *)view;
     - (CGRect)convertRect:(CGRect)rect toViewOrWindow:(nullable UIView *)view;
     - (CGRect)convertRect:(CGRect)rect fromViewOrWindow:(nullable UIView *)view;
     */
}

- (CGPoint)convertPoint:(CGPoint)point fromViewOrWindow:(UIView *)view {
    if (!view) {
        if ([self isKindOfClass:[UIWindow class]]) {
            return [((UIWindow *)self) convertPoint:point fromWindow:nil];
        } else {
            return [self convertPoint:point fromView:nil];
        }
    }
    
    UIWindow *from = [view isKindOfClass:[UIWindow class]] ? (id)view : view.window;
    UIWindow *to = [self isKindOfClass:[UIWindow class]] ? (id)self : self.window;
    if ((!from || !to) || (from == to)) return [self convertPoint:point fromView:view];
    point = [from convertPoint:point fromView:view];
    point = [to convertPoint:point fromWindow:from];
    point = [self convertPoint:point fromView:to];
    return point;
}

- (CGRect)convertRect:(CGRect)rect toViewOrWindow:(UIView *)view {
    if (!view) {
        if ([self isKindOfClass:[UIWindow class]]) {
            return [((UIWindow *)self) convertRect:rect toWindow:nil];
        } else {
            return [self convertRect:rect toView:nil];
        }
    }
    
    UIWindow *from = [self isKindOfClass:[UIWindow class]] ? (id)self : self.window;
    UIWindow *to = [view isKindOfClass:[UIWindow class]] ? (id)view : view.window;
    if (!from || !to) return [self convertRect:rect toView:view];
    if (from == to) return [self convertRect:rect toView:view];
    rect = [self convertRect:rect toView:from];
    rect = [to convertRect:rect fromWindow:from];
    rect = [view convertRect:rect fromView:to];
    return rect;
}

- (CGRect)convertRect:(CGRect)rect fromViewOrWindow:(UIView *)view {
    if (!view) {
        if ([self isKindOfClass:[UIWindow class]]) {
            return [((UIWindow *)self) convertRect:rect fromWindow:nil];
        } else {
            return [self convertRect:rect fromView:nil];
        }
    }
    
    UIWindow *from = [view isKindOfClass:[UIWindow class]] ? (id)view : view.window;
    UIWindow *to = [self isKindOfClass:[UIWindow class]] ? (id)self : self.window;
    if ((!from || !to) || (from == to)) return [self convertRect:rect fromView:view];
    rect = [from convertRect:rect fromView:view];
    rect = [to convertRect:rect fromWindow:from];
    rect = [self convertRect:rect fromView:to];
    return rect;
}

- (CGFloat)left {
    return self.frame.origin.x;
}

- (void)setLeft:(CGFloat)x {
    CGRect frame = self.frame;
    frame.origin.x = x;
    self.frame = frame;
}

- (CGFloat)top {
    return self.frame.origin.y;
}

- (void)setTop:(CGFloat)y {
    CGRect frame = self.frame;
    frame.origin.y = y;
    self.frame = frame;
}

- (CGFloat)right {
    return self.frame.origin.x + self.frame.size.width;
}

- (void)setRight:(CGFloat)right {
    CGRect frame = self.frame;
    frame.origin.x = right - frame.size.width;
    self.frame = frame;
}

- (CGFloat)bottom {
    return self.frame.origin.y + self.frame.size.height;
}

- (void)setBottom:(CGFloat)bottom {
    CGRect frame = self.frame;
    frame.origin.y = bottom - frame.size.height;
    self.frame = frame;
}

- (CGFloat)width {
    return self.frame.size.width;
}

- (void)setWidth:(CGFloat)width {
    CGRect frame = self.frame;
    frame.size.width = width;
    self.frame = frame;
}

- (CGFloat)height {
    return self.frame.size.height;
}

- (void)setHeight:(CGFloat)height {
    CGRect frame = self.frame;
    frame.size.height = height;
    self.frame = frame;
}

- (CGFloat)centerX {
    return self.center.x;
}

- (void)setCenterX:(CGFloat)centerX {
    self.center = CGPointMake(centerX, self.center.y);
}

- (CGFloat)centerY {
    return self.center.y;
}

- (void)setCenterY:(CGFloat)centerY {
    self.center = CGPointMake(self.center.x, centerY);
}

- (CGPoint)origin {
    return self.frame.origin;
}

- (void)setOrigin:(CGPoint)origin {
    CGRect frame = self.frame;
    frame.origin = origin;
    self.frame = frame;
}

- (CGSize)size {
    return self.frame.size;
}

- (void)setSize:(CGSize)size {
    CGRect frame = self.frame;
    frame.size = size;
    self.frame = frame;
}

@end
