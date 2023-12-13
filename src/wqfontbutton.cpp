// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqfontbutton.h"

#include <QApplication>
#include <QClipboard>
#include <QFontDialog>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include <QStyleOptionButton>
#include <QFontDatabase>
#include <qdrawutil.h>
#include "qstylepainter.h"

class WQFontButtonPrivate
{
public:
    WQFontButtonPrivate(WQFontButton *qq);
    WQFontButton *q;

    QFont font;
    bool wysiwyg;
    bool showing;
    Qt::Alignment alignment;
    WQFontButton::FontButtonOptions options;
    QString str;

    QIcon tticon;
    QIcon bmpicon;

    WQCustomFontDialog *dialog;
    QPointer<QFontDialog> dialogPtr;

    void update();
};

const Qt::WindowFlags fontDialogDefaultWindowFlags =
        Qt::Dialog |
        Qt::WindowTitleHint |
        Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint;

/* WQCustomFontDialog */

WQCustomFontDialog::WQCustomFontDialog(QWidget *parent):
    QDialog(parent, fontDialogDefaultWindowFlags)
{

}

WQCustomFontDialog::~WQCustomFontDialog() = default;

/* WQFontButtonPrivate */

WQFontButtonPrivate::WQFontButtonPrivate(WQFontButton *qq):
    q(qq),
    tticon(QStringLiteral(":/qt-project.org/styles/commonstyle/images/fonttruetype-16.png")),
    bmpicon(QStringLiteral(":/qt-project.org/styles/commonstyle/images/fontbitmap-16.png"))
{
    wysiwyg = true;
    options = WQFontButton::ShowSize;
    dialog = nullptr;
    showing = false;
    alignment = Qt::AlignCenter;
    str = font.family();
}

void WQFontButtonPrivate::update()
{
    str = font.family();

    if (options & WQFontButton::ShowStyle){
        QFontDatabase base;
        QString sts = base.styleString(font);
        if (QString::compare(sts, "") != 0)
            str = str + " " + sts;
    };
}

/* WQFontButton */


WQFontButton::WQFontButton(QWidget *parent):
    QPushButton(parent),
    d(new WQFontButtonPrivate(this))
{
    connect(this, SIGNAL(clicked()), this, SLOT(chooseFontValue()));
}

WQFontButton::~WQFontButton()= default;

QFont WQFontButton::fontValue()
{
    return d->font;
}

void WQFontButton::setFontValue(QFont f)
{
    //if (d->font != f){
        d->font = f;
        d->update();
        update();
        emit fontValueChanged(d->font);
    //}
}

WQCustomFontDialog* WQFontButton::customDialog()
{
    return d->dialog;
}

void WQFontButton::setCustomDialog(WQCustomFontDialog *v)
{
    if (d->dialog == v)
        return;
    if (!d->dialog){
        disconnect(d->dialog, SIGNAL(accepted()), this, SLOT(fontValueChosen()));
    }

    if (!d->showing){
        d->dialog = v;
        connect(d->dialog, SIGNAL(accepted()), this, SLOT(fontValueChosen()));
    }
}

WQFontButton::FontButtonOptions WQFontButton::options()
{
    return d->options;
}

void WQFontButton::setOptions(FontButtonOptions o)
{
    if (d->options != o){
        d->options = o;
        d->update();
        update();
    }
}

bool WQFontButton::wysiwyg()
{
    return d->wysiwyg;
}

void WQFontButton::setWYSIWYG(bool w)
{
    if (d->wysiwyg != w){
        d->wysiwyg = w;
        update();
    }
}

Qt::Alignment WQFontButton::fontAlignment()
{
    return d->alignment;
}

void WQFontButton::setFontAlignment(Qt::Alignment a)
{
    if (d->alignment != a){
        d->alignment = a;
        update();
    }
}

QFontDialog* WQFontButton::fontDialog()
{
    return d->dialogPtr.data();
}


