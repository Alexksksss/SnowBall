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
    timer->start(500); // Запускаем таймер с интервалом в миллисекундах

}
snow::~snow()
{
    delete ui;
}

void snow::update() {
    // Увеличиваем масштаб на 10%
    m_scale *= 1.15;

    // Поворачиваем на
    m_rotation += 22.5;

    // Двигаем вниз вправо
    m_pos.rx() += 25;
    m_pos.ry() += 0;//1.1/sqrt(2)*2

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
    painter.drawLine(0, 330, width, height/2 + 20);
    pen.setStyle(Qt::SolidLine); // установить сплошную линию
    painter.setPen(pen);
}



void snow::drawOctagon(QPainter &painter) {//центр-поворот- масштабирование - спуск-обратно
    //painter.setBrush(QBrush(Qt::white));
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
    QPolygonF poly_Dot;
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
        poly << QPointF(x2, y2 );//+ y_center
    }


    for (int i = 0; i < 8; i++) {
        QPointF Point = poly.at(i);

        double x1 = Point.x();
        double y1 = Point.y();
        double x2 = x1;
        double y2 = y1;

        x1 *= scale;//масштаб
        y1 *= scale;
        x1 += m_pos.rx();//сдвиг
        y1 += m_pos.ry();//-2*y_center

        x2 += m_pos.rx();
        y2 += m_pos.ry();
        poly2 << QPointF(x1, y1);
        poly_Dot << QPointF(x2,y2);
    }

    painter.translate(-x_center, -y_center);//*1.1*2
    QPointF Point_Dot = poly_Dot.at(0);

    double x_Dot = Point_Dot.x();
    double y_Dot = Point_Dot.y();


    painter.setBrush(QBrush(Qt::white));
    painter.drawPolygon(poly2); // Рисуем полигон
    painter.setBrush(QBrush(Qt::green));
    painter.drawEllipse(x_Dot, y_Dot, 3, 3);//Рисуем точку


}


void snow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int width = size().width();
    int height = size().height();
    painter.setRenderHint(QPainter::Antialiasing, true);//сглаживание
    drawLine(painter, width, height);

    drawOctagon(painter);



}


// y = 2/3*800+1/3x
