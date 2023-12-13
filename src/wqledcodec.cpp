// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqledcodec.h"

/* leds */

const int NUMBER_LEDS[10] = { // 0123456789
    LED_A | LED_B | LED_C | LED_D | LED_E | LED_F,
    LED_B | LED_C,
    LED_A | LED_B | LED_D | LED_E | LED_G,
    LED_A | LED_B | LED_C | LED_D | LED_G,
    LED_B | LED_C | LED_F | LED_G,
    LED_A | LED_C | LED_D | LED_F | LED_G,
    LED_A | LED_C | LED_D | LED_E | LED_F | LED_G,
    LED_A | LED_B | LED_C,
    LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G,
    LED_A | LED_B | LED_C | LED_D | LED_F | LED_G
};

const int LITERAL_7S_LEDS_U[7] = { // ABCDEF P
    LED_A | LED_B | LED_C | LED_E | LED_F | LED_G,
    LED_C | LED_D | LED_E | LED_F | LED_G,
    LED_A | LED_D | LED_E | LED_F,
    LED_B | LED_C | LED_D | LED_E | LED_G,
    LED_A | LED_D | LED_E | LED_F | LED_G,
    LED_A | LED_E | LED_F | LED_G,
    LED_A | LED_B | LED_E | LED_F | LED_G
};

const int LITERAL_7S_LEDS_L[7] = { // abcdef p
    LED_A | LED_B | LED_C | LED_D | LED_E | LED_G,
    LED_C | LED_D | LED_E | LED_F | LED_G,
    LED_D | LED_E | LED_G,
    LED_B | LED_C | LED_D | LED_E | LED_G,
    LED_A | LED_B | LED_D | LED_E | LED_F | LED_G,
    LED_A | LED_E | LED_F | LED_G,
    LED_A | LED_B | LED_E | LED_F | LED_G
};

const int LITERAL_12S_LEDS_U[26] = { // ABCDEFGHIJKLMNOPQRSTUVWXYZ
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C,
    LED_A1 | LED_A2 | LED_I  | LED_B  | LED_G2 | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_E  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_I  | LED_B  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C ,
    LED_A1 | LED_A2 | LED_I  | LED_L  | LED_D1 | LED_D2 ,
    LED_B  | LED_C  | LED_D1 | LED_D2 ,
    LED_A2 | LED_F  | LED_I  | LED_G1 | LED_E  | LED_L  | LED_D2,
    LED_F  | LED_E  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_I  | LED_B  | LED_E  | LED_C,
    LED_A1 | LED_F  | LED_I  | LED_B  | LED_E  | LED_L  | LED_C  | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_E  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_L  | LED_D2 ,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_I  | LED_L ,
    LED_F  | LED_B  | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_B  | LED_G2 | LED_E  | LED_L  | LED_D1,
    LED_F  | LED_B  | LED_E  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_I  | LED_G1 | LED_G2 | LED_L  | LED_C,
    LED_F  | LED_B  | LED_G1 | LED_G2 | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2
};

const int LITERAL_12S_LEDS_L[26] = { // abcdefghijklmnopqrstuvwxyz
    LED_A1 | LED_A2 | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_A2 | LED_I  | LED_G1 | LED_G2 | LED_L,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_G1 | LED_G2 | LED_E  | LED_C ,
    LED_I  | LED_L ,
    LED_I  | LED_L  | LED_D1,
    LED_G2 | LED_E  | LED_L  | LED_D1 | LED_D2,
    LED_A1 | LED_I  | LED_L  | LED_D1 | LED_D2,
    LED_G1 | LED_G2 | LED_E  | LED_L  | LED_C ,
    LED_G1 | LED_E  | LED_L  | LED_C  | LED_D2 ,
    LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_C,
    LED_G1 | LED_G2 | LED_E,
    LED_A2 | LED_I  | LED_G2 | LED_C  | LED_D2,
    LED_I  | LED_G1 | LED_G2 | LED_L  | LED_D2,
    LED_E  | LED_C  | LED_D1 | LED_D2 ,
    LED_G2 | LED_E  | LED_L  | LED_D1 ,
    LED_E  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_I  | LED_G1 | LED_G2 | LED_L  | LED_C,
    LED_I  | LED_B  | LED_G2 | LED_C  | LED_D2 ,
    LED_A1 | LED_I  | LED_L  | LED_D2
};

