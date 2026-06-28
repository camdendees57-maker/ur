#import <UIKit/UIKit.h>

@implementation GUI

+ (void)initMenu {
    UIWindow *menuWindow = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    menuWindow.windowLevel = UIWindowLevelAlert + 1;
    menuWindow.hidden = NO;
    
    UIViewController *vc = [[UIViewController alloc] init];
    vc.view.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.8];
    
    UIButton *toggleBtn = [UIButton buttonWithType:UIButtonTypeSystem];
    [toggleBtn setTitle:@"Aimbot: ON" forState:UIControlStateNormal];
    toggleBtn.frame = CGRectMake(50, 100, 300, 60);
    [vc.view addSubview:toggleBtn];
    
    menuWindow.rootViewController = vc;
}

@end