QSize WQFontButton::sizeHint() const
{
    QStyleOptionButton opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(70, 15), this);
}

QSize WQFontButton::minimumSizeHint() const
{
    return sizeHint();
}

void WQFontButton::chooseFontValue()
{
    if (d->dialog){
        emit beforeShowDialog();
        d->dialog->show();
        d->dialog->raise();
        d->dialog->activateWindow();
    }
    else {
        QFontDialog* dialog = d->dialogPtr.data();
        if (!dialog){
            dialog = new QFontDialog(this);
            dialog->setCurrentFont(fontValue());
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, SIGNAL(accepted()), this, SLOT(fontValueChosen()));
            d->dialogPtr = dialog;
        }
        emit beforeShowDialog();
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
        d->showing = true;
    }
}

void WQFontButton::fontValueChosen()
{
    if (customDialog()){
        setFontValue(customDialog()->selectedFont());
    }
    else{
        QFontDialog *dialog = d->dialogPtr.data();
        if (!dialog){
            return;
        }
        setFontValue(dialog->currentFont());
    }

    d->showing = false;
    emit afterShowDialog();
}

void WQFontButton::initStyleOption(QStyleOptionButton *opt) const
{
    if (!opt)
        return;

    opt->initFrom(this);

    if (isDown())
        opt->state |= QStyle::State_Sunken;
    else
        opt->state |= QStyle::State_Raised;

    if (isDefault())
        opt->features =  QStyleOptionButton::None | QStyleOptionButton::DefaultButton;
    else
        opt->features = QStyleOptionButton::None;

    opt->text.clear();
    opt->icon = QIcon();
    opt->iconSize = iconSize();
}

void WQFontButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyle *style = QWidget::style();

    QStyleOptionButton opt;
    initStyleOption(&opt);
    style->drawControl(QStyle::CE_PushButtonBevel, &opt, &painter, this);

    QRect r = style->subElementRect(QStyle::SE_PushButtonContents, &opt, this);

    drawFontValue(&painter, &opt, r);
}

void WQFontButton::drawFontValue(QPainter *painter, QStyleOptionButton *opt, QRect r)
{
    // metrics

    QFontMetrics fm(fontValue());
    QSize sz = fm.boundingRect(d->str).size();

    int delta = 0;
    if (ShowSize & options())
        delta = 17;

    QSize iconSz(0, 0);
    if (ShowIcon & options()){
        iconSz = iconSize();
    }

    int valueW = iconSz.width() + 4 + sz.width();
    int valueH = qMax(sz.height(), iconSz.height());

    int x;

    switch(fontAlignment() & Qt::AlignHorizontal_Mask){
    case Qt::AlignLeft: x = r.left(); break;
    case Qt::AlignHCenter: x = (r.left() + r.right() - delta - valueW)/2; break;
    case Qt::AlignRight: x = r.right() - delta - valueW; break;
    default: x = r.left();
    }
    int y = (r.top() + r.bottom() - valueH)/2;

    // draw icon
    if (ShowIcon & options()){
        QFontDatabase base;
        if (base.isSmoothlyScalable(fontValue().family())) {
            d->tticon.paint(painter, x, y, iconSize().width(), iconSize().height());
        }
        else{
            d->bmpicon.paint(painter, x, y, iconSize().width(), iconSize().height());
        }
        x += iconSz.width() + 4;
    }

    // draw string
    Qt::Alignment al = Qt::AlignVCenter | Qt::AlignLeft;
    if (wysiwyg())
        painter->setFont(fontValue());
    else
        painter->setFont(font());
    painter->drawText(x, r.top(), r.right() - x, r.height(), al, d->str);

    // draw size
    if (ShowSize & options()){
        al = Qt::AlignVCenter | Qt::AlignRight;
        painter->setFont(font());
        painter->drawText(r, al, QString::number(fontValue().pointSize()));
    }
}
