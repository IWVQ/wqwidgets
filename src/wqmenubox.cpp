// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqmenubox.h"

class WQMenuBoxPrivate
{
public:
    WQMenuBoxPrivate(WQMenuBox *qq);
    ~WQMenuBoxPrivate();
    void setMenu(QMenu *m);
    void openSubmenu(QMenu *m);
    QMenu *current();
    QMenu *backward();
    QMenu *forward();
    bool canGoBackward();
    bool canGoForward();
    void ensureRoom();


    WQMenuBox *q;

    QMenu *menu = nullptr;
    QMenu* *path = nullptr;
    int pathsize = 0;
    int pathcount = 0;
    int currpath = -1;
    int activeitem = -1;
    int hovereditem = -1;
    int mouseitem = -1;

    bool showcheckgutter = false;
    double itemsizefactor = 1;
    bool activatewithsel = false;
    int arrowwidth = 25;
    QFont separatorfont;
    bool canpopupsubmenu = true;
    bool showseparatortitle = true;

    WQActionItemDelegate *actiondelegate;
    WQMenuFilter *menufilter;
};

WQMenuBoxPrivate::WQMenuBoxPrivate(WQMenuBox *qq): q(qq)
{
    actiondelegate = new WQActionItemDelegate(q);
    menufilter = new WQMenuFilter(q);
}

WQMenuBoxPrivate::~WQMenuBoxPrivate()
{
    if(path) delete [] path;
}

void WQMenuBoxPrivate::ensureRoom()
{
    if (pathcount == pathsize){
        pathsize += 4;
        QMenu* *newpath = new QMenu *[pathsize];
        for (int i = 0; i < pathcount; i++)
            newpath[i] = path[i];
        if (path) delete [] path;
        path = newpath;
    }
}

void WQMenuBoxPrivate::setMenu(QMenu *m)
{
    menu = m;
    pathcount = 0;
    currpath = -1;
    activeitem = -1;
    hovereditem = -1;
    openSubmenu(m);
}

void WQMenuBoxPrivate::openSubmenu(QMenu *m)
{
    if (m == nullptr) return;
    ensureRoom();
    path[++currpath] = m;
    pathcount = currpath + 1;
    activeitem = -1;
    hovereditem = -1;
}

QMenu *WQMenuBoxPrivate::current()
{
    if (currpath > -1) return path[currpath];
    return nullptr;
}

QMenu *WQMenuBoxPrivate::backward()
{
    if (canGoBackward()){
        activeitem = -1;
        hovereditem = -1;
        return path[--currpath];
    }
    return nullptr;
}

QMenu *WQMenuBoxPrivate::forward()
{
    if (canGoForward()){
        activeitem = -1;
        hovereditem = -1;
        return path[++currpath];
    }
    return nullptr;
}

bool WQMenuBoxPrivate::canGoBackward()
{
    return currpath > 0;
}

bool WQMenuBoxPrivate::canGoForward()
{
    return currpath < pathcount - 1;
}

/* WQActionItemDelegate */

WQActionItemDelegate::WQActionItemDelegate(QObject *parent): QStyledItemDelegate(parent)
{}

WQActionItemDelegate::~WQActionItemDelegate() = default;

