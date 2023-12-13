// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqoptionbox.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include <QStyleOptionButton>
#include <qdrawutil.h>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QButtonGroup>
#include "qstylepainter.h"
#include <QtCore/qbitarray.h>

class WQOptionBoxPrivate
{
public:
    WQOptionBoxPrivate(WQOptionBox *qq);
    ~WQOptionBoxPrivate();
    WQOptionBox *q;

    uint options;
    int count;
    bool modifying;

    QHBoxLayout *layout;
    QButtonGroup *group;

    QPushButton *buttonAt(int i);
    void clear();
    void recreate(int c, uint o);
    void update(uint o);
};

/* WQOptionBoxPrivate */

WQOptionBoxPrivate::WQOptionBoxPrivate(WQOptionBox *qq):
    q(qq)
{
    modifying = false;
    options = 0;
    count = 0;
    layout = new QHBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    group = new QButtonGroup();

}

WQOptionBoxPrivate::~WQOptionBoxPrivate()
{
    clear();
    delete group;
    delete layout;
}

void WQOptionBoxPrivate::clear()
{
    for (int i = 0; i < count; i++){
        QPushButton *button = dynamic_cast<QPushButton *>(group->button(i));
        group->removeButton(button);
        delete button;
    }
    count = 0;
}

QPushButton *WQOptionBoxPrivate::buttonAt(int i)
{
    return dynamic_cast<QPushButton *>(layout->itemAt(i)->widget());
}

void WQOptionBoxPrivate::recreate(int c, uint o)
{
    QPushButton *button;

    clear();
    count = c;

    for (int i = 0; i < c; i++){
        button = new QPushButton(q);

        QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        policy.setHorizontalStretch(0);
        policy.setVerticalStretch(0);
        policy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
        button->setSizePolicy(policy);
        button->setCheckable(true);

        layout->addWidget(button);
        group->addButton(button, i);
    }

    update(o);
}

void WQOptionBoxPrivate::update(uint o)
{
    options = o;
    for (int i = 0; i < count; i++){
        QPushButton *button = buttonAt(i);
        button->setChecked((options & (1 << i)));
    }
}

/* WQOptionBox */

WQOptionBox::WQOptionBox(QWidget *parent):
    QWidget(parent),
    d(new WQOptionBoxPrivate(this))
{
    connect(d->group, SIGNAL(buttonClicked(QAbstractButton*)), this, SIGNAL(buttonClicked(QAbstractButton*)));
    connect(d->group, SIGNAL(buttonToggled(int, bool)), this, SLOT(buttonToggled(int, bool)));
}

WQOptionBox::~WQOptionBox() = default;

void WQOptionBox::boundOptions(uint &o, int l)
{
    if (l == 0)
        o = 0;
    else
        o &= (0xffffffff >> (32 - l));
}

int WQOptionBox::leftmostBit(uint o, int l)
{
    for(int i = l - 1; i >= 0; i--){
        if (o & (1 << i)){
            return i;
        }
    }
    return -1;
}

void WQOptionBox::createOptions(int count, uint o)
{
    if (count > 32)
        count = 32;
    if (count < 0)
        count = 0;
    boundOptions(o, count);
    if ((o != d->options) || (count != d->count)){
        d->modifying = true;
        d->recreate(count, o);
        emit selectionChanged(o);
        d->modifying = false;
    }
}

int WQOptionBox::optionCount()
{
    return d->count;
}

void WQOptionBox::setOptionCount(int c)
{
    if (d->count != c){
        createOptions(c);
    }
}

uint WQOptionBox::selection()
{
    return d->options;
}

void WQOptionBox::setSelection(uint sel)
{
    boundOptions(sel, d->count);
    if (sel != d->options){
        d->modifying = true;
        d->update(sel);
        emit selectionChanged(sel);
        d->modifying = false;
    }
}

bool WQOptionBox::multiselect()
{
    return !(d->group->exclusive());
}

void WQOptionBox::setMultiselect(bool m)
{
    d->group->setExclusive(!m);
}

int WQOptionBox::spacing()
{
    return d->layout->spacing();
}

void WQOptionBox::setSpacing(int s)
{
    d->layout->setSpacing(s);
}

void WQOptionBox::setOptionCaption(uint o, QString s)
{
    for (int i = 0; i < d->count; i++){
        if (o & (1 << i)){
            d->group->button(i)->setText(s);
        }
    }
}

QString WQOptionBox::optionCaption(uint o)
{
    return optionButton(o)->text();
}

QPushButton* WQOptionBox::optionButton(uint o)
{
    int i = leftmostBit(o, d->count);
    return dynamic_cast<QPushButton *>(d->group->button(i));
}

QButtonGroup* WQOptionBox::optionButtonGroup()
{
    return d->group;
}

QSize WQOptionBox::sizeHint() const
{
    return QSize{200, 23};
}

QSize WQOptionBox::minimumSizeHint() const
{
    return sizeHint();
}

void WQOptionBox::buttonToggled(int id, bool checked)
{
    if (d->modifying)
        return;

    uint o;
    if (multiselect()){
        if (checked)
            o = selection() | (1 << id);
        else
            o = selection() & ~(1 << id);
    }
    else{
        if (checked)
            o = 1 << id;
        else
            o = 0;
    }
    d->options = o;
}
