#include <Appkit/AppKit.h>

#include "PopoverWidget.h"
#include <QSystemTrayIcon>
#include <QDebug>
#include <QPixmap>
#include <QtMac>
#include <QVBoxLayout>
#include <QMap>
#include <QString>

constexpr int StatusBarIconSize = 20;
constexpr auto trayPixmap = ":/app/images/appicon/mono/appicon-32x32.png";

@implementation StatusBarHelper

- (void) statusBarItemClicked:(NSStatusBarButton *) sender {
    m_viewController = [[NSViewController alloc] init];

    // create the popover

    m_entryPopover = [[NSPopover alloc] init];

    [m_entryPopover setContentSize: NSMakeSize(200.0, 200.0)];
    [m_entryPopover setBehavior: NSPopoverBehaviorTransient];
    [m_entryPopover setAnimates: YES];
    [m_entryPopover setContentViewController: m_viewController];

    qDebug() << m_contentWidget->parentWidget();

    auto contentWidgetView = reinterpret_cast<NSView *>(m_contentWidget->winId());

    m_nativeView = [[NSView alloc] init];

    [m_nativeView addSubview: contentWidgetView];
    qDebug() << m_contentWidget->parentWidget();
    m_contentWidget->show();

    [m_viewController setView: contentWidgetView];

    // show the popover in the correct place

    NSRect entryRect = [sender visibleRect];

    [m_entryPopover showRelativeToRect: entryRect
                                ofView: sender
                         preferredEdge: NSMinYEdge];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receivePopoverClosedNotification:)
                                                 name:@"NSPopoverDidCloseNotification"
                                               object:nil];
}

- (void) receivePopoverClosedNotification:(NSNotification *) notification {
    delete m_contentWidget;
}

- (void) setContentWidget:(QWidget *) contentWidget {
    m_contentWidget = contentWidget;
}

StatusBarHelper *m_statusBarHelper;
NSStatusItem *m_statusBarItem;

extern "C" QMap<QString, void *> addStatusIcon(QWidget *contentWidget) {
    QMap<QString, void *> map;

    NSStatusBar *bar = [NSStatusBar systemStatusBar];

    m_statusBarItem = [bar statusItemWithLength:NSSquareStatusItemLength];

    m_statusBarHelper = [[StatusBarHelper alloc] init];

    [m_statusBarHelper retain];

    [m_statusBarHelper setContentWidget:contentWidget];

    QPixmap trayIcon(trayPixmap);

    trayIcon = trayIcon.scaled(StatusBarIconSize, StatusBarIconSize);

    CGImageRef imageRef = trayIcon.toImage().toCGImage();

    NSImage *m_nsImage = [[NSImage alloc] initWithCGImage:imageRef
                                                     size:NSMakeSize(trayIcon.width(), trayIcon.height())];

    NSStatusBarButton *m_button = [m_statusBarItem button];

    [m_button setImage:m_nsImage];

    [m_button setTarget:m_statusBarHelper];
    [m_button setAction:@selector(statusBarItemClicked:)];

    map["m_button"] = m_button;
    map["m_nsImage"] = m_nsImage;
    map["m_statusBarHelper"] = m_statusBarHelper;
    map["m_statusBarItem"] = m_statusBarItem;

    return map;
}

extern "C" void removeStatusIcon(QMap<QString, void *> map) {
    NSStatusBar *bar = [NSStatusBar systemStatusBar];

    NSStatusItem *statusBarItem = (NSStatusItem*) map["m_statusBarItem"];

    qDebug() << "removing!";

    [bar removeStatusItem: statusBarItem];
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