void WQActionItemDelegate::paint(QPainter *painter,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    WQMenuBox *box = qobject_cast<WQMenuBox *>(option.styleObject);
    if (box){
        QAction *a = box->actionFromIndex(index.row());
        QStyle *style = box->style();

        int kind = box->itemKind(index.row());
        if (kind == WQ_MENU_ITEM_ACTION){
            painter->setRenderHint(QPainter::Antialiasing, true);
            // background
            if (box->activeAction() == a){
                QColor activeback = box->palette().highlight().color();
                activeback.setAlpha(128);
                painter->fillRect(option.rect, activeback);
            }
            else
                style->drawControl(QStyle::CE_ItemViewItem, &option, painter, box);
            // foreground
            int x = option.rect.left();
            QSize iconsz = box->iconSize();
            int checkw = checkWidthHint();
            // check-icon
            if (box->showCheckGutter()){
                QRect checkarea = option.rect;
                checkarea.setWidth(checkw);
                if (a->isCheckable())
                    drawCheckGlyph(painter, option, index, a, checkarea, false);
                x += checkw;

                QRect iconarea(x, option.rect.top(), iconsz.width(), option.rect.height());
                drawItemIcon(painter, option, index, a, iconarea, false);
                x += iconsz.width();
            }
            else{
                int iconcheckw = qMax(iconsz.width(), checkw);
                QRect iconcheckarea = option.rect;
                iconcheckarea.setWidth(iconcheckw);
                bool iconcheck = a->isCheckable() && !a->icon().isNull();
                if (a->isCheckable())
                    drawCheckGlyph(painter, option, index, a, iconcheckarea, iconcheck);
                if (!a->icon().isNull())
                    drawItemIcon(painter, option, index, a, iconcheckarea, iconcheck);
                x += iconcheckw;
            }
            // text
            QRect txtr = option.rect;
            txtr.setLeft(x);
            painter->setPen(option.palette.text().color());
            style->drawItemText(painter, txtr, Qt::AlignLeft | Qt::AlignVCenter,
                                box->palette(), a->isEnabled(), a->text());

            // shortcut
            QString shortcutstr = " " + QKeySequence::listToString(a->shortcuts());
            int shortcutw = option.fontMetrics.horizontalAdvance(shortcutstr);
            QRect shortcutr = option.rect;
            shortcutr.setLeft(option.rect.right() + 1 - arrowWidthHint() - shortcutw);
            shortcutr.setWidth(shortcutw);
            painter->setPen(option.palette.text().color());
            style->drawItemText(painter, shortcutr, Qt::AlignLeft | Qt::AlignVCenter,
                                box->palette(), a->isEnabled(), shortcutstr);

            // arrow
            if (a->menu()){
                QRect arrowarea(option.rect.right() + 1 - arrowWidthHint(),
                                option.rect.top(), arrowWidthHint(),
                                option.rect.height());
                drawArrowGlyph(painter, option, index, a, arrowarea);
            }
        }
        else if (kind == WQ_MENU_ITEM_SEPARATOR){
            painter->setRenderHint(QPainter::Antialiasing, false);
            int ym = option.rect.top() + option.rect.height()/2;
            // background
            painter->fillRect(option.rect, box->palette().base());
            // foreground
            painter->setPen(QPen(box->palette().dark().color(), 1));
            painter->drawLine(option.rect.left(), ym, option.rect.right(), ym);
            painter->setPen(QPen(box->palette().light().color(), 1));
            painter->drawLine(option.rect.left(), ym + 1, option.rect.right(), ym + 1);
        }
        else if (kind == WQ_MENU_ITEM_SECTION){
            painter->setRenderHint(QPainter::Antialiasing, false);
            // background
            painter->fillRect(option.rect, box->palette().button());
            // foreground
            int x = option.rect.left();
            QSize iconsz = box->iconSize();
            QRect iconr = option.rect;
            iconr.setWidth(iconsz.width());
            if (!a->icon().isNull()){
                if (box->isEnabled())
                    a->icon().paint(painter, iconr, Qt::AlignCenter, QIcon::Normal);
                else
                    a->icon().paint(painter, iconr, Qt::AlignCenter, QIcon::Disabled);
                x += iconsz.width();
            }
            QRect txtr = option.rect;
            txtr.setLeft(x);
            style->drawItemText(painter, txtr, Qt::AlignLeft | Qt::AlignVCenter,
                                box->palette(), a->isEnabled(), a->text());
            // painter->drawText(txtr, Qt::AlignLeft | Qt::AlignVCenter, a->text());
        }
        // case WQ_MENU_ITEM_WIDGET   :
    }
}

