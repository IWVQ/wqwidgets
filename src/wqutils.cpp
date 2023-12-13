// Copyright (c) 2023 Ivar Vilca Quispe
// see LICENSE for details

#include "wqutils.h"

int charToDigit(QChar c)
{
    return (static_cast<int>(c.toLatin1() - '0'))
}

inline QString doubleToStr(double n, int precision)
{
    return QString::number(n, 'g', precision);
}

QString magnitudeToStr(const QString &format, double pos, double min, double max)
{
    int l = format.length();
    QString result = "";
    QChar c;
    
    int i = 0;
    while (i < l){
        // scan char
        c = format[i];
        i++;
        
        if (c == '%' && i < l){
            // scan char
            c = format[i];
            i++;
            
            // scan precision
            int precision = 0;
            if (i < l && format[i] == '.')
                while (i < l && isDigitChar(format[i])){
                    precision = precision*10 + charToDigit(format[i]);
                    i++;
                }
            
            // convert
            char x = c.toLatin1();
            QString s = "";
            switch (x){
            case 'p':
                if (max == min)
                    if (pos == min) s = "0";
                    else 
                        if (pos > min) s = "∞";
                        else s = "-∞";
                else 
                    s = doubleToStr(100*(Pos - Min)/(Max - Min), precision)
                break;
            case 'v': s = doubleToStr(pos, precision); break;
            case 'm': s = doubleToStr(max - min, precision); break;
            case 'n': s = doubleToStr(min, precision); break;
            case 'x': s = doubleToStr(max, precision); break;
            default : s = c;
            }
            result.append(s);
        }
        else
            result.append(c);
    }
    
    return result;
}
