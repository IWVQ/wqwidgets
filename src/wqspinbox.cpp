// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqspinbox.h"

#include <QtGlobal>
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
#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractspinbox.h>
#include <QtWidgets/qspinbox.h>

QCursor makeSpinCursor()
{
    QPixmap p = QPixmap(":cursor_spin"); //#
    QCursor cur = QCursor(p, 4, 0);
    return cur;
}

class WQSpinBoxPrivate
{
public:
    WQSpinBoxPrivate(WQSpinBox *qq);
    WQSpinBox *q;
    QString caption;
    Qt::Alignment captionAlignment;
};

WQSpinBoxPrivate::WQSpinBoxPrivate(WQSpinBox *qq):q(qq)
{
    captionAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    caption = "";
}

WQSpinBox::WQSpinBox(QWidget *parent):
    QDoubleSpinBox(parent),
    d(new WQSpinBoxPrivate(this))
{
    lineEdit()->setStyleSheet("background-color:rgba(0,0,0,0)");
    setValue(0);
    setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    connect(lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(autosizeLineEdit(QString)));
    connect(this, SIGNAL(valueChanged(QString)), this, SLOT(autosizeLineEdit(QString)));
    setMouseTracking(true);

}

WQSpinBox::~WQSpinBox() = default;

QRect WQSpinBox::trackRect()
{
    QStyleOptionSpinBox opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_SpinBoxEditField;
    return style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField, this);
}

QPoint WQSpinBox::pointFromValue(double v)
{
    QRect r = trackRect();
    int y = (r.top() + r.bottom())/2;
    int delta;
    if (maximum() - minimum() <= 0){
        delta = 0;
    }
    else{
        delta = (v - minimum())*(r.width() - 1)/(maximum() - minimum());
    }
    int x = r.left() + delta;
    return QPoint{x,y};
}

double WQSpinBox::valueFromPoint(QPoint p)
{
    QRect r = trackRect();
    double res;
    if (r.width() - 1 <= 0){
        res = minimum();
    }
    else{
        int delta = p.x() - r.left();
        res = minimum() + delta*(maximum() - minimum())/(r.width() - 1);
    }
    if (res < minimum()){res = minimum();}
    if (res > maximum()){res = maximum();}
    return res;
}

QLineEdit *WQSpinBox::lineEdit() const
{
    return QDoubleSpinBox::lineEdit();
}

void WQSpinBox::setLineEdit(QLineEdit *edit)
{
    QDoubleSpinBox::setLineEdit(edit);
    connect(lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(autosizeLineEdit(QString)));
}

QString WQSpinBox::caption()
{
    return d->caption;
}

Qt::Alignment WQSpinBox::captionAlignment()
{
    return d->captionAlignment;
}

void WQSpinBox::setCaption(QString s)
{
    if (QString::compare(s, d->caption) != 0){
        d->caption = s;
        update();
    }
}

void WQSpinBox::setCaptionAlignment(Qt::Alignment al)
{
    if (al != d->captionAlignment){
        d->captionAlignment = al;
        update();
    }
}

void WQSpinBox::autosizeLineEdit(const QString &s)
{
    QString ss = lineEdit()->text();
    QRect r = trackRect();
    QFontMetrics fm = lineEdit()->fontMetrics();
    QSize sz = fm.boundingRect(ss).size();

    int x,y,w,h;

    w = qMin(sz.width() + 7, r.width());
    switch(alignment() & Qt::AlignHorizontal_Mask){
    case Qt::AlignLeft:     x = r.left();
        break;
    case Qt::AlignHCenter:  x = (r.left() + r.right() - w)/2;
        break;
    case Qt::AlignRight:    x = r.right() - w;
        break;
    default:                x = r.left();
    }

    h = qMin(sz.height(), r.height());
    switch (alignment() & Qt::AlignVertical_Mask) {
    case Qt::AlignTop:      y = r.top();
        break;
    case Qt::AlignVCenter:  y = (r.top() + r.bottom() - h)/2;
        break;
    case Qt::AlignBottom:   y = r.bottom() - h;
        break;
    default:                y = r.top();
    }

    lineEdit()->setGeometry(x, y, w, h);
}

void WQSpinBox::resizeEvent(QResizeEvent *event)
{
    QDoubleSpinBox::resizeEvent(event);
    autosizeLineEdit(lineEdit()->text());
}

void WQSpinBox::mouseMoveEvent(QMouseEvent *event)
{
    bool useSpinCursor = false;
    QRect r = trackRect();
    QPoint p = event->pos();
    if (r.contains(p)){
        useSpinCursor = true;
    }
    if ((Qt::LeftButton & event->buttons()) == Qt::LeftButton){
        setValue(valueFromPoint(p));
        update();
        useSpinCursor = true;
    }
    if (useSpinCursor){
        setCursor(makeSpinCursor()); // ! optimizar
    }
    else{
        setCursor(QCursor(Qt::ArrowCursor)); // ! optimizar
    }
    QDoubleSpinBox::mouseMoveEvent(event);
}

void WQSpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor)); // ! optimizar
    QDoubleSpinBox::mouseReleaseEvent(event);
}

void WQSpinBox::paintEvent(QPaintEvent *event)
{
    QDoubleSpinBox::paintEvent(event);
    QRect r = trackRect();
    QRect band = r;
    QPoint p = pointFromValue(value());
    band.setRight(p.x());
    r.setLeft(band.right()+1);

    QPainter painter(this);
    QColor cl = QColor::fromRgb(183, 195, 205);
    painter.fillRect(band, cl);

    if (band.width()>1){
        cl = QColor::fromRgb(121, 137, 151);
        painter.setPen(cl);
        painter.drawLine(QPoint{band.left(),band.bottom()},
                         QPoint{band.right(),band.bottom()});
        painter.drawLine(QPoint{band.right() - 1,band.bottom()},
                         QPoint{band.right() - 1,band.top()});
    }
    cl = QColor::fromRgb(64, 64, 64);
    painter.setPen(cl);
    painter.drawLine(QPoint{band.right(),band.bottom()},
                     QPoint{band.right(),band.top()});

    cl = Qt::white;
    painter.fillRect(r, cl);

    r.setLeft(band.left());
    r.adjust(2, 2, -2, -2);
    painter.setFont(font());
    painter.drawText(r, captionAlignment(), caption());
}

