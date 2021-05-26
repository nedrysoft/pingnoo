/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 27/03/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RouteAnalyserComponent.h"

#include "ColourDialog.h"
#include "IRouteEngine.h"
#include "LatencyRibbonGroup.h"
#include "LatencySettings.h"
#include "LatencySettingsPage.h"
#include "NewTargetDialog.h"
#include "NewTargetRibbonGroup.h"
#include "Pingnoo.h"
#include "PingResult.h"
#include "RouteAnalyser.h"
#include "RouteAnalyserMenuItem.h"
#include "TargetManager.h"
#include "TargetSettings.h"
#include "TargetSettingsPage.h"
#include "ViewportRibbonGroup.h"

#include <AppNap>
#include <ICommandManager>
#include <IEditorManager>
#include <IRibbonBarManager>
#include <IRibbonPage>
#include <ISystemTrayIcon>
#include <ISystemTrayIconManager>
#if defined(Q_OS_MACOS)
#include <MacHelper>
#include <MacMenubarIcon>
#include <MacPopover>
#endif
#include <QDir>
#include <QDirIterator>
#if !defined(Q_OS_MACOS)
#include <QGuiApplication>
#include <QScreen>
#endif
#include <QVBoxLayout>
#include <PopoverWindow.h>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <Shellapi.h>
#endif

constexpr auto FontBasePath = ":/Nedrysoft/RouteAnalyser/Roboto_Mono/static";

RouteAnalyserComponent::RouteAnalyserComponent() :
        m_newTargetGroupWidget(nullptr),
        m_latencyGroupWidget(nullptr),
        m_viewportGroupWidget(nullptr),
        m_latencySettingsPage(nullptr),
        m_targetSettingsPage(nullptr),
        m_newTargetAction(nullptr),
        m_latencySettings(nullptr) {

}

RouteAnalyserComponent::~RouteAnalyserComponent() {

}

auto RouteAnalyserComponent::initialiseEvent() -> void {
    qRegisterMetaType<Nedrysoft::RouteAnalyser::PingResult>("Nedrysoft::RouteAnalyser::PingResult");
    qRegisterMetaType<Nedrysoft::RouteAnalyser::RouteList>("Nedrysoft::RouteAnalyser::RouteList");
    qRegisterMetaType<Nedrysoft::RouteAnalyser::IPingEngineFactory *>("Nedrysoft::RouteAnalyser::IPingEngineFactory *");
}

auto RouteAnalyserComponent::finaliseEvent() -> void {
    auto editorList = Nedrysoft::ComponentSystem::getObjects<Nedrysoft::RouteAnalyser::RouteAnalyserEditor>();

    if (!editorList.isEmpty()) {
        qDeleteAll(editorList);
    }

    if (m_latencySettingsPage) {
        Nedrysoft::ComponentSystem::removeObject(m_latencySettingsPage);

        delete m_latencySettingsPage;
    }

    if (m_targetSettingsPage) {
        Nedrysoft::ComponentSystem::removeObject(m_targetSettingsPage);

        delete m_targetSettingsPage;
    }

    if (m_latencySettings) {
        Nedrysoft::ComponentSystem::removeObject(m_latencySettings);

        delete m_latencySettings;
    }

    if (m_targetSettings) {
        Nedrysoft::ComponentSystem::removeObject(m_targetSettings);

        delete m_targetSettings;
    }

    if (m_viewportGroupWidget) {
        Nedrysoft::ComponentSystem::removeObject(m_viewportGroupWidget);

        //delete m_viewportGroupWidget;
    }

    if (m_latencyGroupWidget) {
        Nedrysoft::ComponentSystem::removeObject(m_latencyGroupWidget);

        //delete m_latencyGroupWidget;
    }

    if (m_newTargetAction) {
        delete m_newTargetAction;
    }

    delete Nedrysoft::RouteAnalyser::TargetManager::getInstance();
}
static int x = 0;

