// AFRESTClient.h
//
// Copyright (c) 2012 Mattt Thompson (http://mattt.me)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//#import "AFHTTPClient.h"
#import "AFHTTPRequestOperation.h"

#import "AFIncrementalStore.h"
#import "TTTStringInflector.h"
#import "AFNetworking.h"
#import "AFHTTPSessionManager.h"

@protocol AFPaginator;

/**
 `AFRESTClient` is a subclass of `AFHTTPClient` that implements the `AFIncrementalStoreHTTPClient` protocol in a way that follows the conventions of a RESTful web service.

*/
typedef void (^AFCompletionBlock)(void);

@interface AFHTTPRequestOperationManager (client)



///----------------------------------------
/// @name Managing Enqueued HTTP Operations
///----------------------------------------

/**
 Enqueues an `AFHTTPRequestOperation` to the HTTP client's operation queue.

 @param operation The HTTP request operation to be enqueued.
 */
- (void)enqueueHTTPRequestOperation:(AFHTTPRequestOperation *)operation;

/**
 Cancels all operations in the HTTP client's operation queue whose URLs match the specified HTTP method and path.

 This method only cancels `AFHTTPRequestOperations` whose request URL matches the HTTP client base URL with the path appended. For complete control over the lifecycle of enqueued operations, you can access the `operationQueue` property directly, which allows you to, for instance, cancel operations filtered by a predicate, or simply use `-cancelAllRequests`. Note that the operation queue may include non-HTTP operations, so be sure to check the type before attempting to directly introspect an operation's `request` property.

 @param method The HTTP method to match for the cancelled requests, such as `GET`, `POST`, `PUT`, or `DELETE`. If `nil`, all request operations with URLs matching the path will be cancelled.
 @param path The path appended to the HTTP client base URL to match against the cancelled requests. If `nil`, no path will be appended to the base URL.
 */
- (void)cancelAllHTTPOperationsWithMethod:(NSString *)method path:(NSString *)path;

///---------------------------------------
/// @name Batching HTTP Request Operations
///---------------------------------------

/**
 Creates and enqueues an `AFHTTPRequestOperation` to the HTTP client's operation queue for each specified request object into a batch. When each request operation finishes, the specified progress block is executed, until all of the request operations have finished, at which point the completion block also executes.

 Operations are created by passing the specified `NSURLRequest` objects in `requests`, using `-HTTPRequestOperationWithRequest:success:failure:`, with `nil` for both the `success` and `failure` parameters.

 @param urlRequests The `NSURLRequest` objects used to create and enqueue operations.
 @param progressBlock A block object to be executed upon the completion of each request operation in the batch. This block has no return value and takes two arguments: the number of operations that have already finished execution, and the total number of operations.
 @param completionBlock A block object to be executed upon the completion of all of the request operations in the batch. This block has no return value and takes a single argument: the batched request operations.
 */
- (void)enqueueBatchOfHTTPRequestOperationsWithRequests:(NSArray *)urlRequests
                                          progressBlock:(void (^)(NSUInteger numberOfFinishedOperations, NSUInteger totalNumberOfOperations))progressBlock
                                        completionBlock:(void (^)(NSArray *operations))completionBlock;

/**
 Enqueues the specified request operations into a batch. When each request operation finishes, the specified progress block is executed, until all of the request operations have finished, at which point the completion block also executes.

 @param operations The request operations used to be batched and enqueued.
 @param progressBlock A block object to be executed upon the completion of each request operation in the batch. This block has no return value and takes two arguments: the number of operations that have already finished execution, and the total number of operations.
 @param completionBlock A block object to be executed upon the completion of all of the request operations in the batch. This block has no return value and takes a single argument: the batched request operations.
 */
- (void)enqueueBatchOfHTTPRequestOperations:(NSArray *)operations
                              progressBlock:(void (^)(NSUInteger numberOfFinishedOperations, NSUInteger totalNumberOfOperations))progressBlock
                            completionBlock:(void (^)(NSArray *operations))completionBlock;



@end


@interface AFRESTClient : AFHTTPRequestOperationManager <AFIncrementalStoreHTTPClient>

/**

 */
@property (nonatomic, strong) id <AFPaginator> paginator;

/**
 
 */
@property (readonly, nonatomic, strong) TTTStringInflector *inflector;

///------------------------
/// @name Determining Paths
///------------------------

/**
 Returns the request path for a collection of resources of the specified entity. By default, this returns an imprecise pluralization of the entity name.
 
 @discussion The return value of this method is used as the `path` parameter in other `AFHTTPClient` methods.
 
 @param entity The entity used to determine the resources path.
 
 @return An `NSString` representing the request path.
 */
- (NSString *)pathForEntity:(NSEntityDescription *)entity;

/**
 Returns the request path for the resource of a particular managed object. By default, this returns an imprecise pluralization of the entity name, with the additional path component of the resource identifier corresponding to the managed object.
 
 @discussion The return value of this method is used as the `path` parameter in other `AFHTTPClient` methods.
 
 @param object The managed object used to determine the resource path.
 
 @return An `NSString` representing the request path.
 */
- (NSString *)pathForObject:(NSManagedObject *)object;

/**
 Returns the request path for the resource of a particular managed object. By default, this returns an imprecise pluralization of the entity name, with the additional path component of either an imprecise pluralization of the relationship destination entity name if the relationship is to-many, or the relationship destination entity name if to-one.
 
 @discussion The return value of this method is used as the `path` parameter in other `AFHTTPClient` methods.
 
 @param relationship The relationship used to determine the resource path
 @param object The managed object used to determine the resource path.
 
 @return An `NSString` representing the request path.
 */
- (NSString *)pathForRelationship:(NSRelationshipDescription *)relationship
                        forObject:(NSManagedObject *)object;

@end

///-----------------
/// @name Pagination
///-----------------

/**
 
 */
@protocol AFPaginator <NSObject>
- (NSDictionary *)parametersForFetchRequest:(NSFetchRequest *)fetchRequest;
@end

/**
 
 */
@interface AFLimitAndOffsetPaginator : NSObject <AFPaginator>

@property (readonly, nonatomic, copy) NSString *limitParameter;
@property (readonly, nonatomic, copy) NSString *offsetParameter;

+ (instancetype)paginatorWithLimitParameter:(NSString *)limitParameterName
                            offsetParameter:(NSString *)offsetParameterName;
@end

/**
 
 */
@interface AFPageAndPerPagePaginator : NSObject <AFPaginator>

@property (readonly, nonatomic, copy) NSString *pageParameter;
@property (readonly, nonatomic, copy) NSString *perPageParameter;

+ (instancetype)paginatorWithPageParameter:(NSString *)pageParameterName
                          perPageParameter:(NSString *)perPageParameterName;
@end

/**
 
 */
@interface AFBlockPaginator : NSObject <AFPaginator>
+ (instancetype)paginatorWithBlock:(NSDictionary * (^)(NSFetchRequest *fetchRequest))block;
@end
