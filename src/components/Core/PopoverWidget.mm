#include <Appkit/AppKit.h>

#include "PopoverWidget.h"
#include <QSystemTrayIcon>
#include <QDebug>
#include <QPixmap>
#include <QtMac>
#include <QVBoxLayout>

constexpr int StatusBarIconSize = 20;
constexpr auto trayPixmap = ":/app/images/appicon/mono/appicon-32x32.png";

@implementation StatusBarHelper

- (void) statusBarItemClicked:(NSStatusBarButton *) sender {
    NSViewController *viewController = [[NSViewController alloc] init];

    // create the popover

    NSPopover *entryPopover = [[NSPopover alloc] init];

    [entryPopover setContentSize: NSMakeSize(200.0, 200.0)];
    [entryPopover setBehavior: NSPopoverBehaviorTransient];
    [entryPopover setAnimates: YES];
    [entryPopover setContentViewController: viewController];

    // create our qt widget

    auto contentWidget = new QWidget();

    auto layout = new QVBoxLayout;

    layout->addWidget(m_contentWidget);

    contentWidget->setLayout(layout);

    // create a NSView and add our content view as a sub view

    auto contentView = reinterpret_cast<NSView *>(contentWidget->winId());

    NSView *view = [[NSView alloc] init];

    [view addSubview: contentView];

    contentWidget->show();

    [viewController setView: contentView];

    // show the popover in the correct place

    NSRect entryRect = [sender visibleRect];

    [entryPopover showRelativeToRect: entryRect
                              ofView: sender
                       preferredEdge: NSMinYEdge];
}

- (void) setContentWidget:(QWidget *) contentWidget {
    m_contentWidget = contentWidget;
}

extern "C" void addStatusIcon(QWidget *contentWidget) {
    NSStatusBar *bar = [NSStatusBar systemStatusBar];

    NSStatusItem *statusBarItem = [bar statusItemWithLength: NSSquareStatusItemLength];

    StatusBarHelper *statusBarHelper = [[StatusBarHelper alloc] init];

    [statusBarItem retain];

    [statusBarHelper setContentWidget: contentWidget];

    QPixmap trayIcon(trayPixmap);

    trayIcon = trayIcon.scaled(StatusBarIconSize, StatusBarIconSize);

    CGImageRef imageRef = trayIcon.toImage().toCGImage();

    NSImage *nsImage = [[NSImage alloc] initWithCGImage: imageRef
                                                   size: NSMakeSize(trayIcon.width(), trayIcon.height())];

    NSStatusBarButton *button = [statusBarItem button];

    [button setImage: nsImage];

    [button setTarget: statusBarHelper];
    [button setAction: @selector(statusBarItemClicked:)];
}

void showPopover(QSystemTrayIcon *systemTrayIcon) {
    /*NSViewController *viewController = [[NSViewController alloc] init];

    NSPopover *entryPopover = [[NSPopover alloc] init];

    [entryPopover setContentSize:NSMakeSize(200.0, 200.0)];
    [entryPopover setBehavior:NSPopoverBehaviorTransient];
    [entryPopover setAnimates:YES];
    [entryPopover setContentViewController:viewController];

    NSRect entryRect = NSRectFromCGRect(parentWidget->geometry().toCGRect());

    auto menuIconView = reinterpret_cast<NSView *>(parentWidget->winId()];

    auto contentWidget = new QWidget();

    auto contentView = reinterpret_cast<NSView *>(contentWidget->winId()];

    [viewController setView: contentView];

    [entryPopover
            showRelativeToRect:entryRect
            ofView:menuIconView
            preferredEdge:NSMinYEdge];*/

    qDebug() << systemTrayIcon->isWidgetType();
}

@end