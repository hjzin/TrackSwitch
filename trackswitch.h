#ifndef TRACKSWITCH_H
#define TRACKSWITCH_H

#include <QMainWindow>

namespace Ui {
class TrackSwitch;
}

class TrackSwitch : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrackSwitch(QWidget *parent = 0);
    ~TrackSwitch();

private slots:
    void on_toolButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::TrackSwitch *ui;
};

#endif // TRACKSWITCH_H
