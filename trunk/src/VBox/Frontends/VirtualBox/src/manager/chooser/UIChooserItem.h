/* $Id$ */
/** @file
 * VBox Qt GUI - UIChooserItem class declaration.
 */

/*
 * Copyright (C) 2012-2018 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___UIChooserItem_h___
#define ___UIChooserItem_h___

/* Qt includes: */
#include <QMimeData>
#include <QRectF>
#include <QString>

/* GUI includes: */
#include "QIGraphicsWidget.h"
#include "QIWithRetranslateUI.h"

/* Other VBox includes: */
#include <iprt/cdefs.h>

/* Forward declaration: */
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneDragDropEvent;
class QPropertyAnimation;
class QStateMachine;
class UIActionPool;
class UIChooserItemGroup;
class UIChooserItemGlobal;
class UIChooserItemMachine;
class UIChooserModel;


/** UIChooserItem types. */
enum UIChooserItemType
{
    UIChooserItemType_Any     = QGraphicsItem::UserType,
    UIChooserItemType_Group,
    UIChooserItemType_Global,
    UIChooserItemType_Machine
};


/** UIChooserItem search flags. */
enum UIChooserItemSearchFlag
{
    UIChooserItemSearchFlag_Machine   = RT_BIT(0),
    UIChooserItemSearchFlag_Global    = RT_BIT(1),
    UIChooserItemSearchFlag_Group     = RT_BIT(2),
    UIChooserItemSearchFlag_ExactName = RT_BIT(3)
};


/** Drag token placement types. */
enum DragToken
{
    DragToken_Off,
    DragToken_Up,
    DragToken_Down
};


/** QIGraphicsWidget extension used as interface
  * for graphics chooser model/view architecture. */
class UIChooserItem : public QIWithRetranslateUI4<QIGraphicsWidget>
{
    Q_OBJECT;
    Q_PROPERTY(int animationDarkness READ animationDarkness WRITE setAnimationDarkness);

signals:

    /** @name Basic stuff.
      * @{ */
        /** Notifies listeners about hover enter. */
        void sigHoverEnter();
        /** Notifies listeners about hover leave. */
        void sigHoverLeave();
    /** @} */

    /** @name Layout stuff.
      * @{ */
        /** Notifies listeners about @a iMinimumWidthHint change. */
        void sigMinimumWidthHintChanged(int iMinimumWidthHint);
        /** Notifies listeners about @a iMinimumHeightHint change. */
        void sigMinimumHeightHintChanged(int iMinimumHeightHint);
    /** @} */

public:

    /** Constructs item passing @a pParent to the base-class.
      * @param  fTemporary  Brings whether this item created for temporary needs. */
    UIChooserItem(UIChooserItem *pParent, bool fTemporary);

    /** @name Item stuff.
      * @{ */
        /** Returns parent  reference. */
        UIChooserItem *parentItem() const {  return m_pParent; }
        /** Returns whether item is temporary. */
        bool isTemporary() const { return m_fTemporary; }

        /** Casts item to group one. */
        UIChooserItemGroup *toGroupItem();
        /** Casts item to global one. */
        UIChooserItemGlobal *toGlobalItem();
        /** Casts item to machine one. */
        UIChooserItemMachine *toMachineItem();

        /** Returns model reference. */
        UIChooserModel *model() const;
        /** Returns action-pool reference. */
        UIActionPool *actionPool() const;

        /** Shows item. */
        virtual void show();
        /** Hides item. */
        virtual void hide();

        /** Starts item editing. */
        virtual void startEditing() = 0;

        /** Updates item tool-tip. */
        virtual void updateToolTip() = 0;

        /** Returns item name. */
        virtual QString name() const = 0;
        /** Returns item description. */
        virtual QString description() const = 0;
        /** Returns item full-name. */
        virtual QString fullName() const = 0;
        /** Returns item definition. */
        virtual QString definition() const = 0;

        /** Defines whether item is @a fRoot. */
        void setRoot(bool fRoot);
        /** Returns whether item is root. */
        bool isRoot() const;

        /** Defines whether item is @a fHovered. */
        void setHovered(bool fHovered);
        /** Returns whether item is hovered. */
        bool isHovered() const;
    /** @} */

    /** @name Children stuff.
      * @{ */
        /** Adds child @a pItem to certain @a iPosition. */
        virtual void addItem(UIChooserItem *pItem, int iPosition) = 0;
        /** Removes child @a pItem. */
        virtual void removeItem(UIChooserItem *pItem) = 0;

