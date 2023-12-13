// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqcolorbutton.h"

#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include <QStyleOptionButton>
#include <qdrawutil.h>
#include "qstylepainter.h"

class WQColorButtonPrivate
{
public:
    WQColorButtonPrivate(WQColorButton *qq);
    WQColorButton *q;

    QColor color;
    bool alphaBlend;
    bool showing;
    WQCustomColorDialog *dialog;
    QPointer<QColorDialog> dialogPtr;
};

const Qt::WindowFlags colorDialogDefaultWindowFlags =
        Qt::Dialog |
        Qt::WindowTitleHint |
        Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint;

/* WQCustomColorDialog */

WQCustomColorDialog::WQCustomColorDialog(QWidget *parent):
    QDialog(parent, colorDialogDefaultWindowFlags)
{

}

WQCustomColorDialog::~WQCustomColorDialog() = default;

/* WQColorButtonPrivate */

WQColorButtonPrivate::WQColorButtonPrivate(WQColorButton *qq):q(qq)
{
    color = Qt::red;
    alphaBlend = false;
    dialog = nullptr;
    showing = false;
}

/* WQColorButton */

WQColorButton::WQColorButton(QWidget *parent):
    QPushButton(parent),
    d(new WQColorButtonPrivate(this))
{
    connect(this, SIGNAL(clicked()), this, SLOT(chooseColor()));
}

WQColorButton::~WQColorButton() = default;


QColor WQColorButton::color()
{
    return d->color;
}

void WQColorButton::setColor(QColor c)
{
    if (d->color != c){
        d->color = c;
        update();
        emit colorChanged(d->color);
    }
}

bool WQColorButton::alphaBlend()
{
    return d->alphaBlend;
}

void WQColorButton::setAlphaBlend(bool a)
{
    if (d->alphaBlend != a){
        d->alphaBlend = a;
        update();
    }
}

WQCustomColorDialog* WQColorButton::customDialog()
{
    return d->dialog;
}

void WQColorButton::setCustomDialog(WQCustomColorDialog *v)
{
    if (d->dialog == v)
        return;
    if (!d->dialog){
        disconnect(d->dialog, SIGNAL(accepted()), this, SLOT(colorChosen()));
    }

    if (!d->showing){
        d->dialog = v;
        connect(d->dialog, SIGNAL(accepted()), this, SLOT(colorChosen()));
    }
}

QColorDialog* WQColorButton::colorDialog()
{
    return d->dialogPtr.data();
}

QSize WQColorButton::sizeHint() const
{
    QStyleOptionButton opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(40, 15), this);
}

QSize WQColorButton::minimumSizeHint() const
{
    return sizeHint();
}

void WQColorButton::chooseColor()
{
    if (d->dialog){
        emit beforeShowDialog();
        d->dialog->show();
        d->dialog->raise();
        d->dialog->activateWindow();
    }
    else {
        QColorDialog* dialog = d->dialogPtr.data();
        if (!dialog){
            dialog = new QColorDialog(this);
            dialog->setCurrentColor(color());
            dialog->setOption(QColorDialog::ShowAlphaChannel, alphaBlend());
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, SIGNAL(accepted()), this, SLOT(colorChosen()));
            d->dialogPtr = dialog;
        }
        emit beforeShowDialog();
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
        d->showing = true;
    }
}

void WQColorButton::colorChosen()
{
    if (customDialog()){
        if (customDialog()->selectedColor().isValid())
            setColor(customDialog()->selectedColor());
    }
    else{
        QColorDialog *dialog = d->dialogPtr.data();
        if (!dialog){
            return;
        }
        if (dialog->selectedColor().isValid()){
            setColor(dialog->selectedColor());
        }
    }

    d->showing = false;
    emit afterShowDialog();
}

void WQColorButton::initStyleOption(QStyleOptionButton *opt) const
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

void WQColorButton::drawColorPattern(QPainter* painter, QStyleOptionButton *opt, QRect r)
{
    // int delta = QWidget::style()->pixelMetric(QStyle::PM_ButtonMargin, opt, this)/2;
    // r.adjust(delta, delta, -delta, -delta);

    QColor c = isEnabled()? color() : palette().color(backgroundRole());

    if (c.isValid()){
        if (c.alpha() < 255) {
            QPixmap pmp(16, 16);
            QPainter pattern(&pmp);
            pattern.fillRect(0, 0, 8, 8, Qt::lightGray);
            pattern.fillRect(8, 8, 8, 8, Qt::lightGray);
            pattern.fillRect(0, 8, 8, 8, Qt::white);
            pattern.fillRect(8, 0, 8, 8, Qt::white);
            pattern.end();
            painter->fillRect(r, QBrush(pmp));
        }
        painter->fillRect(r, c);
    }
}

void WQColorButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyle *style = QWidget::style();

    QStyleOptionButton opt;
    initStyleOption(&opt);
    style->drawControl(QStyle::CE_PushButtonBevel, &opt, &painter, this);

    QRect r = style->subElementRect(QStyle::SE_PushButtonContents, &opt, this);

    drawColorPattern(&painter, &opt, r);
}