void WQActionItemDelegate::drawArrowGlyph(QPainter *painter,
    const QStyleOptionViewItem &option, const QModelIndex &index,
    QAction *action, const QRect &area)const
{
    QPoint p = QPoint(area.left() + (area.width() - 6)/2 + 6, area.center().y());
    painter->setPen(QPen(option.palette.text().color(), 1.5));
    painter->drawLine(p.x(), p.y(), p.x() - 5, p.y() - 5);
    painter->drawLine(p.x(), p.y(), p.x() - 5, p.y() + 5);
}

void WQActionItemDelegate::drawCheckGlyph(QPainter *painter,
    const QStyleOptionViewItem &option, const QModelIndex &index,
    QAction *action, const QRect &area, bool iconCheck)const
{
    int checkw = 21;
    int checkh = 21;
    int checkx = area.left() + (area.width() - checkw)/2;
    int checky = area.top() + (area.height() - checkh)/2;
    QColor checkback = option.palette.dark().color();
    checkback.setAlpha(128);

    if (action->isChecked()){ // draw only if checked
        // background
        painter->setBrush(checkback);
        painter->setPen(Qt::NoPen);

        if (iconCheck){ // check behind of icon
            painter->drawRoundedRect(QRectF(checkx, checky, checkw, checkh), 3, 3, Qt::AbsoluteSize);
        }
        else if (action->actionGroup() && (action->actionGroup()->isExclusive())){ // radio
            painter->drawEllipse(QRectF(checkx, checky, checkw, checkh));
            if (action->isEnabled())
                painter->setBrush(QColor("#404040"));
            else
                painter->setBrush(option.palette.dark().color());
            painter->drawEllipse(QRectF(checkx + (checkw - 7)/2,
                    checky + (checkh - 7)/2, 7, 7));
        }
        else { // check
            painter->drawRoundedRect(QRectF(checkx, checky, checkw, checkh), 3, 3, Qt::AbsoluteSize);
            if (action->isEnabled())
                painter->setPen(QPen(QColor("#404040"), 2)); //#
            else
                painter->setPen(QPen(option.palette.dark().color(), 2));
            QPoint checkv = QPoint(checkx + 8, checky + 15);
            painter->drawLine(checkv, QPoint(checkv.x() - 4, checkv.y() - 4));
            painter->drawLine(checkv, QPoint(checkv.x() + 8, checkv.y() - 8));
        }
    }
}

void WQActionItemDelegate::drawItemIcon(QPainter *painter,
    const QStyleOptionViewItem &option, const QModelIndex &index,
    QAction *action, const QRect &area, bool iconCheck)const
{
    if (!action->icon().isNull()){
        if (action->isEnabled())
            action->icon().paint(painter, area, Qt::AlignCenter, QIcon::Normal);
        else
            action->icon().paint(painter, area, Qt::AlignCenter, QIcon::Disabled);
    }
}

QSize WQActionItemDelegate::sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    WQMenuBox *box = qobject_cast<WQMenuBox *>(option.styleObject);
    if (box) {
        QAction *a = box->actionFromIndex(index.row());

        QSize iconsz = box->iconSize();
        int strw = option.fontMetrics.horizontalAdvance(a->text());
        int shortcutw = option.fontMetrics.horizontalAdvance(" " +
                        QKeySequence::listToString(a->shortcuts()));

        int w = 0;
        int h = 0;
        switch(box->itemKind(index.row())){
        case WQ_MENU_ITEM_ACTION    :
            h = qMax(actionHeightHint(), iconsz.height());
            if (box->showCheckGutter())
                w = checkWidthHint() + iconsz.width() + strw +
                    shortcutw + arrowWidthHint();
            else
                w = qMax(checkWidthHint(), iconsz.width()) + strw +
                    shortcutw + arrowWidthHint();
            break;
        case WQ_MENU_ITEM_SEPARATOR :
            h = separatorHeightHint();
            w = 0;
            break;
        case WQ_MENU_ITEM_SECTION   :
            h = qMax(sectionHeightHint(), iconsz.height());
            w = strw;
            if (!a->icon().isNull())
                w += iconsz.width();
            break;
        // case WQ_MENU_ITEM_WIDGET   :
        }

        return QSize{w, int(h*box->itemSizeFactor())};
    }
    return QStyledItemDelegate::sizeHint(option, index);
}

