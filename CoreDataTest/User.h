//
//  User.h
//  CoreDataTest
//
//  Created by Juvham on 14/11/4.
//  Copyright (c) 2014年 GouMin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class Post;

@interface User : NSManagedObject

@property (nonatomic, retain) NSString * avatarImageURLString;
@property (nonatomic, retain) NSNumber * userID;
@property (nonatomic, retain) NSString * username;
@property (nonatomic, retain) NSSet *posts;
@end

//@interface User (CoreDataGeneratedAccessors)
//
//- (void)addPostsObject:(Post *)value;
//- (void)removePostsObject:(Post *)value;
//- (void)addPosts:(NSSet *)values;
//- (void)removePosts:(NSSet *)values;
//
//@end
