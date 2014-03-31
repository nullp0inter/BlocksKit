//
//  NSHashTable+BlocksKit.h
//  BlocksKit
//

#import <Foundation/Foundation.h>

/** Block extensions for NSHashTable.

 Both inspired by and resembling Smalltalk syntax, these utilities allows for
 iteration of a hashtable in a logical way that saves quite a bit of boilerplate code
 for filtering or finding objects or an object.

 @see NSSet(BlocksKit)
 @see NSMutableSet(BlocksKit)
 */
@interface NSHashTable (BlocksKit)

/** Loops through a hashtable and executes the given block with each object.

 @param block A single-argument, void-returning code block.
 */
- (void)bk_each:(void (^)(id obj))block;

/** Loops through a hashtable to find the object matching the block.

 bk_match: is functionally identical to bk_select:, but will stop and return
 on the first match.

 @param block A single-argument, BOOL-returning code block.
 @return Returns the object if found, `nil` otherwise.
 @see bk_select:
 */
- (id)bk_match:(BOOL (^)(id obj))block;

/** Loops through a hashtable to find the objects matching the block.

 @param block A single-argument, BOOL-returning code block.
 @return Returns a hashtable of the objects found. It uses the same NSPointerFunction, as the original hashtable.
 @see bk_match:
 */
- (NSHashTable *)bk_select:(BOOL (^)(id obj))block;

/** Loops through a hashtable to find the objects not matching the block.

 This selector performs *literally* the exact same function as select, but in reverse.

 @param block A single-argument, BOOL-returning code block.
 @return Returns a hashtable of all objects not found. It uses the same NSPointerFunction, as the original hashtable.
 */
- (NSHashTable *)bk_reject:(BOOL (^)(id obj))block;

/** Call the block once for each object and create a hashtable of the return values.

 @param block A single-argument, object-returning code block.
 @return Returns a hashtable of the objects returned by the block. It uses the same NSPointerFunction, as the original hashtable.
 */
- (NSHashTable *)bk_map:(id (^)(id obj))block;

/** Arbitrarily accumulate objects using a block.

 The concept of this selector is difficult to illustrate in words. The sum can
 be any NSObject, including (but not limited to) a string, number, or value.

 You can also do something like summing the count of an item:
 NSUInteger numberOfBodyParts = [[bodyList bk_reduce:nil withBlock:^id(id sum, id obj) {
 return @([sum integerValue] + obj.numberOfAppendages);
 }] unsignedIntegerValue];

 @param initial The value of the reduction at its start.
 @param block A block that takes the current sum and the next object to return the new sum.
 @return An accumulated value.
 */
- (id)bk_reduce:(id)initial withBlock:(id (^)(id sum, id obj))block;

/** Loops through a hashtable to find whether any object matches the block.

 This method is similar to the Scala list `exists`. It is functionally
 identical to bk_match: but returns a `BOOL` instead. It is not recommended
 to use bk_any: as a check condition before executing bk_match:, since it would
 require two loops through the hashtable.

 @param block A single-argument, BOOL-returning code block.
 @return YES for the first time the block returns YES for an object, NO otherwise.
 */
- (BOOL)bk_any:(BOOL (^)(id obj))block;

/** Loops through a hashtable to find whether no objects match the block.

 This selector performs *literally* the exact same function as bk_all: but in reverse.

 @param block A single-argument, BOOL-returning code block.
 @return YES if the block returns NO for all objects in the hashtable, NO otherwise.
 */
- (BOOL)bk_none:(BOOL (^)(id obj))block;

/** Loops through a hashtable to find whether all objects match the block.

 @param block A single-argument, BOOL-returning code block.
 @return YES if the block returns YES for all objects in the hashtable, NO otherwise.
 */
- (BOOL)bk_all:(BOOL (^)(id obj))block;

/** Filters a mutable hashtable to the objects matching the block.

 @param block A single-argument, BOOL-returning code block.
 @see bk_reject:
 */
- (void)bk_performSelect:(BOOL (^)(id obj))block;

/** Filters a mutable hashtable to all objects but the ones matching the block,
 the logical inverse to bk_select:.

 @param block A single-argument, BOOL-returning code block.
 @see bk_select:
 */
- (void)bk_performReject:(BOOL (^)(id obj))block;

/** Transforms the objects in the hashtable to the results of the block.

 @param block A single-argument, object-returning code block.
 @see bk_map:
 */
- (void)bk_performMap:(id (^)(id obj))block;

@end