const int LITERAL_16S_LEDS_U[26] = { // ABCDEFGHIJKLMNOPQRSTUVWXYZ
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C,
    LED_A1 | LED_A2 | LED_I  | LED_B  | LED_G2 | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_E  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_I  | LED_B  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C ,
    LED_A1 | LED_A2 | LED_I  | LED_L  | LED_D1 | LED_D2 ,
    LED_B  | LED_C  | LED_D1 | LED_D2 ,
    LED_F  | LED_J  | LED_G1 | LED_E  | LED_M,
    LED_F  | LED_E  | LED_D1 | LED_D2,
    LED_H  | LED_J  | LED_F  | LED_B  | LED_E  | LED_C,
    LED_H  | LED_F  | LED_B  | LED_E  | LED_M  | LED_C,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_E  | LED_M  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_M,
    LED_A1 | LED_A2 | LED_F  | LED_G1 | LED_G2 | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_I  | LED_L ,
    LED_F  | LED_B  | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_E  | LED_K  | LED_J,
    LED_F  | LED_B  | LED_E  | LED_C  | LED_K  | LED_M,
    LED_H  | LED_J  | LED_K  | LED_M,
    LED_H  | LED_J  | LED_L,
    LED_A1 | LED_A2 | LED_K  | LED_J  | LED_D1 | LED_D2
};

const int LITERAL_16S_LEDS_L[26] = { // abcdefghijklmnopqrstuvwxyz
    LED_A1 | LED_A2 | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_B  | LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E  | LED_D1 | LED_D2,
    LED_A2 | LED_I  | LED_G1 | LED_G2 | LED_L,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_C  | LED_D1 | LED_D2,
    LED_F  | LED_G1 | LED_G2 | LED_E  | LED_C ,
    LED_I  | LED_L ,
    LED_I  | LED_L  | LED_D1,
    LED_G2 | LED_E  | LED_L  | LED_D1 | LED_D2,
    LED_A1 | LED_I  | LED_L  | LED_D1 | LED_D2,
    LED_G1 | LED_G2 | LED_E  | LED_L  | LED_C ,
    LED_G1 | LED_E  | LED_L  | LED_C  | LED_D2 ,
    LED_G1 | LED_G2 | LED_E  | LED_C  | LED_D1 | LED_D2,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_E,
    LED_A1 | LED_A2 | LED_F  | LED_B  | LED_G1 | LED_G2 | LED_C,
    LED_G1 | LED_G2 | LED_E,
    LED_A2 | LED_I  | LED_G2 | LED_C  | LED_D2,
    LED_I  | LED_G1 | LED_G2 | LED_L  | LED_D2,
    LED_E  | LED_C  | LED_D1 | LED_D2 ,
    LED_G2 | LED_E  | LED_L  | LED_D1 ,
    LED_E  | LED_L  | LED_C  | LED_D1 | LED_D2,
    LED_H  | LED_J  | LED_K  | LED_M,
    LED_I  | LED_B  | LED_G2 | LED_C  | LED_D2 ,
    LED_A1 | LED_A2 | LED_K  | LED_J  | LED_D1 | LED_D2 | LED_C  | LED_G2
};

/* WQLeds */

WQLeds::WQLeds(QObject* parent): QObject(parent)
{
    resize(5);
}

WQLeds::~WQLeds()
{
    if (body) delete [] body;
}

