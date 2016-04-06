#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QKeyEvent>
#include <QPixmap>

#include "album.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* keyevent);

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
    bool moving;
    QString folder;
    //Musica *song;
    QList<Album*> albuns;
};

#endif // MAINWINDOW_H