/* WQMenuFilter */


WQMenuFilter::WQMenuFilter(QObject *parent): QObject(parent)
{
    menubox = qobject_cast<WQMenuBox *>(parent);
}

bool WQMenuFilter::eventFilter(QObject *watched, QEvent *event)
{
    if ((event->type() == QEvent::ActionAdded) ||
        (event->type() == QEvent::ActionRemoved) ||
        (event->type() == QEvent::ActionChanged)){
        if (menubox) menubox->refreshMenu();
    }
    return QObject::eventFilter(watched, event);
}

/* WQMenuBox */

WQMenuBox::WQMenuBox(QWidget* parent):
    QListWidget(parent),
    d(new WQMenuBoxPrivate(this))
{
    d->separatorfont = this->font();

    connect(this, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(onItemActivated(QListWidgetItem*)));
    connect(this, SIGNAL(currentRowChanged(int)),
            this, SLOT(onCurrentRowChanged(int)));

    setItemDelegate(d->actiondelegate);

}

WQMenuBox::~WQMenuBox() = default;

void WQMenuBox::clear()
{
    removeMenuActions();
    d->setMenu(nullptr);
}

void WQMenuBox::setMenu(QMenu *m)
{
    if (d->menu != m){
        removeMenuActions();
        if (d->menu)
            removeAction(d->menu->menuAction());
        d->setMenu(m);
        if (d->menu)
            addAction(d->menu->menuAction());
        appendMenuActions();
    }
}

QMenu *WQMenuBox::backward()
{
    if (canGoBackward()){
        QAction *a = d->current()->menuAction();
        removeMenuActions();
        QMenu *m = d->backward();
        appendMenuActions();
        activateAction(m->actions().indexOf(a));
        return m;
    }
    return nullptr;
}

QMenu *WQMenuBox::forward()
{
    if (canGoForward()){
        removeMenuActions();
        QMenu *m = d->forward();
        appendMenuActions();
        activateAction(0);
        return m;
    }
    return nullptr;
}

QMenu *WQMenuBox::openSubmenu(int i)
{
    if ((i >= 0) && (i < count()) && (d->current())){
        QMenu *m = d->current();
        QAction *a = m->actions().at(i);
        if (a)
            if (a->menu()){
                qDebug("openning 1");
                QMenu *sm = a->menu();
                qDebug("openning 2");
                removeMenuActions();
                qDebug("openning 3");
                d->openSubmenu(sm);
                qDebug("openning 4");
                appendMenuActions();
                qDebug("openning 5");
                activateAction(0);
                qDebug("openning 6");
                return sm;
            }
    }
    return nullptr;
}

QMenu *WQMenuBox::openSubmenu(QAction *a) // only for this level
{
    QMenu *m = d->current();
    if (m){
        for (int i = 0; i < m->actions().count(); i++)
            if (a == m->actions().at(i))
                if (a->menu()){
                    QMenu *sm = a->menu();
                    removeMenuActions();
                    d->openSubmenu(sm);
                    appendMenuActions();
                    activateAction(0);
                    return sm;
                }
    }
    return nullptr;
}

int findAction_(QMenu *m, QAction *a, QMenu* &sm)
{
    if (m)
        for (int i = 0; i < m->actions().count(); i++){
            QAction *aat = m->actions().at(i);
            if (aat == a)
                return i;
            else if (aat->menu()){
                int k = findAction_(aat->menu(), a, sm);
                if (k != -1)return k;
            }
        }
    return -1;
}

