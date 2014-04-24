#include "testApp.h"

#import "MyUIDelegate.h"


@implementation MyUIDelegate


-(void)awakeFromNib {
#ifdef MAPPER_DEBUG
	NSLog(@"MyUIDelegate::awakeFromNib");
#endif
}

@end
