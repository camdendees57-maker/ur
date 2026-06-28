#import <substrate.h>
#import <Foundation/Foundation.h>

%hook PlayerManager

- (void)update {
    %orig;
    NSLog(@"[FortniteCheat] Aimbot running");
}

%end

%ctor {
    NSLog(@"[+] Fortnite Aimbot loaded successfully");
}
