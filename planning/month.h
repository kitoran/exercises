#ifndef MONTH_H
#define MONTH_H

#include <QLabel>

struct year_month {
    int year;
    int month;
};

class Month : public QWidget
{
    Q_OBJECT
public:
    explicit Month(QWidget *parent = nullptr);

    void setMonth(int y, int m);
signals:

public slots:
private:
    static inline int dh;
    static inline int dw;
    QLabel *dayWidgets[5*7];
};

#endif // MONTH_H
