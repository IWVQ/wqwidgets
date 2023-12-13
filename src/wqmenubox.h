// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQMENUBOX_H
#define WQMENUBOX_H

#include <QtWidgets>

#define WQ_MENU_ITEM_ACTION    0
#define WQ_MENU_ITEM_SEPARATOR 1
#define WQ_MENU_ITEM_SECTION   2

class WQMenuBoxPrivate;

class WQActionItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit WQActionItemDelegate(QObject *parent = nullptr);
    ~WQActionItemDelegate();
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    virtual void drawArrowGlyph(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index,
                        QAction *action,
                        const QRect &area)const;
    virtual void drawCheckGlyph(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index,
                        QAction *action,
                        const QRect &area, bool iconCheck)const;
    virtual void drawItemIcon(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index,
                        QAction *action,
                        const QRect &area, bool iconCheck)const;
    virtual int arrowWidthHint()const {return 25;};
    virtual int checkWidthHint()const {return 25;};
    virtual int actionHeightHint()const {return 25;};
    virtual int separatorHeightHint()const {return 5;};
    virtual int sectionHeightHint()const {return 17;};
};

class WQMenuBox;

class WQMenuFilter: public QObject
{
    Q_OBJECT
public:
    WQMenuFilter(QObject *parent = nullptr);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    WQMenuBox *menubox = nullptr;
};

class Q_DECL_EXPORT WQMenuBox : public QListWidget
{
    Q_OBJECT
    Q_PROPERTY(QFont separatorFont READ separatorFont WRITE setSeparatorFont)
    Q_PROPERTY(bool showSeparatorTitle READ showSeparatorTitle WRITE setShowSeparatorTitle)
    Q_PROPERTY(bool canPopupSubmenu READ canPopupSubmenu WRITE setCanPopupSubmenu)
    Q_PROPERTY(bool activateWithSel READ activateWithSel WRITE setActivateWithSel)
public:
    explicit WQMenuBox(QWidget* parent = nullptr);
    ~WQMenuBox() override;

    void clear();
    void setMenu(QMenu *m);

    QMenu *backward();
    QMenu *forward();

    QMenu *openSubmenu(int i);
    QMenu *openSubmenu(QAction *a); // only for this level
    bool gotoAction(QAction *action);
    QAction *activateAction(int i, bool simulate = false);
    void popupSubmenu(int i, QPoint p);

    QMenu *menu();
    QMenu *submenu();
    QAction *activeAction();
    QAction *hoveredAction();

    bool canGoForward();
    bool canGoBackward();

    QAction *actionFromIndex(int i);
    int indexFromAction(QAction *a);
    int findAction(QAction *a, QMenu* &subm);

    QFont separatorFont();
    void setSeparatorFont(QFont f);

    bool showSeparatorTitle();
    void setShowSeparatorTitle(bool b);

    bool canPopupSubmenu();
    void setCanPopupSubmenu(bool b);

    bool activateWithSel();
    void setActivateWithSel(bool b);

    void refreshMenu();
    int itemKind(int i);

    double itemSizeFactor();
    void setItemSizeFactor(double f);

    bool showCheckGutter();
    void setShowCheckGutter(bool b);
signals:
    void menuChanged();
    void actionActivated();
    void actionHovered();
protected slots:
    void onItemActivated(QListWidgetItem *item);
    void onCurrentRowChanged(int currentRow);
protected:
    void hoverAction(int i);
    void removeMenuActions();
    void appendMenuActions();

    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
private:
    std::unique_ptr<class WQMenuBoxPrivate> const d;
};
/*
 *
 *
activate
- left click
- double click
- item activation
- return reactivates

- keyboard ^v arrows
- keyboard ^v moves and activates

- keyboard <>BACK moves throught the path
- return opens a submenu

- rightclick shows popup submenu

- hovered with mouse
- stay with mouse

*/


#endif // WQMENUBOX_H
