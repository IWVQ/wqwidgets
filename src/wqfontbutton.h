// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQFONTBUTTON_H
#define WQFONTBUTTON_H

#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qwidget.h>
#include <QPushButton>
#include <QDialog>
#include <QFontDialog>
#include <memory>

class Q_DECL_EXPORT WQCustomFontDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
public:
    explicit WQCustomFontDialog(QWidget *parent = nullptr);
    ~WQCustomFontDialog() override;

    virtual QFont selectedFont() = 0;
    virtual void setSelectedFont(QFont c) = 0;
};

class WQFontButtonPrivate;

class Q_DECL_EXPORT WQFontButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(Qt::Alignment fontAlignment READ fontAlignment WRITE setFontAlignment)
    Q_PROPERTY(QFont fontValue READ fontValue WRITE setFontValue)
    Q_PROPERTY(bool wysiwyg READ wysiwyg WRITE setWYSIWYG)
    Q_PROPERTY(FontButtonOptions options READ options WRITE setOptions)
    Q_PROPERTY(WQCustomFontDialog* customDialog READ customDialog WRITE setCustomDialog)
public:
    enum FontButtonOption{
        ShowPlain = 0x0,
        ShowStyle = 0x1,
        ShowSize = 0x2,
        ShowIcon = 0x4
    };
    Q_DECLARE_FLAGS(FontButtonOptions, FontButtonOption)
    Q_FLAG(FontButtonOptions)

    // vqfboShowStyle, vqfboShowSize, vqfboAddItalic, vqfboShowImage, vqfboAutoSizeFont

    explicit WQFontButton(QWidget *parent = nullptr);
    ~WQFontButton() override;

    QFont fontValue();
    void setFontValue(QFont f);
    WQCustomFontDialog* customDialog();
    void setCustomDialog(WQCustomFontDialog *v);
    FontButtonOptions options();
    void setOptions(FontButtonOptions o);
    bool wysiwyg();
    void setWYSIWYG(bool w);
    Qt::Alignment fontAlignment();
    void setFontAlignment(Qt::Alignment a);

    QFontDialog* fontDialog();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public slots:
    void chooseFontValue();
    void fontValueChosen();
Q_SIGNALS:
    void fontValueChanged(QFont f);
    void beforeShowDialog();
    void afterShowDialog();
protected:
    void initStyleOption(QStyleOptionButton *opt) const;
    void paintEvent(QPaintEvent *event) override;
    virtual void drawFontValue(QPainter *painter, QStyleOptionButton *opt, QRect r);
private:
    std::unique_ptr<class WQFontButtonPrivate> const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WQFontButton::FontButtonOptions)

#endif // WQFONTBUTTON_H
