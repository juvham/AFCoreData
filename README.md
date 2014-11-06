AFCoreData
==========

使用 AFNetworking2.0 集成了coredata 做本地缓存

这个小 demo 改自afnetworing 提供的 AFIncrementalStore
使用最新版本的AFNetworking

1. 使用该项目提供的 AFCoreData 需要重写 AppDelegate 下 的  - (NSManagedObjectContext *)managedObjectContext ，- (NSPersistentStoreCoordinator *)persistentStoreCoordinato 两个方法

demo  
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
    // The persistent store coordinator for the application. This implementation creates and return a coordinator, having added the store for the application to it.
    if (_persistentStoreCoordinator != nil) {
        return _persistentStoreCoordinator;
    }
    
    // Create the coordinator and store
    
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];

    AFIncrementalStore *incrementalStore = (AFIncrementalStore *)[_persistentStoreCoordinator addPersistentStoreWithType:[AppDotNetIncrementalStore type] configuration:nil URL:nil options:nil error:nil];
    NSURL *storeURL = [[self applicationDocumentsDirectory] URLByAppendingPathComponent:@"CoreDataTest.sqlite"];

    NSDictionary *options = @{
                              NSInferMappingModelAutomaticallyOption : @(YES),
                              NSMigratePersistentStoresAutomaticallyOption: @(YES)
                              };
    NSError *error = nil;

    NSString *failureReason = @"There was an error creating or loading the application's saved data.";
    if (![incrementalStore.backingPersistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeURL options:options error:&error]) {
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }

    return _persistentStoreCoordinator;
}


- (NSManagedObjectContext *)managedObjectContext {
    // Returns the managed object context for the application (which is already bound to the persistent store coordinator for the application.)
    if (_managedObjectContext != nil) {
        return _managedObjectContext;
    }
    
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (!coordinator) {
        return nil;
    }
    _managedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
    [_managedObjectContext setPersistentStoreCoordinator:coordinator];

    NSLog(@"managedObjectContext success %@", _managedObjectContext);
    
    return _managedObjectContext;
}

2. 继承 AFIncrementalStore 重写 + (NSString *)type ，+ (NSManagedObjectModel *)model 两个方法

示例代码见本工程

3. 继承 AFRESTClient 自定义 inflector ，paginator 或者重写 #pragma mark Read Methods 下的方法

详细见工程
