/** @file
 * VBox Qt GUI - UIMachineLogicFullscreen class declaration.
 */

/*
 * Copyright (C) 2010-2013 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___UIMachineLogicFullscreen_h___
#define ___UIMachineLogicFullscreen_h___

/* Local includes: */
#include "UIMachineLogic.h"

/* Forward declarations: */
class UIMultiScreenLayout;

/* Fullscreen machine logic implementation: */
class UIMachineLogicFullscreen : public UIMachineLogic
{
    Q_OBJECT;

protected:

    /* Constructor/destructor: */
    UIMachineLogicFullscreen(QObject *pParent, UISession *pSession);
    ~UIMachineLogicFullscreen();

    /* Check if this logic is available: */
    bool checkAvailability();

    /* Helpers: Multi-screen stuff: */
    void maybeAdjustGuestScreenSize();
    int hostScreenForGuestScreen(int iScreenId) const;
    bool hasHostScreenForGuestScreen(int iScreenId) const;

private slots:

#ifdef RT_OS_DARWIN
    /** Mac OS X: Marks sender() machine-window as 'fullscreen' one. */
    void sltHandleNativeFullscreenDidEnter();
    /** Mac OS X: Marks sender() machine-window as 'non-fullscreen' one,
      * changes visual-state to requested if there is/are no more fullscreen window(s). */
    void sltHandleNativeFullscreenDidExit();

    /** Mac OS X: Requests visual-state change from 'fullscreen' to 'normal' (window). */
    void sltChangeVisualStateToNormal();
    /** Mac OS X: Requests visual-state change from 'fullscreen' to 'seamless'. */
    void sltChangeVisualStateToSeamless();
    /** Mac OS X: Requests visual-state change from 'fullscreen' to 'scale'. */
    void sltChangeVisualStateToScale();
#endif /* RT_OS_DARWIN */

    /* Handler: Console callback stuff: */
    void sltMachineStateChanged();

#ifdef Q_WS_MAC
    void sltChangePresentationMode(bool fEnabled);
#endif /* Q_WS_MAC */

    /** Updates machine-window(s) location/size on screen-layout changes. */
    void sltScreenLayoutChanged();

    void sltGuestMonitorChange(KGuestMonitorChangedEventType changeType, ulong uScreenId, QRect screenGeo);
    void sltHostScreenCountChanged();

private:

    /* Prepare helpers: */
    void prepareActionGroups();
    void prepareActionConnections();
#ifdef Q_WS_MAC
    void prepareOtherConnections();
#endif /* Q_WS_MAC */
    void prepareMachineWindows();
    void prepareMenu();

    /* Cleanup helpers: */
    //void cleanupMenu() {}
    void cleanupMachineWindows();
#ifdef Q_WS_MAC
    //void cleanupOtherConnections() {}
#endif /* Q_WS_MAC */
    void cleanupActionConnections();
    void cleanupActionGroups();

#ifdef Q_WS_MAC
    void setPresentationModeEnabled(bool fEnabled);

    /** Mac OS X: Invalidates 'fullscreen' mode. */
    void invalidateFullscreenMode();
#endif /* Q_WS_MAC */

    /* Variables: */
    UIMultiScreenLayout *m_pScreenLayout;

#ifdef Q_WS_MAC
    /** Mac OS X: Contains machine-window(s) marked as 'fullscreen'. */
    QSet<UIMachineWindow*> m_fullscreenMachineWindows;
    /** Mac OS X: Holds 'fullscreen' mode invalidation status. */
    bool m_fIsFullscreenInvalidated;
#endif /* Q_WS_MAC */

    /* Friend classes: */
    friend class UIMachineLogic;
    friend class UIMachineWindowFullscreen;
    friend class UIMachineViewFullscreen;
};

#endif /* !___UIMachineLogicFullscreen_h___ */

