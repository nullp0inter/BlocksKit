//
//  NSHashTable+BlocksKit.m
//  BlocksKit
//


#import "NSHashTable+BlocksKit.h"

@implementation NSHashTable (BlocksKit)

- (void)bk_each:(void (^)(id obj))block
{
	NSParameterAssert(block != nil);
    for (id obj in self) {
        block(obj);
    }
}

- (id)bk_match:(BOOL (^)(id obj))block
{
    NSParameterAssert(block != nil);

    for (id obj in self) {
        if (block(obj)) {
            return obj;
        }
    }
    return nil;
}

- (NSHashTable *)bk_select:(BOOL (^)(id obj))block
{
	NSParameterAssert(block != nil);

	return [self bk_objectsPassingTest:^BOOL(id obj, BOOL *stop) {
		return block(obj);
	}];
}

- (NSHashTable *)bk_reject:(BOOL (^)(id obj))block
{
	NSParameterAssert(block != nil);

	return [self bk_objectsPassingTest:^BOOL(id obj, BOOL *stop) {
		return !block(obj);
	}];
}

- (NSHashTable *)bk_map:(id (^)(id obj))block
{
	NSParameterAssert(block != nil);

	NSHashTable *result = [[NSHashTable alloc] initWithPointerFunctions:self.pointerFunctions capacity:self.count];

	for (id obj in self) {
		[result addObject:block(obj)];
	};

	return result;
}

- (id)bk_reduce:(id)initial withBlock:(id (^)(id sum, id obj))block
{
	NSParameterAssert(block != nil);

	id result = initial;

	for (id obj in self) {
		result = block(result, obj);
	};
    
	return result;
}

- (BOOL)bk_any:(BOOL (^)(id obj))block
{
	return [self bk_match:block] != nil;
}

- (BOOL)bk_none:(BOOL (^)(id obj))block
{
	return [self bk_match:block] == nil;
}

- (BOOL)bk_all:(BOOL (^)(id obj))block
{
	NSParameterAssert(block != nil);

	for (id obj in self) {
		if (!block(obj)) {
			return NO;
		}
	};

    return YES;
}

- (void)bk_performSelect:(BOOL (^)(id obj))block {
	NSParameterAssert(block != nil);

	NSHashTable *objectsToReject = [self bk_objectsPassingTest:^BOOL(id obj, BOOL *stop) {
		return !block(obj);
	}];

	[self minusHashTable:objectsToReject];
}

- (void)bk_performReject:(BOOL (^)(id obj))block {
	NSParameterAssert(block != nil);
	[self bk_performSelect:^BOOL(id obj) {
		return !block(obj);
	}];
}

- (void)bk_performMap:(id (^)(id obj))block {
	NSParameterAssert(block != nil);

    NSHashTable *result = [self bk_map:block];
	[self removeAllObjects];
    [self unionHashTable:result];
}

#pragma mark - Private

- (NSHashTable*)bk_objectsPassingTest:(BOOL (^)(id obj, BOOL *stop))predicate
{
    NSHashTable *result = [[NSHashTable alloc] initWithPointerFunctions:self.pointerFunctions capacity:self.count];
    BOOL stop = NO;
    id obj = nil;
    NSEnumerator* valueEnumerator = [self objectEnumerator];
    while ((obj = [valueEnumerator nextObject]) != nil && !stop) {
        if (predicate(obj, &stop)) {
            [result addObject:obj];
        }
    }
    return result;
}

@end
