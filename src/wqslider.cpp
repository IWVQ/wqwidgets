// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqslider.h"

#include <QtGlobal>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include "qstyleoption.h"
#include "qstylepainter.h"
#include <qdrawutil.h>
#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractslider.h>
#include <QtWidgets/qslider.h>

class WQSliderPrivate
{
public:
    WQSliderPrivate(WQSlider *qq);
    WQSlider *q;

    bool ownerDrawTick;
    QColor tickColor;
    bool magnetic;
    QList<int> list;

    int customTickAt(int i);
    int addCustomTick(int p);
    void clearCustomTicks();
    int customTickCount();
};

//

WQSliderPrivate::WQSliderPrivate(WQSlider *qq):q(qq)
{
    magnetic = true;
    tickColor = Qt::lightGray;
    ownerDrawTick = false;
}

int WQSliderPrivate::customTickAt(int i)
{
    return list.at(i);
}

int WQSliderPrivate::addCustomTick(int p)
{
    int r = customTickCount();
    list.append(p);
    return r;
}

void WQSliderPrivate::clearCustomTicks()
{
    list.clear();
}

int WQSliderPrivate::customTickCount()
{
    return list.count();
}

// WQSlider

WQSlider::WQSlider(QWidget *parent):
    QSlider(parent),
    d(new WQSliderPrivate(this))
{
    connect(this, SIGNAL(sliderMoved(int)), this, SLOT(checkMagnetic(int)));
}

WQSlider::~WQSlider() = default;

int WQSlider::customTickAt(int index)
{
    return d->customTickAt(index);
}

int WQSlider::addCustomTick(int pos)
{
    return d->addCustomTick(pos);
    update();
}

int WQSlider::customTickCount()
{
    return d->customTickCount();
}

void WQSlider::clearCustomTicks()
{
    if (customTickCount() > 0){
        d->clearCustomTicks();
        update();
    }
}

bool WQSlider::magnetic()
{
    return d->magnetic;
}

void WQSlider::setMagnetic(bool v)
{
    if (d->magnetic != v){
        d->magnetic = v;
    }
}

QColor WQSlider::tickColor()
{
    return d->tickColor;
}

void WQSlider::setTickColor(QColor c)
{
    if (c != d->tickColor){
        d->tickColor = c;
        update();
    }
}
bool WQSlider::ownerDrawTick()
{
    return d->ownerDrawTick;
}

void WQSlider::setOwnerDrawTick(bool v)
{
    if (v != d->ownerDrawTick){
        d->ownerDrawTick = v;
        update();
    }
}

QPoint WQSlider::pixelFromValue(int v)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    int x, y;

    if (orientation() == Qt::Horizontal){
        // if (sr.width() >= gr.width()){ return sr.center(); }

        y = sr.center().y();
        x = QStyle::sliderPositionFromValue(minimum(), maximum(), v, gr.width() - sr.width(), opt.upsideDown);
        return QPoint{x + gr.left() + sr.width()/2, y};
    }
    else{
        // if (sr.height() >= gr.height()){ return sr.center(); }

        x = sr.center().x();
        y = QStyle::sliderPositionFromValue(minimum(), maximum(), v, gr.height() - sr.height(), opt.upsideDown);
        return QPoint{x, y + gr.top() + sr.height()/2};
    }
}

void WQSlider::checkMagnetic(int position)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    QPoint p;

    // check for interval ticks
    if ((tickPosition() != NoTicks) && (tickInterval() != 0)){
        for (int i = minimum(); i <= maximum(); i += tickInterval()) {
            p = pixelFromValue(i);
            if (sr.contains(p)) {
                setValue(i);
                return;
            }
        }
    }
    // check for custom ticks
    for (int i = 0; i < customTickCount(); i++) {
        p = pixelFromValue(customTickAt(i));
        if (sr.contains(p)) {
            setValue(customTickAt(i));
            return;
        }
    }
}

void WQSlider::drawTickMark(QPainter *painter, int v, Qt::Edge e, QPoint p, QRect sr, QRect gr)
{
    if (ownerDrawTick()){
        emit customDrawTickMark(painter, v, e, p, sr, gr);
    }
    else{
        switch(e){
        case Qt::LeftEdge: painter->drawLine(sr.left() - 2, p.y(), sr.left() - 6, p.y()); break;
        case Qt::TopEdge: painter->drawLine(p.x(), sr.top() - 2, p.x(), sr.top() - 6); break;
        case Qt::RightEdge: painter->drawLine(sr.right() + 2, p.y(), sr.right() + 6, p.y()); break;
        case Qt::BottomEdge: painter->drawLine(p.x(), sr.bottom() + 2, p.x(), sr.bottom() + 6); break;
        }
    }
}

void WQSlider::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    QPoint p;

    painter.setPen(tickColor());

    // draw interval ticks
    if ((tickPosition() != NoTicks) && (tickInterval() != 0)){
        for (int i = minimum(); i <= maximum(); i += tickInterval()) {
            p = pixelFromValue(i);
            if (orientation() == Qt::Horizontal){
                if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove){
                    drawTickMark(&painter, i, Qt::TopEdge, p, sr, gr);
                }
                if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow){
                    drawTickMark(&painter, i, Qt::BottomEdge, p, sr, gr);
                }
            }
            else{
                if (tickPosition() == TicksBothSides || tickPosition() == TicksLeft){
                    drawTickMark(&painter, i, Qt::LeftEdge, p, sr, gr);
                }
                if (tickPosition() == TicksBothSides || tickPosition() == TicksRight){
                    drawTickMark(&painter, i, Qt::RightEdge, p, sr, gr);
                }
            }
        }
    }

    // draw custom ticks
    for (int i = 0; i < customTickCount(); i++) {
        int v = customTickAt(i);
        p = pixelFromValue(v);
        if (orientation() == Qt::Horizontal){
            if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove){
                drawTickMark(&painter, v, Qt::TopEdge, p, sr, gr);
            }
            if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow){
                drawTickMark(&painter, v, Qt::BottomEdge, p, sr, gr);
            }
        }
        else{
            if (tickPosition() == TicksBothSides || tickPosition() == TicksLeft){
                drawTickMark(&painter, v, Qt::LeftEdge, p, sr, gr);
            }
            if (tickPosition() == TicksBothSides || tickPosition() == TicksRight){
                drawTickMark(&painter, v, Qt::RightEdge, p, sr, gr);
            }
        }
    }

    // draw the rest
    opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, opt);
}
