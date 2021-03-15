#include "month.h"
#include <QLabel>
//#include <date.h>

bool isLeapYear(int y) {
    bool divisibleBy4 = y%4==0;
    bool divisibleBy100 = y%100==0;
    bool divisibleBy400 = y%400==0;
    return (divisibleBy4 && !divisibleBy100) || divisibleBy400;
}
int dayOfWeek(int y, int m, int d) {
    return (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
}
int daysInMonth(int y, int m) { // month 1-based
    int table[12] = {31,-1,31,30,31,30,31,31,30,31,30,31};
    if(m != 2) {
        return table[m-1];
    } else {
        return 28 + isLeapYear(y);
    }
}
struct tm currentDay() {
    time_t t = time(0);
    return *(localtime(&t));
}

Month::Month(QWidget *parent) : QWidget(parent)
{
    QFontMetrics fm = fontMetrics();
    const int margin = 2;
    dh = fm.height() + 2*margin;
    dw = fm.width("77") + 2*margin;
    for(int x = 0; x < 5; x++) {
        for(int y = 0; y < 7; y++) {
            QLabel* l = new QLabel(this);
            l->setAlignment(Qt::AlignRight);
            l->setText(QString::number(x*10+y));
            l->setFixedSize(dw, dh);
            l->move(dw*x, dw*y);
            dayWidgets[x*7+y] = l;
        }
    }
    setFixedSize(dh*7, dw*5);
//    printf("15 03 2021 %d %d days 15 04 2021 %d %d days 15 05 2021 %d %d days",
//           dayOfWeek(2021, 3, 15), daysInMonth(2021, 3),
//           dayOfWeek(2021, 4, 15), daysInMonth(2021, 4),
//           dayOfWeek(2021, 5, 15), daysInMonth(2021, 5));

}

void Month::setMonth(int y, int m) {

    int days = daysInMonth(y, m);
    int square = 0;
    int day = 2-dayOfWeek(y, m, 1);

    struct tm current = currentDay();
    while(square < 5*7) {
        if(day < 1 || day > days) {
            dayWidgets[square]->setText("");
        } else {
            dayWidgets[square]->setNum(day);
        }
        if(current.tm_year == y &&
            current.tm_mon+1 == m &&
            current.tm_mday == day) {
            dayWidgets[square]->setFrameStyle(QLabel::Panel);
        } else {
            dayWidgets[square]->setFrameStyle(QLabel::NoFrame);
        }
        square++;
        day++;
    }
}
