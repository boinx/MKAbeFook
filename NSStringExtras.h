//
//  NSStringExtras.h
//  MKAbeFook
//
//  Created by Mike Kinney on 9/20/09.
//  Copyright 2009 Mike Kinney. All rights reserved.
//

#import <Cocoa/Cocoa.h>


/*
 \category StringExtras(NSString)
 Extra string methods.
 */
@interface NSString (StringExtras)

/*
 Prepares string so it can be passed in a URL.
 */
- (NSString *) encodeURLLegally;

/*
 Returns the string found between start and stop, nil if no string was found
 */
- (NSString *)substringBetweenString:(NSString *)start andString:(NSString *)stop;
@end