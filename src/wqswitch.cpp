// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqswitch.h"

#include <QtGlobal>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include "qstyleoption.h"
#include "qstylepainter.h"
#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractbutton.h>

class WQSwitchPrivate
{
public:
    WQSwitchPrivate(WQSwitch *qq);
    WQSwitch *q;

    WQSwitch::SwitchAlign a;
    bool hovering;
    int spacing;
};

WQSwitchPrivate::WQSwitchPrivate(WQSwitch *qq):
    q(qq)
{
    hovering = true;
    spacing = 5;
    a = WQSwitch::SwitchAlignLeft;
}

//

WQSwitch::WQSwitch(QWidget *parent) :
    QAbstractButton(parent),
    d(new WQSwitchPrivate(this))
{
    setCheckable(true);
    setMouseTracking(true);
}

WQSwitch::~WQSwitch() = default;

QSize WQSwitch::sizeHint() const
{
    ensurePolished();

    QFontMetrics fm = fontMetrics();
    QStyleOptionButton opt;
    initStyleOption(&opt);

    QSize sz = style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, text()).size();
    if (!opt.icon.isNull()){
        sz = QSize(sz.width() + opt.iconSize.width() + 4, qMax(sz.height(), opt.iconSize.height()));
    }

    QSize spz;
    measureSwitchPart(spz);
    sz = QSize(sz.width() + spz.width(), qMax(sz.height(), spz.height()));

    return sz;
}

QSize WQSwitch::minimumSizeHint() const
{
    return sizeHint();
}

WQSwitch::SwitchAlign WQSwitch::align()
{
    return d->a;
}

void WQSwitch::setAlign(WQSwitch::SwitchAlign a)
{
    if (d->a != a){
        d->a = a;
        update();
    }
}

int WQSwitch::spacing()
{
    return d->spacing;
}

void WQSwitch::setSpacing(int s)
{
    if (s != d->spacing){
        d->spacing = s;
        update();
    }
}

void WQSwitch::initStyleOption(QStyleOptionButton *opt) const
{
    if (!opt)
        return;

    opt->initFrom(this);

    if (isDown())
        opt->state |= QStyle::State_Sunken;
    if (isChecked())
        opt->state |=  QStyle::State_On;
    else
        opt->state |= QStyle::State_Off;

    if (testAttribute(Qt::WA_Hover) && underMouse()) {
        if (d->hovering)
            opt->state |= QStyle::State_MouseOver;
        else
            opt->state &= ~QStyle::State_MouseOver;
    }

    opt->text = text();
    opt->icon = icon();
    opt->iconSize = iconSize();
}

void WQSwitch::mouseMoveEvent(QMouseEvent *event)
{
    if (testAttribute(Qt::WA_Hover)) {
        if (underMouse() != d->hovering) {
            update();
            d->hovering = underMouse();
        }
    }
    QAbstractButton::mouseMoveEvent(event);
}

void WQSwitch::drawSwitchPart(QPainter *painter, QRect r, QStyleOptionButton *opt)
{   // hovered pressed normal disabled | on off
    // State_Enabled State_Off State_On State_MouseOver State_Sunken
    QColor handleCl, borderCl, trackCl;

    if ((opt->state & QStyle::State_On)){ // on
        if (!(opt->state & QStyle::State_Enabled)) { // disabled
            handleCl = QColor(240, 240, 240, 255);
            borderCl = QColor(192, 192, 192, 255);
            trackCl = QColor(192, 192, 192, 255);
        }
        else if (opt->state & QStyle::State_Sunken) { // pressed
            handleCl = QColor(240, 240, 240, 255);
            borderCl = QColor(90, 90, 255, 255);
            trackCl = QColor(90, 90, 255, 255);
        }
        else if (opt->state & QStyle::State_MouseOver) { // hovered
            handleCl = QColor(240, 240, 240, 255);
            borderCl = QColor(128, 128, 255, 255);
            trackCl = QColor(128, 128, 255, 255);
        }
        else{ // normal
            handleCl = QColor(240, 240, 240, 255);
            borderCl = QColor(100, 100, 255, 255);
            trackCl = QColor(100, 100, 255, 255);
        }
    }
    else{ // state off
        if (!(opt->state & QStyle::State_Enabled)) { // disabled
            handleCl = QColor(192, 192, 192, 255);
            borderCl = QColor(192, 192, 192, 255);
            trackCl = QColor(240, 240, 240, 255);
        }
        else if (opt->state & QStyle::State_Sunken) { // pressed
            handleCl = QColor(64, 64, 64, 255);
            borderCl = QColor(64, 64, 64, 255);
            trackCl = QColor(200, 200, 200, 255);
        }
        else if (opt->state & QStyle::State_MouseOver) { // hovered
            handleCl = QColor(64, 64, 64, 255);
            borderCl = QColor(64, 64, 64, 255);
            trackCl = QColor(220, 220, 220, 255);
        }
        else{ // normal
            handleCl = QColor(64, 64, 64, 255);
            borderCl = QColor(64, 64, 64, 255);
            trackCl = QColor(240, 240, 240, 255);
        }
    }
    int x;
    int y = (r.top() + r.bottom() - 20) / 2;

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(borderCl);
    pen.setWidthF(1.5);
    painter->setPen(pen);
    painter->setBrush(trackCl);

    if (align() == WQSwitch::SwitchAlignLeft){
        x = r.left() + 1;
        painter->drawRoundedRect(x, y, 43, 20, 10, 10, Qt::AbsoluteSize);
    }
    else{
        x = r.left() + 2;
        painter->drawRoundedRect(x, y, 43, 20, 10, 10, Qt::AbsoluteSize);
    }
    painter->setPen(Qt::transparent);
    painter->setBrush(handleCl);
    if (opt->state & QStyle::State_On){
        painter->drawEllipse(QPoint{x + 43 - 10, y + 10}, 8, 8);
    }
    else{
        painter->drawEllipse(QPoint{x + 10, y + 10}, 7, 7);
    }
}

void WQSwitch::measureSwitchPart(QSize &sz) const
{
    sz = QSize(44 + 2, 23);
}

void WQSwitch::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);

    QRect r = opt.rect;
    QSize sz;
    measureSwitchPart(sz);

    QRect tr = r;

    if (align() == SwitchAlignLeft){
        r.setRight(r.left() + sz.width());
        tr.setLeft(r.right() + spacing());
    }
    else{
        r.setLeft(r.right() - sz.width());
        tr.setRight(r.left() - spacing());
    }

    // icon
    if (!icon().isNull()){
        int y = (tr.top() + tr.bottom() - iconSize().height()) / 2;
        if (align() == SwitchAlignLeft){
            icon().paint(&painter, tr.left(), y, iconSize().width(), iconSize().height());
            tr.setLeft(tr.left() + iconSize().width() + 3);
        }
        else{
            icon().paint(&painter, tr.right() - iconSize().width(), y, iconSize().width(), iconSize().height());
            tr.setRight(tr.right() - iconSize().width() - 3);
        }
    }

    // text

    painter.setFont(font());

    Qt::Alignment al = Qt::AlignVCenter;
    if (align() == SwitchAlignLeft){
        al |= Qt::AlignLeft;
    }
    else{
        al |= Qt::AlignRight;
    }

    painter.drawText(tr, al, text());

    // switch part

    drawSwitchPart(&painter, r, &opt);
}