void WQLeds::parse(const QString &s)
{
    if (righttoleft){
        int i = 0;
        int l = bodysize - qMin(s.length(), bodysize);
        while (i < l){
            body[i] = LED_O;
            i++;
        }
        switch(dig_segments){
        case Segments7:
            while (i < bodysize){
                body[i] = charTo7SegmentLed(s[i - l]);
                i++;
            }
            break;
        case Segments12:
            while (i < bodysize){
                body[i] = charTo12SegmentLed(s[i - l]);
                i++;
            }
            break;
        case Segments16:
            while (i < bodysize){
                body[i] = charTo16SegmentLed(s[i - l]);
                i++;
            }
            break;
        }
    }
    else{
        int l = qMin(s.length(), bodysize);
        int i = 0;
        switch(dig_segments){
        case Segments7:
            while (i < l){
                body[i] = charTo7SegmentLed(s[i]);
                i++;
            }
            break;
        case Segments12:
            while (i < l){
                body[i] = charTo12SegmentLed(s[i]);
                i++;
            }
            break;
        case Segments16:
            while (i < l){
                body[i] = charTo16SegmentLed(s[i]);
                i++;
            }
            break;
        }
        while (i < bodysize){
            body[i] = LED_O;
            i++;
        }
    }
    str = s;
}

void WQLeds::render(QPainter *painter, int x, int y)
{
    render(painter, x, y, fore_on, fore_off);
}

void WQLeds::render(QPainter *painter, int x, int y, QColor foreOn, QColor foreOff)
{
    fore_on = foreOn;
    fore_off = foreOff;

    if (!painter) return;

    int l = bodysize;
    int i = 0;

    int v1 = y;
    int v2 = y + dig_thickness;
    int v3 = y + (dig_height - dig_thickness) / 2;
    int v4 = v3 + dig_thickness;
    int v5 = y + dig_height - dig_thickness;
    int v6 = y + dig_height;
    
    int d = (dig_height - dig_thickness) / 3;
    int d1 = y;
    int d2 = y + d;
    int d3 = v5 - d;
    int d4 = v5;

    if (dig_segments == Segments7){
        painter->setRenderHint(QPainter::Antialiasing, false);
        while (i < l){
            if (body[i] & LED_DOTS){
                renderDotLed(painter, x, body[i], d1, d2, d3, d4);
                x += dig_thickness + dig_spacing;
            }
            else {
                render7SegmentLed(painter, x, body[i], v1, v2, v3, v4, v5, v6);
                x += dig_width + dig_spacing;
            }
            i++;
        }
    }
    else if (dig_segments == Segments12){
        painter->setRenderHint(QPainter::Antialiasing, false);
        while (i < l){
            if (body[i] & LED_DOTS){
                renderDotLed(painter, x, body[i], d1, d2, d3, d4);
                x += dig_thickness + dig_spacing;
            }
            else {
                render12SegmentLed(painter, x, body[i], v1, v2, v3, v4, v5, v6);
                x += dig_width + dig_spacing;
            }
            i++;
        }
    }
    else { // Segments16
        painter->setRenderHint(QPainter::Antialiasing, true);
        int vc1 = v1 + dig_thickness/2;
        int vc2 = (v1 + v6) / 2;
        int vc3 = v6 - dig_thickness/2;
        while (i < l){
            if (body[i] & LED_DOTS){
                renderDotLed(painter, x, body[i], d1, d2, d3, d4);
                x += dig_thickness + dig_spacing;
            }
            else {
                render16SegmentLed(painter, x, body[i], v1, v2, v3, v4, v5, v6, vc1, vc2, vc3);
                x += dig_width + dig_spacing;
            }
            i++;
        }
    }
}

#define FILL_LED_OFF(l, t, r, b) painter->fillRect(l, t, r - l, b - t, fore_off)
#define FILL_LED_ON(l, t, r, b) painter->fillRect(l, t, r - l, b - t, fore_on)

