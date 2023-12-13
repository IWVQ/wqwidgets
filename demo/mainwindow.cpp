#include "mainwindow.h"
#include "ui_mainwindow.h"

enum AColorModel{
    RGB = 0x1,
    CMY = 0x2,
    HSL = 0x4,
    HSV = 0x8};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    colorbutton = new WQColorButton(ui->widget_widgets);
    colorbutton->setObjectName(QString::fromUtf8("colorbutton"));
    colorbutton->setGeometry(QRect(10, 10, 60, 23));
    colorbutton->setVisible(true);

    fontbutton = new WQFontButton(ui->widget_widgets);
    fontbutton->setObjectName(QString::fromUtf8("fontbutton"));
    fontbutton->setGeometry(QRect(10, 45, 200, 27));
    fontbutton->setVisible(true);

    optionbox = new WQOptionBox(ui->widget_widgets);
    optionbox->setObjectName(QString::fromUtf8("optionbox"));
    optionbox->setGeometry(QRect(10, 85, 200, 21));
    optionbox->setVisible(true);
    optionbox->createOptions(4);
    optionbox->setMultiselect(false);
    optionbox->setOptionCaption(RGB, "RGB");
    optionbox->setOptionCaption(CMY, "CMY");
    optionbox->setOptionCaption(HSL, "HSL");
    optionbox->setOptionCaption(HSV, "HSV");
    optionbox->setSelection(RGB);

    ledlabel = new WQLedLabel(ui->widget_widgets);
    ledlabel->setObjectName(QString::fromUtf8("ledlabel"));
    ledlabel->setGeometry(QRect(10, 123, 250, 25));
    ledlabel->setVisible(true);
    ledlabel->codec()->resize(10);
    ledlabel->setText("123.45E-57");

    slider = new WQSlider(ui->widget_widgets);
    slider->setObjectName(QString::fromUtf8("slider"));
    slider->setGeometry(QRect(10, 162, 200, 30));
    slider->setVisible(true);
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(1000);
    slider->setMagnetic(true);
    slider->addCustomTick(550);
    slider->setTickInterval(100);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->
            setStyleSheet("QSlider::sub-page:Horizontal { background-color: #52bdfa; }"
                          "QSlider::add-page:Horizontal { background-color: #c2c2c2; }"
                          "QSlider::groove:Horizontal { background: transparent; height:1px; }"
                          "QSlider::handle:Horizontal { width:5px; background:#5276fa; margin: -5px 0px -5px 0px; }");
    slider->setOwnerDrawTick(true);

    connect(slider, SIGNAL(customDrawTickMark(QPainter*, int, Qt::Edge, QPoint, QRect, QRect)),
            this, SLOT(drawNewTickMark(QPainter*, int, Qt::Edge, QPoint, QRect, QRect)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));

    spinbox = new WQSpinBox(ui->widget_widgets);
    spinbox->setObjectName(QString::fromUtf8("spinbox"));
    spinbox->setGeometry(QRect(10, 205, 200, 23));
    spinbox->setVisible(true);
    spinbox->setMaximum(100);
    spinbox->setSuffix("cm");
    spinbox->setCaption("Thickness:");

    wswitch = new WQSwitch(ui->widget_widgets);
    wswitch->setObjectName(QString::fromUtf8("wswitch"));
    wswitch->setGeometry(QRect(10, 240, 200, 23));
    wswitch->setVisible(true);
    wswitch->setText("Switch");
    wswitch->setAlign(WQSwitch::SwitchAlignRight);
    connect(wswitch, SIGNAL(toggled(bool)), this, SLOT(onSwitchToggled(bool)));


    notifier = new WQNotifier(centralWidget());
    notifier->setObjectName(QString::fromUtf8("notifier"));
    notifier->setGeometry(QRect(0, 0, 200, 40));
    notifier->setVisible(false);
    notifier->setMaximumHeight(40);
    connect(notifier, SIGNAL(needsStyle(QWidget*)), this, SLOT(onNotifierNeedsStyle(QWidget*)));
    ui->verticalLayout->insertWidget(0, notifier);
    notifier->showMessage(WQNotifier::Information, "this is some message");



    actiongroup = new QActionGroup(this);
    actiongroup->addAction(ui->actionDefaultTheme);
    actiongroup->addAction(ui->actionLightTheme);
    actiongroup->addAction(ui->actionDarkTheme);
    actiongroup->addAction(ui->actionModernTheme);
    actiongroup->setExclusive(true);

    menubox = new WQMenuBox(ui->widget_menu);
    menubox->setObjectName(QString::fromUtf8("menubox"));
    menubox->setGeometry(QRect(0, 0, 230, 300));
    menubox->setVisible(true);

    editmenu = new QMenu(this);
    editmenu->addAction(ui->actionUndo);
    editmenu->addAction(ui->actionRedo);
    editmenu->addSeparator();
    editmenu->addAction(ui->actionCopy);
    editmenu->addAction(ui->actionPaste);
    editmenu->addAction(ui->actionCut);
    editmenu->addAction(ui->actionSelectAll);
    editmenu->addSeparator();

    stringmenu = new QMenu(this);
    stringmenu->setTitle("String operations");
    stringmenu->setToolTip("String operations");
    stringmenu->setWhatsThis("my string operations");
    stringmenu->setStatusTip("my string operations");

    stringmenu->addAction(ui->actionUPPERCASE);
    stringmenu->addAction(ui->actionlowercase);
    stringmenu->addAction(ui->actionProperCase);
    editmenu->addAction(stringmenu->menuAction());

    thememenu = new QMenu(this);
    thememenu->setTitle("Theme");
    thememenu->setToolTip("Theme");
    thememenu->setWhatsThis("my theme");
    thememenu->setStatusTip("my theme");

    thememenu->addAction(ui->actionDefaultTheme);
    thememenu->addAction(ui->actionLightTheme);
    thememenu->addAction(ui->actionDarkTheme);
    thememenu->addAction(ui->actionModernTheme);
    editmenu->addAction(thememenu->menuAction());

    editmenu->addSeparator();
    editmenu->addAction(ui->actionFind);
    editmenu->addAction(ui->actionReplace);
    editmenu->addAction(ui->actionGoto);
    editmenu->addAction(ui->actionMyAction);

    menubox->setIconSize(QSize{20, 20});

    menubox->setMenu(editmenu);
    menubox->refreshMenu();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawNewTickMark(QPainter *painter, const int &v, Qt::Edge e, QPoint p, QRect sr, QRect gr)
{
    switch(e){
    case Qt::LeftEdge: painter->drawLine(sr.left() - 2, p.y(), sr.left() - 6, p.y()); break;
    case Qt::TopEdge: painter->drawLine(p.x(), gr.top() - 1, p.x(), gr.top() - 6); break;
    case Qt::RightEdge: painter->drawLine(sr.right() + 2, p.y(), sr.right() + 6, p.y()); break;
    case Qt::BottomEdge: painter->drawLine(p.x(), gr.bottom() + 1, p.x(), gr.bottom() + 6); break;
    }
}

