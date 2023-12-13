// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQOPTIONBOX_H
#define WQOPTIONBOX_H

#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qwidget.h>
#include <QPushButton>
#include <memory>

class WQOptionBoxPrivate;

class Q_DECL_EXPORT WQOptionBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint selection READ selection WRITE setSelection)
    Q_PROPERTY(bool multiselect READ multiselect WRITE setMultiselect)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(int optionCount READ optionCount WRITE setOptionCount)
public:
    explicit WQOptionBox(QWidget *parent = nullptr);
    ~WQOptionBox() override;

    static void boundOptions(uint &o, int l = 32);
    static int leftmostBit(uint o, int l = 32);

    void createOptions(int count, uint o = 0);
    int optionCount();
    void setOptionCount(int c);

    uint selection(); // for enum types
    void setSelection(uint sel);
    bool multiselect();
    void setMultiselect(bool m);
    int spacing();
    void setSpacing(int s);

    void setOptionCaption(uint o, QString s);
    QString optionCaption(uint o);
    QPushButton* optionButton(uint o);
    QButtonGroup* optionButtonGroup();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public slots:
    void buttonToggled(int id, bool checked);
Q_SIGNALS:
    void selectionChanged(uint s);
    void buttonClicked(QAbstractButton *button);
private:
    std::unique_ptr<class WQOptionBoxPrivate> const d;
};

#endif // WQOPTIONBOX_H