int WQMenuBox::findAction(QAction *a, QMenu* &subm)
{
    return findAction_(menu(), a, subm);
}

bool WQMenuBox::gotoAction(QAction *action)
{
    QMenu *m = menu();
    if (m){
        QMenu *sm = nullptr;
        int i = -1;

        if (m->menuAction() == action)
            sm = m;
        else
            i = findAction_(m, action, sm);

        if (sm){
            removeMenuActions();
            d->openSubmenu(sm);
            appendMenuActions();
            activateAction(i);
            return true;
        }
    }
    return false;
}

QMenu *WQMenuBox::menu()
{
    return d->menu;
}

QMenu *WQMenuBox::submenu()
{
    return d->current();
}

QAction *WQMenuBox::activateAction(int i, bool simulate)
{
    QAction *a = actionFromIndex(i);
    int oldactive = d->activeitem;

    if (a && !a->isSeparator()){
        d->activeitem = i;
        if (!simulate)
            a->trigger(); //# test if has submenu

        if (oldactive != -1)
            update(indexFromItem(item(oldactive)));
        if (d->activeitem != -1)
            update(indexFromItem(item(d->activeitem)));

        emit actionActivated();
        return a;
    }
    return nullptr;
}

void WQMenuBox::popupSubmenu(int i, QPoint p)
{
    if (d->current()){
        QMenu *m = d->current();
        QAction *a = m->actions().at(i);
        if (a->menu()){
            QListWidgetItem *itm = item(i);
            QRect r = visualItemRect(itm);
            m = a->menu();
            m->popup(QPoint{r.right(), r.top()});
        }
    }
}

void WQMenuBox::hoverAction(int i)
{
    QAction *a = actionFromIndex(i);
    if (a){
        d->hovereditem = -1;
        a->hover();
        emit actionHovered();
    }
}

QAction *WQMenuBox::activeAction()
{
    return actionFromIndex(d->activeitem);
}

QAction *WQMenuBox::hoveredAction()
{
    return actionFromIndex(d->hovereditem);
}

bool WQMenuBox::canGoForward()
{
    return d->canGoForward();
}

bool WQMenuBox::canGoBackward()
{
    return d->canGoBackward();
}

QAction *WQMenuBox::actionFromIndex(int i)
{
    if ((i >= 0) && (i < count()))
        if (d->current())
            return d->current()->actions().at(i);
    return nullptr;
}

int WQMenuBox::indexFromAction(QAction *a)
{
    if (d->current())
        return d->current()->actions().indexOf(a);
    return -1;
}

QFont WQMenuBox::separatorFont()
{
    return d->separatorfont;
}

void WQMenuBox::setSeparatorFont(QFont f)
{
    if (d->separatorfont != f){
        d->separatorfont = f;
        update();
    }
}

bool WQMenuBox::showSeparatorTitle()
{
    return d->showseparatortitle;
}

void WQMenuBox::setShowSeparatorTitle(bool b)
{
    if (d->showseparatortitle != b){
        d->showseparatortitle = b;
        update();
    }
}

bool WQMenuBox::canPopupSubmenu()
{
    return d->canpopupsubmenu;
}

void WQMenuBox::setCanPopupSubmenu(bool b)
{
    d->canpopupsubmenu = b;
}

bool WQMenuBox::activateWithSel()
{
    return d->activatewithsel;
}

void WQMenuBox::setActivateWithSel(bool b)
{
    d->activatewithsel = b;
}

double WQMenuBox::itemSizeFactor()
{
    return d->itemsizefactor;
}

void WQMenuBox::setItemSizeFactor(double f)
{
    if (d->itemsizefactor != f){
        d->itemsizefactor = f;
        update();
    }
}

bool WQMenuBox::showCheckGutter()
{
    return d->showcheckgutter;
}

void WQMenuBox::setShowCheckGutter(bool b)
{
    if (d->showcheckgutter != b){
        d->showcheckgutter = b;
        update();
    }
}

// interaction

