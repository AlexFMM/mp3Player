#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void play();
    void setVolume();
    void setBarPosition();
    void setSongPosition();
    void movingSlider();
    void setEndTime();
    void songEnd();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QPushButton *btn;
    QSlider *vol;
    QSlider *pos;
    QLabel *curL;
    QLabel *totL;
    bool moving;

};

#endif // MAINWINDOW_H
