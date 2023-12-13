// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqnotifier.h"

#include <QPointer>
#include <QtWidgets>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLabel>
#include <QToolButton>
#include <QStringList>
#include <QtWidgets/QHBoxLayout>
#include <memory.h>
#include <QStyle>
#include "qstylepainter.h"

class WQNotifierPrivate
{
public:
    WQNotifierPrivate(WQNotifier *qq);
    ~WQNotifierPrivate();
    WQNotifier *q;

    WQNotifier::NotificationKind kind;
    bool multiline;

    bool showing;

    QHBoxLayout *layout = nullptr;
    QVBoxLayout *txtlayout = nullptr;
    QVBoxLayout *clslayout = nullptr;
    QLabel *clsLbl = nullptr;

    QDialogButtonBox *buttons;
    QToolButton *closeBtn;
    QLabel *txtLbl;
    QLabel *iconLbl;

    void updateLayout();
    void releaseLayout();
};

/* QWNotifierPrivate */

WQNotifierPrivate::WQNotifierPrivate(WQNotifier *qq):
    q(qq)
{
    kind = WQNotifier::Success;
    multiline = false;
    showing = false;

    buttons = new QDialogButtonBox(q);
    buttons->setStandardButtons(0);
    buttons->setVisible(false);

    closeBtn = new QToolButton(q);
    txtLbl = new QLabel(q);
    txtLbl->setText("");
    iconLbl = new QLabel(q);
    iconLbl->setText("");

    updateLayout();
}

WQNotifierPrivate::~WQNotifierPrivate()
{
    releaseLayout();
    delete buttons;
    delete closeBtn;
    delete txtLbl;
    delete iconLbl;
}

void WQNotifierPrivate::releaseLayout()
{
    if (clsLbl){
        delete clsLbl;
        clsLbl = nullptr;
    }
    if (txtlayout){
        txtlayout->removeWidget(txtLbl);
        txtlayout->removeWidget(buttons);
        delete txtlayout;
        txtlayout = nullptr;
    }
    if (clslayout){
        // clsLbl already removed, so remove closeBtn only
        clslayout->removeWidget(closeBtn);
        delete clslayout;
        clslayout = nullptr;
    }
    if (layout){
        layout->removeWidget(iconLbl);
        delete layout;
        layout = nullptr;
    }
}

void WQNotifierPrivate::updateLayout()
{
    QSizePolicy sp;
    releaseLayout();
    if (multiline){
        layout = new QHBoxLayout(q);

        sp = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sp.setHorizontalStretch(0);
        sp.setVerticalStretch(0);
        sp.setHeightForWidth(iconLbl->sizePolicy().hasHeightForWidth());
        iconLbl->setSizePolicy(sp);
        iconLbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout->addWidget(iconLbl);

        txtlayout = new QVBoxLayout();

        txtLbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        txtlayout->addWidget(txtLbl);
        txtlayout->addWidget(buttons);

        layout->addLayout(txtlayout);

        clslayout = new QVBoxLayout();
        clslayout->addWidget(closeBtn, 0, Qt::AlignTop);
        //clsLbl = new QLabel(q);
        //clsLbl->setVisible(closeBtn->isVisible());
        //clslayout->addWidget(clsLbl);

        layout->addLayout(clslayout);
    }
    else{
        layout = new QHBoxLayout(q);

        sp = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sp.setHorizontalStretch(0);
        sp.setVerticalStretch(0);
        sp.setHeightForWidth(iconLbl->sizePolicy().hasHeightForWidth());
        iconLbl->setSizePolicy(sp);
        layout->addWidget(iconLbl);

        layout->addWidget(txtLbl);

        sp = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sp.setHorizontalStretch(0);
        sp.setVerticalStretch(0);
        sp.setHeightForWidth(buttons->sizePolicy().hasHeightForWidth());
        buttons->setSizePolicy(sp);
        layout->addWidget(buttons);

        layout->addWidget(closeBtn);
    }
}

/* WQNotifier */

WQNotifier::WQNotifier(QWidget* parent):
    QFrame(parent),
    d(new WQNotifierPrivate(this))
{
    QAction *action = new QAction(this);
    action->setText("");
    action->setToolTip("close");
    QStyleOptionFrame opt;
    opt.initFrom(this);
    QIcon closeIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-closetab-16.png");
    action->setIcon(closeIcon);
    // action->setIcon(this->style()->standardIcon(QStyle::SP_DialogCloseButton, &opt, this));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(closeAction(bool)));
    d->closeBtn->setDefaultAction(action);

    connect(d->buttons, SIGNAL(clicked(QAbstractButton*)), this, SIGNAL(clicked(QAbstractButton*)));
    connect(d->buttons, SIGNAL(acepted()), this, SIGNAL(acepted()));
    connect(d->buttons, SIGNAL(helpRequested()), this, SIGNAL(helpRequested()));
    connect(d->buttons, SIGNAL(rejected()), this, SIGNAL(rejected()));
}

WQNotifier::~WQNotifier() = default;

QString WQNotifier::text()
{
    return d->txtLbl->text();
}

void WQNotifier::setText(QString s)
{
    d->txtLbl->setText(s);
}

WQNotifier::NotificationKind WQNotifier::kind()
{
    return d->kind;
}

void WQNotifier::setKind(WQNotifier::NotificationKind k)
{
    if (k != d->kind){
        d->kind = k;
        updateKind();
    }
}

bool WQNotifier::enableCloseBtn()
{
    return d->closeBtn->isVisible();
}

void WQNotifier::setEnableCloseBtn(bool b)
{
    d->closeBtn->setVisible(b);
}

bool WQNotifier::multiline() const
{
    return d->multiline;
}

void WQNotifier::setMultiline(bool m)
{
    if (m != d->multiline){
        d->multiline = m;
        d->updateLayout();
    }
}

QDialogButtonBox::StandardButtons WQNotifier::buttons()
{
    return d->buttons->standardButtons();
}

void WQNotifier::setButtons(QDialogButtonBox::StandardButtons btns)
{
    d->buttons->setStandardButtons(btns);
}

void WQNotifier::showMessage(WQNotifier::NotificationKind k, QString s)
{
    setKind(k);
    setText(s);
    showMessage();
}

void WQNotifier::showMessage()
{
    setVisible(true);
    d->showing = true;
}

void WQNotifier::closeMessage()
{
    setVisible(false);
    d->showing = false;
}

QLabel *WQNotifier::iconLabel()
{
    return d->iconLbl;
}

QLabel *WQNotifier::textLabel()
{
    return d->txtLbl;
}

QDialogButtonBox *WQNotifier::dialogButtonBox()
{
    return d->buttons;
}

QToolButton *WQNotifier::closeButton()
{
    return d->closeBtn;
}

void WQNotifier::updateKind()
{
    QWidget *me = this;
    emit needsStyle(me);
    update();
}

void WQNotifier::updateLayout()
{
    d->updateLayout();
}

void WQNotifier::closeAction(bool checked)
{
    closeMessage();
}