void WQMenuBox::onItemActivated(QListWidgetItem *item)
{
    activateAction(indexFromItem(item).row());
}

void WQMenuBox::onCurrentRowChanged(int currentRow)
{
    if (d->activatewithsel){
        // do nothing
    }
}

void WQMenuBox::keyPressEvent(QKeyEvent *event)
{
    if ((event == QKeySequence::Backspace) || (event->key() == Qt::Key_Backspace)){
        backward();
    }
    else if (event->key() == Qt::Key_Return){
        int i = currentRow();
        if (i != -1){
            // activateAction(i);
            openSubmenu(i);
        }
    }
    else if ((event->modifiers() == Qt::AltModifier)){
        if (event->key() == Qt::Key_Left)
            backward();
        else if (event->key() == Qt::Key_Right)
            forward();
    }
    QListWidget::keyPressEvent(event);
}

void WQMenuBox::mousePressEvent(QMouseEvent *event)
{
    if ((Qt::RightButton & event->buttons()) == Qt::RightButton){
        d->mouseitem = indexAt(event->pos()).row();
    }
    QListWidget::mousePressEvent(event);
}

void WQMenuBox::mouseMoveEvent(QMouseEvent *event)
{
    QListWidget::mouseMoveEvent(event);
    int i = indexAt(event->pos()).row();
    //if (d->hovereditem != i)
    //    hoverAction(i);
}

void WQMenuBox::mouseReleaseEvent(QMouseEvent *event)
{
    if ((Qt::RightButton & event->buttons()) == Qt::RightButton){
        QModelIndex index = indexAt(event->pos());
        if (d->mouseitem == index.row()){
            QListWidgetItem *item = itemFromIndex(index);
            QRect r = visualItemRect(item);
            if (event->pos().x() >= r.right() + 1 - d->arrowwidth){
                popupSubmenu(d->mouseitem, event->pos());
            }
        }
        d->mouseitem = -1;
    }
    QListWidget::mouseReleaseEvent(event);
}

void WQMenuBox::leaveEvent(QEvent *event)
{
    QListWidget::leaveEvent(event);
    d->hovereditem = -1;
}

// internals

void WQMenuBox::removeMenuActions()
{
    if (d->current()){
        qDebug("removing 1");
        d->current()->removeEventFilter(d->menufilter);
        qDebug("removing 2");
    }
}

void WQMenuBox::appendMenuActions()
{
    if (d->current()){
        d->current()->installEventFilter(d->menufilter);
        refreshMenu();
    }
}

void WQMenuBox::refreshMenu()
{
    if (d->current()){
        int actioncount = d->current()->actions().count();
        if (actioncount < count()){
            while (actioncount < count())
                delete takeItem(count() - 1);
        }
        else if (actioncount > count()){
            while (actioncount > count())
                addItem("");
        }
        for (int i = 0; i < count(); i++){
            QAction *a = d->current()->actions().at(i);
            QListWidgetItem *im = item(i);
            im->setFont(a->font());
            //#im->setHidden(a->isVisible());
            im->setStatusTip(a->statusTip());
            im->setText(a->text());
            im->setToolTip(a->toolTip());
            im->setWhatsThis(a->whatsThis());
            if (a->isSeparator()) im->setFlags(Qt::NoItemFlags);
            else if (a->isEnabled()) im->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            else im->setFlags(Qt::ItemIsSelectable);
        }
        update();
    }
}

int WQMenuBox::itemKind(int i)
{
   QAction *a = actionFromIndex(i);
   if (a){
       if (a->isSeparator()){
           if (!d->showseparatortitle)
               return WQ_MENU_ITEM_SEPARATOR;
           else if (a->text().isEmpty() && a->icon().isNull())
               return WQ_MENU_ITEM_SEPARATOR;
           else
               return WQ_MENU_ITEM_SECTION;
       }
       else
           return WQ_MENU_ITEM_ACTION;
   }
   return 0;
}
