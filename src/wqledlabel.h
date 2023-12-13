// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQLEDLABEL_H
#define WQLEDLABEL_H

#include <QtWidgets>
#include "wqledcodec.h"

class WQLedLabelPrivate;

class Q_DECL_EXPORT WQLedLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor foregroundOn READ foregroundOn WRITE setForegroundOn)
    Q_PROPERTY(QColor foregroundOff READ foregroundOff WRITE setForegroundOff)
    Q_PROPERTY(QColor foregroundDis READ foregroundDis WRITE setForegroundDis)
public:
    explicit WQLedLabel(QWidget* parent = nullptr);
    ~WQLedLabel() override;

    void showValue(int v);
    void showValue(const QString &s);

    WQLeds *codec();
    QString text();
    Qt::Alignment alignment();
    QColor background();

    void setCodec(WQLeds *l);
    void setText(const QString &s);
    void setAlignment(Qt::Alignment a);
    void setBackground(QColor c);

    QColor foregroundOn();
    void setForegroundOn(QColor c);
    QColor foregroundOff();
    void setForegroundOff(QColor c);
    QColor foregroundDis(); // disabled foreground color
    void setForegroundDis(QColor c);

protected slots:
    void onConfigured();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    std::unique_ptr<class WQLedLabelPrivate> const d;
};

#endif // WQLEDLABEL_H
