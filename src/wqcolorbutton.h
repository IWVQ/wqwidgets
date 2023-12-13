// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQCOLORBUTTON_H
#define WQCOLORBUTTON_H

#include <QPushButton>
#include <QDialog>
#include <QColorDialog>
#include <memory>

class Q_DECL_EXPORT WQCustomColorDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor)
    Q_PROPERTY(bool alphaBlend READ alphaBlend WRITE setAlphaBlend)
public:
    explicit WQCustomColorDialog(QWidget *parent = nullptr);
    ~WQCustomColorDialog() override;

    virtual QColor selectedColor() = 0;
    virtual void setSelectedColor(QColor c) = 0;
    virtual bool alphaBlend() = 0;
    virtual void setAlphaBlend(bool a) = 0;
};

class WQColorButtonPrivate;

class Q_DECL_EXPORT WQColorButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(bool alphaBlend READ alphaBlend WRITE setAlphaBlend)
    Q_PROPERTY(WQCustomColorDialog* customDialog READ customDialog WRITE setCustomDialog)
public:
    explicit WQColorButton(QWidget *parent = nullptr);
    ~WQColorButton() override;

    QColor color();
    void setColor(QColor c);
    bool alphaBlend();
    void setAlphaBlend(bool a);
    WQCustomColorDialog* customDialog();
    void setCustomDialog(WQCustomColorDialog *v);

    QColorDialog* colorDialog();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public slots:
    void chooseColor();
    void colorChosen();
Q_SIGNALS:
    void colorChanged(QColor c);
    void beforeShowDialog();
    void afterShowDialog();
protected:

    void initStyleOption(QStyleOptionButton *opt) const;
    void paintEvent(QPaintEvent *event) override;
    virtual void drawColorPattern(QPainter *painter, QStyleOptionButton *opt, QRect r);
private:
    std::unique_ptr<class WQColorButtonPrivate> const d;
};

#endif // NEWCOLORBUTTON_H