void WQLeds::renderDotLed(QPainter *painter, int x, int led,
    int d1, int d2, int d3, int d4)
{
    // 4
    if (led & LED_DOT4)
        painter->fillRect(x, d1, dig_thickness, dig_thickness, fore_on);
    //else
    //    painter->fillRect(x, d1, dig_thickness, dig_thickness, fore_off);
    // 3
    if (led & LED_DOT3)
        painter->fillRect(x, d2, dig_thickness, dig_thickness, fore_on);
    //else
    //    painter->fillRect(x, d2, dig_thickness, dig_thickness, fore_off);
    // 2
    if (led & LED_DOT2)
        painter->fillRect(x, d3, dig_thickness, dig_thickness, fore_on);
    //else
    //    painter->fillRect(x, d3, dig_thickness, dig_thickness, fore_off);
    //1
    if (led & LED_DOT1)
        painter->fillRect(x, d4, dig_thickness, dig_thickness, fore_on);
    //else
    //    painter->fillRect(x, d4, dig_thickness, dig_thickness, fore_off);
}

void WQLeds::render7SegmentLed(QPainter *painter, int x, int led,
    int v1, int v2, int v3, int v4, int v5, int v6)
{
    int h1 = x;
    int h2 = x + dig_thickness;
    // int h3 = x + (dig_width - dig_thickness) / 2;
    // int h4 = h3 + dig_thickness;
    int h5 = x + dig_width - dig_thickness;
    int h6 = x + dig_width;
    
    // leds off
    FILL_LED_OFF(h1, v1, h6, v2); // A
    FILL_LED_OFF(h5, v1, h6, v4); // B
    FILL_LED_OFF(h5, v3, h6, v6); // C
    FILL_LED_OFF(h1, v5, h6, v6); // D
    FILL_LED_OFF(h1, v3, h2, v6); // E
    FILL_LED_OFF(h1, v1, h2, v4); // F
    FILL_LED_OFF(h1, v3, h6, v4); // G
    // leds on
    if (led & LED_A) FILL_LED_ON(h1, v1, h6, v2); // A
    if (led & LED_B) FILL_LED_ON(h5, v1, h6, v4); // B
    if (led & LED_C) FILL_LED_ON(h5, v3, h6, v6); // C
    if (led & LED_D) FILL_LED_ON(h1, v5, h6, v6); // D
    if (led & LED_E) FILL_LED_ON(h1, v3, h2, v6); // E
    if (led & LED_F) FILL_LED_ON(h1, v1, h2, v4); // F
    if (led & LED_G) FILL_LED_ON(h1, v3, h6, v4); // G
}

void WQLeds::render12SegmentLed(QPainter *painter,
    int x, int led, int v1, int v2, int v3, int v4, int v5, int v6)
{
    int h1 = x;
    int h2 = x + dig_thickness;
    int h3 = x + (dig_width - dig_thickness) / 2;
    int h4 = h3 + dig_thickness;
    int h5 = x + dig_width - dig_thickness;
    int h6 = x + dig_width;
    
    // leds off
    FILL_LED_OFF(h1, v1, h4, v2); // A1
    FILL_LED_OFF(h3, v1, h6, v2); // A2
    FILL_LED_OFF(h5, v1, h6, v4); // B
    FILL_LED_OFF(h5, v3, h6, v6); // C
    FILL_LED_OFF(h1, v5, h4, v6); // D1
    FILL_LED_OFF(h3, v5, h6, v6); // D2
    FILL_LED_OFF(h1, v3, h2, v6); // E
    FILL_LED_OFF(h1, v1, h2, v4); // F
    FILL_LED_OFF(h1, v3, h4, v4); // G1
    FILL_LED_OFF(h3, v3, h6, v4); // G2
    FILL_LED_OFF(h3, v1, h4, v4); // I
    FILL_LED_OFF(h3, v3, h4, v6); // L
    // leds on
    if (led & LED_A) FILL_LED_ON(h1, v1, h4, v2); // A1
    if (led & LED_A) FILL_LED_ON(h3, v1, h6, v2); // A2
    if (led & LED_B) FILL_LED_ON(h5, v1, h6, v4); // B
    if (led & LED_C) FILL_LED_ON(h5, v3, h6, v6); // C
    if (led & LED_D) FILL_LED_ON(h1, v5, h4, v6); // D1
    if (led & LED_D) FILL_LED_ON(h3, v5, h6, v6); // D2
    if (led & LED_E) FILL_LED_ON(h1, v3, h2, v6); // E
    if (led & LED_F) FILL_LED_ON(h1, v1, h2, v4); // F
    if (led & LED_G) FILL_LED_ON(h1, v3, h4, v4); // G1
    if (led & LED_G) FILL_LED_ON(h3, v3, h6, v4); // G2
    if (led & LED_I) FILL_LED_OFF(h3, v1, h4, v4); // I
    if (led & LED_L) FILL_LED_OFF(h3, v3, h4, v6); // L
}

