#ifndef SNOW_H
#define SNOW_H

#include <QMainWindow>
#include "qpainter.h"
#include <QWidget>
#include <QPainter>
#include <QTimer>


namespace Ui {
class snow;
}

class snow : public QMainWindow
{
    Q_OBJECT

public:

    explicit snow(QWidget *parent = 0);
    ~snow();
    void paintEvent(QPaintEvent*);
    void drawLine(QPainter &painter, int width, int height);
    void drawOctagon(QPainter &painter);
    //void drawDot(QPainter &painter, int x, int y);

private:
    Ui::snow *ui;

    qreal m_scale = 1.0; // текущее масштабирование
    qreal m_rotation = 0.0; // текущий поворот в градусах
    QPointF m_pos = QPointF(0, 0); // текущее положение восьмиугольника
public slots:
    void update();

};

#endif // SNOW_H