        /** Replaces children @a items of certain @a enmType. */
        virtual void setItems(const QList<UIChooserItem*> &items, UIChooserItemType enmType) = 0;
        /** Returns children items of certain @a enmType. */
        virtual QList<UIChooserItem*> items(UIChooserItemType enmType = UIChooserItemType_Any) const = 0;
        /** Returns whether there are children items of certain @a enmType. */
        virtual bool hasItems(UIChooserItemType enmType = UIChooserItemType_Any) const = 0;
        /** Clears children items of certain @a enmType. */
        virtual void clearItems(UIChooserItemType enmType = UIChooserItemType_Any) = 0;

        /** Updates all children items with specified @a strId. */
        virtual void updateAllItems(const QString &strId) = 0;
        /** Removes all children items with specified @a strId. */
        virtual void removeAllItems(const QString &strId) = 0;

        /** Searches for a first child item answering to specified @a strSearchTag and @a iItemSearchFlags. */
        virtual UIChooserItem *searchForItem(const QString &strSearchTag, int iItemSearchFlags) = 0;

        /** Searches for a first machine child item. */
        virtual UIChooserItem *firstMachineItem() = 0;

        /** Sorts children items. */
        virtual void sortItems() = 0;
    /** @} */

    /** @name Layout stuff.
      * @{ */
        /** Updates geometry. */
        void updateGeometry();

        /** Updates layout.  */
        virtual void updateLayout() = 0;

        /** Returns minimum width-hint. */
        virtual int minimumWidthHint() const = 0;
        /** Returns minimum height-hint. */
        virtual int minimumHeightHint() const = 0;
    /** @} */

    /** @name Navigation stuff.
      * @{ */
        /** Makes sure item is visible. */
        virtual void makeSureItsVisible();

        /** Returns pixmap item representation. */
        virtual QPixmap toPixmap() = 0;

        /** Returns whether item drop is allowed.
          * @param  pEvent    Brings information about drop event.
          * @param  enmPlace  Brings the place of drag token to the drop moment. */
        virtual bool isDropAllowed(QGraphicsSceneDragDropEvent *pEvent, DragToken enmPlace = DragToken_Off) const = 0;
        /** Processes item drop.
          * @param  pEvent    Brings information about drop event.
          * @param  pFromWho  Brings the item according to which we choose drop position.
          * @param  enmPlace  Brings the place of drag token to the drop moment (according to item mentioned above). */
        virtual void processDrop(QGraphicsSceneDragDropEvent *pEvent, UIChooserItem *pFromWho = 0, DragToken enmPlace = DragToken_Off) = 0;
        /** Reset drag token. */
        virtual void resetDragToken() = 0;

        /** Defines drag token @a enmPlace. */
        void setDragTokenPlace(DragToken enmPlace);
        /** Returns drag token place. */
        DragToken dragTokenPlace() const;
    /** @} */

protected:

