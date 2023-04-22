#include "snow.h"
#include "ui_snow.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QtMath>

snow::snow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::snow)
{
    resize(800, 800);
    setFixedSize(800, 800);
    m_pos = QPoint(100, 100*2/3+800/3); // установка начальной позиции
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &snow::update);
    timer->start(100); // Запускаем таймер с интервалом в миллисекундах

}
snow::~snow()
{
    delete ui;
}

void snow::update() {
    // Увеличиваем масштаб на 10%
    m_scale *= 1.1;

    // Поворачиваем на
    m_rotation += 20.0;

    // Двигаем вниз вправо
    m_pos.rx() += 9;
    m_pos.ry() += 6;

    //Проверяем, не вышел ли центр за границы экрана
    if (m_pos.x() > width()-300 || m_pos.y() > height()-300) {
        return; // Останавливаем процесс обновления
    }

    // Перерисовываем виджет
    QWidget::update();
}


void snow::drawLine(QPainter &painter, int width, int height) {
    QPen pen(QColor(0, 0, 0));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);//пунктир
    pen.setDashPattern({5, 2});
    painter.setPen(pen);
    painter.drawLine(0, height / 3, width, height);
    pen.setStyle(Qt::SolidLine); // установить сплошную линию
    painter.setPen(pen);
}


void snow::drawOctagon(QPainter &painter, int x, int y) {//центр-поворот- масштабирование - спуск-обратно
    double adds = sqrt(2)/2;

    double snowball[9][4]={{5, 5, 10, 5},
                           {10, 5, 10+5*adds, 5+5*adds},
                           {10+5*adds, 5+5*adds, 10+5*adds, 10+5*adds},
                           {10+5*adds,10+5*adds, 10, 10+10*adds},
                           {10, 10+10*adds, 5, 10+10*adds},
                           {5, 10+10*adds, 5-5*adds, 10+5*adds},
                           {5-5*adds, 10+5*adds, 5-5*adds, 5+5*adds},
                           {5-5*adds, 5+5*adds, 5, 5},
                           {5, 5, 10, 5}
                          };



    // Определяем масштаб и поворот для отрисовки
    double scale = m_scale;
    double angle = m_rotation * M_PI / 180.0;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    // Создаем полигон на основе точек из массива snowball
    QPolygonF poly;
    QPolygonF poly2;
    double x_center = 0;
    double y_center = 0;
    for (int i = 0; i < 8; i++){
        x_center += snowball[i][0];
        y_center += snowball[i][1];
    }

    x_center = x_center/8;
    y_center = y_center/8;
    painter.translate(x_center, y_center);//перевод начала координат в центр восьмиугольника

    for (int i = 0; i < 8; i++) {//поворот
        double x1 = snowball[i][0] - x_center;
        double y1 = snowball[i][1] - y_center;
        double x2 = x1 * cosAngle - y1 * sinAngle;
        double y2 = x1 * sinAngle + y1 * cosAngle;
        poly << QPointF(x2 + x_center, y2 + y_center);
    }


    painter.translate(-x_center, -y_center);


    for (int i = 0; i < 8; i++) {
        QPointF Point = poly.at(i);

        double x1 = Point.x();
        double y1 = Point.y();

        x1 *= scale;//масштаб
        y1 *= scale;
        x1 += m_pos.rx();//сдвиг
        y1 += m_pos.ry()-y_center;
        poly2 << QPointF(x1, y1);
    }


    painter.drawPolygon(poly2); // Рисуем полигон
}


void snow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int width = size().width();
    int height = size().height();
    drawLine(painter, width, height);
    painter.setRenderHint(QPainter::Antialiasing, true);//сглаживание
    drawOctagon(painter,0,0);

}