void MainWindow::onSliderValueChanged(int value)
{
    double f = value*9675.0/100;
    ledlabel->showValue(QString::number(f));
}

void MainWindow::onNotifierNeedsStyle(QWidget *me)
{
    me->setStyleSheet(QString::fromUtf8("  WQNotifier{border: 2px solid; \n"
                                        "  border-color: green;\n"
                                        "  border-radius: 6px;\n}"));
}

void MainWindow::onSwitchToggled(bool checked)
{
    if (checked)
        wswitch->setText("On");
    else
        wswitch->setText("Off");
}

void MainWindow::on_actionMyAction_triggered()
{
    notifier->showMessage(WQNotifier::Information, "on_actionMyAction_triggered()");
}


void MainWindow::on_actionUndo_triggered()
{
    notifier->showMessage(WQNotifier::Information, "on_actionUndo_triggered()");
}


void MainWindow::on_actionRedo_triggered()
{
    notifier->showMessage(WQNotifier::Information, "on_actionRedo_triggered()");
}


void MainWindow::on_actionSelectAll_triggered()
{
    notifier->showMessage(WQNotifier::Information, "on_actionSelectAll_triggered()");
}


void MainWindow::on_actionDefaultTheme_triggered()
{
    notifier->showMessage(WQNotifier::Information, "Default theme selected");
}


void MainWindow::on_actionDarkTheme_triggered()
{
    notifier->showMessage(WQNotifier::Information, "Dark theme selected");
}


void MainWindow::on_actionLightTheme_triggered()
{
    notifier->showMessage(WQNotifier::Information, "Light theme selected");
}


void MainWindow::on_actionModernTheme_triggered()
{
    notifier->showMessage(WQNotifier::Information, "Modern"
                                                   " theme selected");
}

