#import <substrate.h>
#import "GUI.h"

static bool aimbotEnabled = true;

%hook PlayerManager

- (void)update {
    %orig;
    
    if (aimbotEnabled) {
        id nearest = [self findNearestEnemy];
        if (nearest) {
            [self lockAimOnPlayer:nearest];
            NSLog(@"[Aimbot] Locked on nearest player");
        }
    }
}

%end

%ctor {
    [GUI initMenu];
    NSLog(@"[+] Fortnite Cheat with GUI Loaded");
}
