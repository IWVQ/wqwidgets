// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQNOTIFIER_H
#define WQNOTIFIER_H

#include <QtWidgets>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLabel>
#include <QToolButton>
#include <QStringList>
#include <memory.h>

class WQNotifierPrivate;

class Q_DECL_EXPORT WQNotifier: public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(NotificationKind kind READ kind WRITE setKind)
    Q_PROPERTY(bool enableCloseBtn READ enableCloseBtn WRITE setEnableCloseBtn)
    Q_PROPERTY(bool multiline READ multiline WRITE setMultiline)
    Q_PROPERTY(QDialogButtonBox::StandardButtons buttons READ buttons WRITE setButtons)
public:
    enum NotificationKind{
        Success,
        Warning,
        Error,
        Information,
        Question
    };

    explicit WQNotifier(QWidget* parent = nullptr);
    ~WQNotifier() override;

    QString text();
    void setText(QString s);
    NotificationKind kind();
    void setKind(NotificationKind k);
    bool enableCloseBtn();
    void setEnableCloseBtn(bool b);
    bool multiline() const;
    void setMultiline(bool m);
    QDialogButtonBox::StandardButtons buttons();
    void setButtons(QDialogButtonBox::StandardButtons btns);

    void showMessage();
    void showMessage(NotificationKind k, QString s);
    void closeMessage();

    QLabel *iconLabel();
    QLabel *textLabel();
    QDialogButtonBox *dialogButtonBox();
    QToolButton *closeButton();

    void updateKind();
    void updateLayout();
public slots:
    void closeAction(bool checked = false);
Q_SIGNALS:
    void needsStyle(QWidget *me);
    void clicked(QAbstractButton *b);
    void accepted();
    void helpRequested();
    void rejected();
private:
    std::unique_ptr<class WQNotifierPrivate> const d;
};

#endif // WQNOTIFIER_H
