#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "wqcolorbutton.h"
#include "wqfontbutton.h"
#include "wqledlabel.h"
#include "wqoptionbox.h"
#include "wqspinbox.h"
#include "wqslider.h"
#include "wqswitch.h"
#include "wqnotifier.h"
#include "wqmenubox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void drawNewTickMark(QPainter *painter, const int &v, Qt::Edge e, QPoint p, QRect sr, QRect gr);
    void onSliderValueChanged(int value);
    void onNotifierNeedsStyle(QWidget *me);
    void onSwitchToggled(bool checked);
private slots:
    void on_actionMyAction_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionSelectAll_triggered();

    void on_actionDefaultTheme_triggered();

    void on_actionDarkTheme_triggered();

    void on_actionLightTheme_triggered();

    void on_actionModernTheme_triggered();

private:
    Ui::MainWindow *ui;

    WQColorButton *colorbutton;
    WQFontButton *fontbutton;
    WQLedLabel *ledlabel;
    WQOptionBox *optionbox;
    WQSpinBox *spinbox;
    WQSlider *slider;
    WQSwitch *wswitch;
    WQNotifier *notifier;
    WQMenuBox *menubox;

    QActionGroup *actiongroup;
    QMenu *editmenu;
    QMenu *stringmenu;
    QMenu *thememenu;
};

#endif // MAINWINDOW_H
