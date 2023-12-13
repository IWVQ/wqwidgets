// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQSLIDER_H
#define WQSLIDER_H

#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractslider.h>
#include <QtWidgets/qslider.h>
#include <memory>

class WQSliderPrivate;

// slider with custom ticks with string and magnetic handle

class Q_DECL_EXPORT WQSlider: public QSlider
{
    Q_OBJECT
    Q_PROPERTY(bool magnetic READ magnetic WRITE setMagnetic)
    Q_PROPERTY(QColor tickColor READ tickColor WRITE setTickColor)
    Q_PROPERTY(bool ownerDrawTick READ ownerDrawTick WRITE setOwnerDrawTick)
public:
    explicit WQSlider(QWidget *parent = nullptr);
    ~WQSlider() override;

    int customTickAt(int index);
    int addCustomTick(int pos);
    int customTickCount();
    void clearCustomTicks();

    bool magnetic();
    void setMagnetic(bool v);
    QColor tickColor();
    void setTickColor(QColor c);
    bool ownerDrawTick();
    void setOwnerDrawTick(bool v);

    QPoint pixelFromValue(int v);
public slots:
    void checkMagnetic(int position);
Q_SIGNALS:
    void customDrawTickMark(QPainter *painter, const int &v, Qt::Edge e, QPoint p, QRect sr, QRect gr);
protected:
    void paintEvent(QPaintEvent *event) override;
    virtual void drawTickMark(QPainter *painter, int v, Qt::Edge e, QPoint p, QRect sr, QRect gr);
private:
    std::unique_ptr<class WQSliderPrivate> const d;
};

#endif // WQSLIDER_H