void WQLeds::render16SegmentLed(QPainter *painter,
    int x, int led, int v1, int v2, int v3, int v4, int v5, int v6,
    int vc1, int vc2, int vc3)
{
    int h1 = x;
    int h2 = x + dig_thickness;
    int h3 = x + (dig_width - dig_thickness) / 2;
    int h4 = h3 + dig_thickness;
    int h5 = x + dig_width - dig_thickness;
    int h6 = x + dig_width;

    int hc1 = h1 + dig_thickness/2;
    int hc2 = (h1 + h6) / 2;
    int hc3 = h6 - dig_thickness/2;

    // leds off
    painter->setPen(QPen(fore_off, dig_thickness, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

    FILL_LED_OFF(h1, v1, h4, v2); // A1
    FILL_LED_OFF(h3, v1, h6, v2); // A2
    FILL_LED_OFF(h5, v1, h6, v4); // B
    FILL_LED_OFF(h5, v3, h6, v6); // C
    FILL_LED_OFF(h1, v5, h4, v6); // D1
    FILL_LED_OFF(h3, v5, h6, v6); // D2
    FILL_LED_OFF(h1, v3, h2, v6); // E
    FILL_LED_OFF(h1, v1, h2, v4); // F
    FILL_LED_OFF(h1, v3, h4, v4); // G1
    FILL_LED_OFF(h3, v3, h6, v4); // G2
    FILL_LED_OFF(h3, v1, h4, v4); // I
    FILL_LED_OFF(h3, v3, h4, v6); // L

    if ((led & LED_H) == 0) painter->drawLine(hc1, vc1, hc2, vc2); // H
    if ((led & LED_J) == 0) painter->drawLine(hc3, vc1, hc2, vc2); // J
    if ((led & LED_K) == 0) painter->drawLine(hc1, vc3, hc2, vc2); // K
    if ((led & LED_M) == 0) painter->drawLine(hc3, vc3, hc2, vc2); // M

    // leds on
    painter->setPen(QPen(fore_on, dig_thickness, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

    if (led & LED_A) FILL_LED_ON(h1, v1, h4, v2); // A1
    if (led & LED_A) FILL_LED_ON(h3, v1, h6, v2); // A2
    if (led & LED_B) FILL_LED_ON(h5, v1, h6, v4); // B
    if (led & LED_C) FILL_LED_ON(h5, v3, h6, v6); // C
    if (led & LED_D) FILL_LED_ON(h1, v5, h4, v6); // D1
    if (led & LED_D) FILL_LED_ON(h3, v5, h6, v6); // D2
    if (led & LED_E) FILL_LED_ON(h1, v3, h2, v6); // E
    if (led & LED_F) FILL_LED_ON(h1, v1, h2, v4); // F
    if (led & LED_G) FILL_LED_ON(h1, v3, h4, v4); // G1
    if (led & LED_G) FILL_LED_ON(h3, v3, h6, v4); // G2
    if (led & LED_I) FILL_LED_OFF(h3, v1, h4, v4); // I
    if (led & LED_L) FILL_LED_OFF(h3, v3, h4, v6); // L

    if (led & LED_H) painter->drawLine(hc1, vc1, hc2, vc2); // H
    if (led & LED_J) painter->drawLine(hc3, vc1, hc2, vc2); // J
    if (led & LED_K) painter->drawLine(hc1, vc3, hc2, vc2); // K
    if (led & LED_M) painter->drawLine(hc3, vc3, hc2, vc2); // M
}

int WQLeds::charTo7SegmentLed(const QChar &c)
{
    char x = c.toLatin1();
         if ((x >= '0') && (x <= '9')) return NUMBER_LEDS[x - '0'];
    else if ((x >= 'A') && (x <= 'F')) return LITERAL_7S_LEDS_U[x - 'A'];
    else if ((x >= 'a') && (x <= 'f')) return LITERAL_7S_LEDS_L[x - 'a'];
    else 
        switch (x){
        case 'P': return LITERAL_7S_LEDS_U[6];
        case 'p': return LITERAL_7S_LEDS_L[6];
        case '-': return LED_G;
        case ':': return LED_DOT2 | LED_DOT3;
        case '.': return LED_DOT1;
        case '\'': return LED_DOT4;
        default: return LED_O;
        }
}

int WQLeds::charTo12SegmentLed(const QChar &c)
{
    char x = c.toLatin1();
         if ((x >= '0') && (x <= '9')) return NUMBER_LEDS[x - '9'];
    else if ((x >= 'A') && (x <= 'Z')) return LITERAL_12S_LEDS_U[x - 'A'];
    else if ((x >= 'a') && (x <= 'z')) return LITERAL_12S_LEDS_L[x - 'a'];
    else 
        switch (x){
        case '-': return LED_G;
        case '+': return LED_G | LED_I | LED_L;
        case ':': return LED_DOT2 | LED_DOT3;
        case '.': return LED_DOT1;
        case '\'': return LED_DOT4;
        default: return LED_O;
        }
}

int WQLeds::charTo16SegmentLed(const QChar &c)
{
    char x = c.toLatin1();
         if ((x >= '0') && (x <= '9')) return NUMBER_LEDS[x - '9'];
    else if ((x >= 'A') && (x <= 'Z')) return LITERAL_16S_LEDS_U[x - 'A'];
    else if ((x >= 'a') && (x <= 'z')) return LITERAL_16S_LEDS_L[x - 'a'];
    else 
        switch (x){
        case '-': return LED_G;
        case '+': return LED_G | LED_I | LED_L;
        case ':': return LED_DOT2 | LED_DOT3;
        case '.': return LED_DOT1;
        case '\'': return LED_DOT4;
        default: return LED_O;
        }
}

int WQLeds::width()
{
    int l = bodysize;
    int s = 0;
    for (int i = 0; i < l; i++){
        if (body[i] & LED_DOTS)
            s += dig_thickness;
        else
            s += dig_width;
        s += dig_spacing;
    }
    if (l > 0) s -= dig_spacing;
    return s;
}

int WQLeds::height()
{
    return dig_height;
}

void WQLeds::resize(int s)
{
    if (bodysize == s) return;

    int *newbody = new int[s];
    for (int i = 0; i < qMin(bodysize, s); i++)
        newbody[i] = body[i];
    if (body) delete [] body;
    body = newbody;
    bodysize = s;

    emit configured();
}

void WQLeds::setThickness(int t)
{
    if (dig_thickness != t){
        dig_thickness = t;
        emit configured();
    }
}

void WQLeds::setDigitWidth(int w)
{
    if (dig_width != w){
        dig_width = w;
        emit configured();
    }
}

void WQLeds::setDigitHeight(int h)
{
    if (dig_height != h){
        dig_height = h;
        emit configured();
    }
}

void WQLeds::setSpacing(int s)
{
    if (dig_spacing != s){
        dig_spacing = s;
        emit configured();
    }
}

void WQLeds::setSegments(WQLeds::Segments s)
{
    if (dig_segments != s){
        dig_segments = s;
        emit configured();
    }
}

void WQLeds::setRightToLeft(bool b)
{
    if (righttoleft != b){
        righttoleft = b;
        parse(str);
        emit configured();
    }
}