    /** @name Event-processing stuff.
      * @{ */
        /** Handles hover enter @a event. */
        void hoverMoveEvent(QGraphicsSceneHoverEvent *pEvent);
        /** Handles hover leave @a event. */
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *pEvent);

        /** Handles mouse press @a event. */
        void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
        /** Handles mouse move @a event. */
        void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);

        /** Handles drag move @a event. */
        void dragMoveEvent(QGraphicsSceneDragDropEvent *pEvent);
        /** Handles drag leave @a event. */
        void dragLeaveEvent(QGraphicsSceneDragDropEvent *pEvent);
        /** Handles drop @a event. */
        void dropEvent(QGraphicsSceneDragDropEvent *pEvent);
    /** @} */

    /** @name Item stuff.
      * @{ */
        /** Handles root status change. */
        virtual void handleRootStatusChange();

        /** Defines item's @a iDefaultDarkness. */
        void setDefaultDarkness(int iDefaultDarkness) { m_iDefaultDarkness = iDefaultDarkness; update(); }
        /** Returns default item darkness. */
        int defaultDarkness() const { return m_iDefaultDarkness; }

        /** Defines item's @a iHighlightDarkness. */
        void setHighlightDarkness(int iHighlightDarkness) { m_iHighlightDarkness = iHighlightDarkness; update(); }
        /** Returns highlight item darkness. */
        int highlightDarkness() const { return m_iHighlightDarkness; }

        /** Defines item's @a iAnimationDarkness. */
        void setAnimationDarkness(int iAnimationDarkness) { m_iAnimationDarkness = iAnimationDarkness; update(); }
        /** Returns animation item darkness. */
        int animationDarkness() const { return m_iAnimationDarkness; }
    /** @} */

    /** @name Layout stuff.
      * @{ */
        /** Defines previous @a geometry. */
        void setPreviousGeometry(const QRectF &geometry) { m_previousGeometry = geometry; }
        /** Returns previous geometry. */
        const QRectF &previousGeometry() const { return m_previousGeometry; }

        /** Returns @a strText size calculated on the basis of certain @a font and @a pPaintDevice. */
        static QSize textSize(const QFont &font, QPaintDevice *pPaintDevice, const QString &strText);
        /** Returns a width of line containing @a iCount of chars calculated on the basis of certain @a font and @a pPaintDevice. */
        static int textWidth(const QFont &font, QPaintDevice *pPaintDevice, int iCount);
        /** Compresses @a strText to @a iWidth on the basis of certain @a font and @a pPaintDevice. */
        static QString compressText(const QFont &font, QPaintDevice *pPaintDevice, QString strText, int iWidth);
    /** @} */

    /** @name Navigation stuff.
      * @{ */
        /** Returns D&D mime data. */
        virtual QMimeData *createMimeData() = 0;

        /** Returns drag token darkness. */
        int dragTokenDarkness() const { return m_iDragTokenDarkness; }
    /** @} */

    /** @name Painting stuff.
      * @{ */
        /** Configures @a pPainter shape with certain @a pOptions and specified @a iRadius. */
        static void configurePainterShape(QPainter *pPainter, const QStyleOptionGraphicsItem *pOptions, int iRadius);

        /** Paints frame @a rectangle using passed @a pPainter.
          * @param  fIsSelected  Brings whether this rectangle should be filled.
          * @param  iRadius      Brings the radius of rounded corners. */
        static void paintFrameRect(QPainter *pPainter, bool fIsSelected, int iRadius,
                                   const QRect &rectangle);
        /** Paints @a pixmap using passed @a pPainter putting its upper-left corner to specified @a point. */
        static void paintPixmap(QPainter *pPainter, const QPoint &point,
                                const QPixmap &pixmap);
        /** Paints @a strText using passed @a pPainter putting its upper-left corner to specified @a point.
          * @param  font          Brings the text font.
          * @param  pPaintDevice  Brings the paint-device reference to initilize painting from. */
        static void paintText(QPainter *pPainter, QPoint point,
                              const QFont &font, QPaintDevice *pPaintDevice,
                              const QString &strText);
    /** @} */

private:

    /** @name Item stuff.
      * @{ */
        /** Holds the item's parent item. */
        UIChooserItem *m_pParent;
        /** Holds whether item is temporary. */
        bool           m_fTemporary;

        /** Holds whether item is root. */
        bool  m_fRoot;
        /** Holds whether item is hovered. */
        bool  m_fHovered;

        /** Holds item highlight machine instance. */
        QStateMachine *m_pHighlightMachine;

        /** Holds item forward animation instance. */
        QPropertyAnimation *m_pForwardAnimation;
        /** Holds item backward animation instance. */
        QPropertyAnimation *m_pBackwardAnimation;

        /** Holds item animation duration. */
        int  m_iAnimationDuration;

        /** Holds default item darkness. */
        int  m_iDefaultDarkness;
        /** Holds highlight item darkness. */
        int  m_iHighlightDarkness;
        /** Holds animation item darkness. */
        int  m_iAnimationDarkness;
    /** @} */

    /** @name Layout stuff.
      * @{ */
        /** Holds previous geometry. */
        QRectF  m_previousGeometry;

        /** Holds previous minimum width hint. */
        int  m_iPreviousMinimumWidthHint;
        /** Holds previous minimum height hint. */
        int  m_iPreviousMinimumHeightHint;
    /** @} */

    /** @name Navigation stuff.
      * @{ */
        /** Holds drag token place. */
        DragToken  m_enmDragTokenPlace;

        /** Holds drag token darkness. */
        int  m_iDragTokenDarkness;
    /** @} */
};


/** QMimeData for graphics item interface. */
class UIChooserItemMimeData : public QMimeData
{
    Q_OBJECT;

public:

    /** Constructs mime-data on the basis of passed @a pItem. */
    UIChooserItemMimeData(UIChooserItem *pItem);

    /** Returns cached item. */
    UIChooserItem *item() const { return m_pItem; }

    /** Constructs mime-data on the basis of passed @a pItem. */
    virtual bool hasFormat(const QString &strMimeType) const /* override */;

private:

    /** Holds the cached item. */
    UIChooserItem *m_pItem;
};


#endif /* !___UIChooserItem_h___ */
