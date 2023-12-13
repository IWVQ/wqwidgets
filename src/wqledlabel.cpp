// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqledlabel.h"


class WQLedLabelPrivate
{
public:
    WQLedLabelPrivate(WQLedLabel *qq);
    ~WQLedLabelPrivate();
    void changeCodec(WQLeds *newcodec);
    void parse(const QString &s);

    WQLedLabel *q;
    WQLeds *codec;
    QColor background = Qt::transparent;
    QColor foreDis = Qt::gray;
    QColor foreOn = Qt::red;
    QColor foreOff = Qt::lightGray;
    Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignTop;
};

WQLedLabelPrivate::WQLedLabelPrivate(WQLedLabel *qq): q(qq)
{
    codec = new WQLeds();
}

WQLedLabelPrivate::~WQLedLabelPrivate()
{
    if (codec) delete codec;
}

void WQLedLabelPrivate::changeCodec(WQLeds *newcodec)
{
    if (!newcodec) return;
    QString s = codec->text();
    delete codec;
    codec = newcodec;
    codec->parse(s);
}

void WQLedLabelPrivate::parse(const QString &s)
{
    codec->parse(s);
}

/* WQLedLabel */

WQLedLabel::WQLedLabel(QWidget* parent):
    QWidget(parent),
    d(new WQLedLabelPrivate(this))
{
    connect(d->codec, SIGNAL(configured()), this, SLOT(onConfigured()));
}

WQLedLabel::~WQLedLabel() = default;

WQLeds *WQLedLabel::codec()
{
    return d->codec;
}

QString WQLedLabel::text()
{
    return d->codec->text();
}

Qt::Alignment WQLedLabel::alignment()
{
    return d->alignment;
}

QColor WQLedLabel::background()
{
    return d->background;
}

void WQLedLabel::setCodec(WQLeds *l)
{
    if (l == d->codec) return;

    disconnect(d->codec, SIGNAL(configured()), this, SLOT(onConfigured()));
    d->changeCodec(l);
    connect(d->codec, SIGNAL(configured()), this, SLOT(onConfigured()));
    update();
}

void WQLedLabel::setText(const QString &s)
{
    if (d->codec->text() != s){
        d->parse(s);
        update();
    }
}

void WQLedLabel::setAlignment(Qt::Alignment a)
{
    if (d->alignment != a){
        d->alignment = a;
        update();
    }
}

void WQLedLabel::setBackground(QColor c)
{
    if (d->background != c){
        d->background = c;
        update();
    }
}

QColor WQLedLabel::foregroundOn()
{
    return d->foreOn;
}

void WQLedLabel::setForegroundOn(QColor c)
{
    if (d->foreOn != c){
        d->foreOn = c;
        update();
    }
}

QColor WQLedLabel::foregroundOff()
{
    return d->foreOff;
}

void WQLedLabel::setForegroundOff(QColor c)
{
    if (d->foreOff != c){
        d->foreOff = c;
        update();
    }
}

QColor WQLedLabel::foregroundDis()
{
    return d->foreDis;
}

void WQLedLabel::setForegroundDis(QColor c)
{
    if (d->foreDis != c){
        d->foreDis = c;
        update();
    }
}

void WQLedLabel::showValue(int v)
{
    showValue(QString::number(v));
}

void WQLedLabel::showValue(const QString &s)
{
    d->parse(s);
    repaint();
}

void WQLedLabel::onConfigured()
{
    update();
}

void WQLedLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect r = rect();
    painter.fillRect(r, d->background);
    int x, y;

    if (Qt::AlignLeft & d->alignment) x = 0;
    else if (Qt::AlignRight & d->alignment) x = width() - d->codec->width();
    else if (Qt::AlignHCenter & d->alignment) x = (width() - d->codec->width())/2;
    else x = 0;

    if (Qt::AlignTop & d->alignment) y = 0;
    else if (Qt::AlignBottom & d->alignment) y = height() - d->codec->height();
    else if (Qt::AlignVCenter & d->alignment) y = (height() - d->codec->height())/2;
    else y = 0;

    if (isEnabled())
        d->codec->render(&painter, x, y, d->foreOn, d->foreOff);
    else
        d->codec->render(&painter, x, y, d->foreDis, d->foreOff);
}