auto RouteAnalyserComponent::initialisationFinishedEvent() -> void {
    auto contextManager = Nedrysoft::Core::IContextManager::getInstance();
    auto appNap = Nedrysoft::AppNap::AppNap::getInstance();

    appNap->prevent(QT_TR_NOOP("App Nap has been disabled as it interferes with thread timing."));

    if (contextManager) {
        m_editorContextId = contextManager->registerContext(Pingnoo::Constants::routeAnalyserContext);

        connect(contextManager, &Nedrysoft::Core::IContextManager::contextChanged,
                [&](int newContext, int previousContext) {
                    Q_UNUSED(newContext)
                    Q_UNUSED(previousContext)
                });
    }

    auto core = Nedrysoft::Core::ICore::getInstance();

    if (core) {
        connect(core, &Nedrysoft::Core::ICore::coreOpened, [&]() {
            auto commandManager = Nedrysoft::Core::ICommandManager::getInstance();

            if (commandManager) {
                // create New Target... action

                m_newTargetAction =  new QAction(tr("New Target..."));

                connect(m_newTargetAction, &QAction::triggered, [=]() {
                    Nedrysoft::RouteAnalyser::NewTargetDialog newTargetDialog;

                    if (newTargetDialog.exec()) {
                        auto editorManager = Nedrysoft::Core::IEditorManager::getInstance();

                        if (editorManager) {
                            Nedrysoft::RouteAnalyser::RouteAnalyserEditor *editor =
                                    new Nedrysoft::RouteAnalyser::RouteAnalyserEditor;

                            editor->setPingEngine(newTargetDialog.pingEngineFactory());
                            editor->setTarget(newTargetDialog.pingTarget());
                            editor->setIPVersion(newTargetDialog.ipVersion());
                            editor->setInterval(newTargetDialog.interval());

                            editorManager->openEditor(editor);
                        }
                    }
                });

                // register File/New Target... menu option global context

                auto command = commandManager->registerAction(m_newTargetAction, "Menu.File.NewTarget");

                auto menu = commandManager->findMenu(Pingnoo::Constants::menuFile);

                menu->appendCommand(command, Pingnoo::Constants::fileNewGroup);
/*
                // create Edit/Cut action for this context

                action = new QAction(Pingnoo::Constants::commandText(Pingnoo::Constants::editCut));

                connect(action, &QAction::triggered, [&](bool) {
                    qDebug() << "action triggered (route analyser) !";
                });

                // register Edit/Cut menu option for this context

                commandManager->registerAction(action, Pingnoo::Constants::editCut, m_editorContextId);

                Nedrysoft::Core::IContextManager::getInstance()->setContext(m_editorContextId);

                action->setEnabled(true);
*/
            }
        });

        m_targetSettings = new Nedrysoft::RouteAnalyser::TargetSettings;

        Nedrysoft::ComponentSystem::addObject(m_targetSettings);

        m_targetSettings->loadFromFile();

        m_latencySettings = new Nedrysoft::RouteAnalyser::LatencySettings;

        Nedrysoft::ComponentSystem::addObject(m_latencySettings);

        m_latencySettings->loadFromFile();

        auto ribbonBarManager = Nedrysoft::Core::IRibbonBarManager::getInstance();

        if (ribbonBarManager) {
            auto ribbonPage = ribbonBarManager->addPage(
                    tr("Route Analyser"),
                    Pingnoo::Constants::ribbonRouteAnalyserPage,
                    0.1f );

            ribbonBarManager->selectPage(Pingnoo::Constants::ribbonRouteAnalyserPage);

            m_newTargetGroupWidget = new Nedrysoft::RouteAnalyser::NewTargetRibbonGroup;
            m_latencyGroupWidget = new Nedrysoft::RouteAnalyser::LatencyRibbonGroup;
            m_viewportGroupWidget = new Nedrysoft::RouteAnalyser::ViewportRibbonGroup;

            ribbonPage->addGroup(
                    tr("New Target"),
                    Pingnoo::Constants::ribbonRouteAnalyserNewTargetGroup,
                    m_newTargetGroupWidget );

            ribbonPage->addGroup(
                    tr("Latency"),
                    Pingnoo::Constants::ribbonRouteAnalyserLatencyGroup,
                    m_latencyGroupWidget );

            ribbonPage->addGroup(
                    tr("Viewport"),
                    Pingnoo::Constants::ribbonRouteAnalyserViewportGroup,
                    m_viewportGroupWidget );
        }

        m_latencySettingsPage = new Nedrysoft::RouteAnalyser::LatencySettingsPage;
        m_targetSettingsPage = new Nedrysoft::RouteAnalyser::TargetSettingsPage;

        Nedrysoft::ComponentSystem::addObject(m_latencySettingsPage);
        Nedrysoft::ComponentSystem::addObject(m_targetSettingsPage);
        Nedrysoft::ComponentSystem::addObject(m_newTargetGroupWidget);
        Nedrysoft::ComponentSystem::addObject(m_viewportGroupWidget);
        Nedrysoft::ComponentSystem::addObject(m_latencyGroupWidget);
    }

    auto dirIterator = QDirIterator(FontBasePath);

    while(dirIterator.hasNext()) {
        dirIterator.next();

        QFontDatabase::addApplicationFont(dirIterator.filePath());
    }

    auto systemTrayIconManager = Nedrysoft::Core::ISystemTrayIconManager::getInstance();

    auto systemTrayIcon = systemTrayIconManager->createIcon();

    systemTrayIcon->setColour(Qt::black);

    connect(Nedrysoft::Core::mainWindow(), &QObject::destroyed, [=](QObject *) {
        delete systemTrayIcon;
    });

    connect(
            systemTrayIcon,
            &Nedrysoft::Core::ISystemTrayIcon::clicked,
            [=](Nedrysoft::Core::ISystemTrayIcon::MouseButton button) {

        if (button==Nedrysoft::Core::ISystemTrayIcon::MouseButton::Left) {
#if defined(Q_OS_MACOS)
            auto popoverWidget = new Nedrysoft::MacHelper::MacPopover;
#else
            auto popoverWidget = new Nedrysoft::RouteAnalyser::PopoverWindow(Nedrysoft::Core::mainWindow());

            auto iconRect = systemTrayIcon->geometry();
#endif
            auto contentLayout = new QVBoxLayout;

            for (int i = 0; i < 5; i++) {
                contentLayout->addWidget(new Nedrysoft::RouteAnalyser::RouteAnalyserMenuItem);
            }

            popoverWidget->setLayout(contentLayout);
#if defined(Q_OS_MACOS)
            popoverWidget->show(
                    systemtrayIcon->menubarIcon(),
                    contentWidget,
                    QSize(contentWidget->minimumWidth(), contentWidget->sizeHint().height()),
                    Nedrysoft::MacHelper::MacPopover::Edge::MaxYEdge
            );
#elif defined(Q_OS_WINDOWS)
            APPBARDATA appbarData;

            memset(&appbarData, 0, sizeof(appbarData));

            appbarData.cbSize = sizeof(appbarData);

            SHAppBarMessage(ABM_GETTASKBARPOS, &appbarData);

            for (auto screen : qGuiApp->screens()) {
                if (screen->geometry().contains(iconRect)) {
                    QRect popoverRect = QRect(QPoint(0,0), popoverWidget->sizeHint());

                    switch(appbarData.uEdge) {
                        case ABE_TOP: {
                            popoverRect.moveTopLeft(
                                    QPoint(
                                            iconRect.center().x()-(popoverRect.width()/2),
                                            iconRect.bottom()
                                    )
                            );

                            if (popoverRect.right()>screen->geometry().right()) {
                                popoverRect.moveRight(screen->geometry().right());
                            }

                            popoverWidget->move(popoverRect.topLeft());

                            break;
                        }

                        case ABE_BOTTOM: {
                            popoverRect.moveBottomRight(
                                    QPoint(
                                            iconRect.center().x()+(popoverRect.width()/2),
                                            iconRect.top()
                                    )
                            );

                            if (popoverRect.right()>screen->geometry().right()) {
                                popoverRect.moveRight(screen->geometry().right());
                            }

                            popoverWidget->move(popoverRect.topLeft());

                            qDebug() << popoverRect;

                            break;
                        }

                        case ABE_LEFT: {
                            popoverRect.moveTopLeft(
                                    QPoint(
                                            iconRect.right(),
                                            iconRect.center().y()-(popoverRect.height()/2)

                                    )
                            );

                            if (popoverRect.bottom()>screen->geometry().bottom()) {
                                popoverRect.moveBottom(screen->geometry().bottom());
                            }

                            popoverWidget->move(popoverRect.topLeft());

                            break;
                        }

                        case ABE_RIGHT: {
                            popoverRect.moveTopLeft(
                                    QPoint(
                                            iconRect.right(),
                                            iconRect.center().y()-(popoverRect.height()/2)

                                    )
                            );

                            if (popoverRect.bottom()>screen->geometry().bottom()) {
                                popoverRect.moveBottom(screen->geometry().bottom());
                            }

                            popoverWidget->move(popoverRect.topLeft());

                            break;
                        }
                    }

                    break;
                }
            }

            popoverWidget->show();
#endif
            connect(this, &RouteAnalyserComponent::destroyed, [=]() {
                delete popoverWidget;
            });
        } else if (button==Nedrysoft::Core::ISystemTrayIcon::MouseButton::Right) {
#if defined(Q_OS_MACOS)
            if (x==0) {
                Nedrysoft::MacHelper::MacHelper::hideApplication();
            } else {
                Nedrysoft::MacHelper::MacHelper::showApplication();
            }
#endif
        }
    });
}

auto RouteAnalyserComponent::contextId() -> int {
    return m_editorContextId;
}