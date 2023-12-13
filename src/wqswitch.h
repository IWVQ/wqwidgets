// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQSWITCH_H
#define WQSWITCH_H

#include <QtWidgets>
#include <QPushButton>
#include <QAbstractButton>

class WQSwitchPrivate;

class Q_DECL_EXPORT WQSwitch : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(SwitchAlign align READ align WRITE setAlign)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    enum SwitchAlign {
        SwitchAlignLeft = 0,
        SwitchAlignRight = 1
    };
    Q_ENUM(SwitchAlign)

    explicit WQSwitch(QWidget* parent = nullptr);
    ~WQSwitch() override;

    SwitchAlign align();
    void setAlign(SwitchAlign a);
    int spacing();
    void setSpacing(int s);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
protected:
    void initStyleOption(QStyleOptionButton *opt) const;

    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    virtual void drawSwitchPart(QPainter *painter, QRect r, QStyleOptionButton *opt);
    virtual void measureSwitchPart(QSize &sz) const;

private:
    std::unique_ptr<class WQSwitchPrivate> const d;
};

#endif // WQSWITCH_H
