//
//  Post.h
//  CoreDataTest
//
//  Created by Juvham on 14/11/4.
//  Copyright (c) 2014年 GouMin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "User.h"

@interface Post : NSManagedObject

@property (nonatomic, retain) NSDate * createdAt;
@property (nonatomic, retain) NSNumber * postID;
@property (nonatomic, retain) NSString * text;
@property (nonatomic, retain) User *user;

@end
