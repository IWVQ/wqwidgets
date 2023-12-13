// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQSPINBOX_H
#define WQSPINBOX_H

#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractspinbox.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qlineedit.h>
#include <memory>

class WQSpinBoxPrivate;

class Q_DECL_EXPORT WQSpinBox: public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(QString caption READ caption WRITE setCaption)
    Q_PROPERTY(Qt::Alignment captionAlignment READ captionAlignment WRITE setCaptionAlignment)
public:
    explicit WQSpinBox(QWidget *parent = nullptr);
    ~WQSpinBox() override;

    QRect trackRect();
    QPoint pointFromValue(double v);
    double valueFromPoint(QPoint p);
    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *edit);

    QString caption();
    Qt::Alignment captionAlignment();
    void setCaption(QString s);
    void setCaptionAlignment(Qt::Alignment al);
public slots:
    void autosizeLineEdit(const QString &s);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    std::unique_ptr<class WQSpinBoxPrivate> const d;
};

#endif // WQSPINBOX_H
