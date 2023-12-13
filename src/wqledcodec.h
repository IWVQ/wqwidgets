// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#ifndef WQLEDCODEC_H
#define WQLEDCODEC_H

#include <QtWidgets>

#define LED_DOT1  0x00100000
#define LED_DOT2  0x00200000
#define LED_DOT3  0x00400000
#define LED_DOT4  0x00800000
#define LED_DOTS  0x00f00000

#define LED_O     0x00000000
#define LED_A     0x00000003 // LED_A1 + LED_A2
#define LED_A1    0x00000001
#define LED_A2    0x00000002
#define LED_B     0x00000004
#define LED_C     0x00000008
#define LED_D     0x00000030 // LED_D1 + LED_D2
#define LED_D1    0x00000010
#define LED_D2    0x00000020
#define LED_E     0x00000040
#define LED_F     0x00000080
#define LED_G     0x00000300 // LED_G1 + LED_G2
#define LED_G1    0x00000100
#define LED_G2    0x00000200
#define LED_H     0x00000400
#define LED_I     0x00000800
#define LED_J     0x00001000
#define LED_K     0x00002000
#define LED_L     0x00004000
#define LED_M     0x00008000
#define LED_DIGIT 0x0000ffff

class WQLeds: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int thickness READ thickness WRITE setThickness)
    Q_PROPERTY(int digitWidth READ digitWidth WRITE setDigitWidth)
    Q_PROPERTY(int digitHeight READ digitHeight WRITE setDigitHeight)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(Segments segments READ segments WRITE setSegments)
    Q_PROPERTY(bool rightToLeft READ rightToLeft WRITE setRightToLeft);
public:
    enum Segments{
        Segments7,
        Segments12,
        Segments16
    };

    WQLeds(QObject* parent = nullptr);
    ~WQLeds();

    virtual void parse(const QString &s);
    void render(QPainter *painter, int x, int y); // draws with the last colors
    virtual void render(QPainter *painter, int x, int y, QColor foreOn, QColor foreOff);
    void resize(int s);
    int width();
    int height();

    int count(){return bodysize;};
    int get(int i){ return body[i];};
    void put(int i, int led){ body[i] = led;};
protected:
    int charTo7SegmentLed(const QChar &c);
    int charTo12SegmentLed(const QChar &c);
    int charTo16SegmentLed(const QChar &c);

    void renderDotLed(QPainter *painter, int x, int led,
                      int d1, int d2, int d3, int d4);
    void render7SegmentLed(QPainter *painter, int x, int led,
                           int v1, int v2, int v3, int v4, int v5, int v6);
    void render12SegmentLed(QPainter *painter,
                            int x, int led, int v1, int v2, int v3, int v4, int v5, int v6);
    void render16SegmentLed(QPainter *painter,
                            int x, int led, int v1, int v2, int v3, int v4, int v5, int v6,
                            int vc1, int vc2, int vc3);
public: // configuration
    int thickness(){return dig_thickness;}
    int digitWidth(){return dig_width;}
    int digitHeight(){return dig_height;}
    int spacing(){return dig_spacing;}
    Segments segments(){return dig_segments;}
    bool rightToLeft(){return righttoleft; }
    QString text(){return str;}

    void setThickness(int t);
    void setDigitWidth(int w);
    void setDigitHeight(int h);
    void setSpacing(int s);
    void setSegments(Segments s);
    void setRightToLeft(bool b);
signals:
    void configured();
protected:
    QColor fore_on = Qt::red;
    QColor fore_off = Qt::lightGray;
private: // configuration
    int dig_thickness = 2;
    int dig_width = 10;
    int dig_height = 21;
    int dig_spacing = 2;
    Segments dig_segments = Segments7;
    bool righttoleft = true;
private:
    int* body = nullptr;
    int bodysize = 0;
    QString str;
};

#endif // WQLEDCODEC_H
