#ifndef SNOW_H
#define SNOW_H

#include <QMainWindow>

namespace Ui {
class snow;
}

class snow : public QMainWindow
{
    Q_OBJECT

public:
    explicit snow(QWidget *parent = 0);
    ~snow();

private:
    Ui::snow *ui;
};

#endif // SNOW_H
