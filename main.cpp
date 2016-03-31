#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QPushButton>

QMediaPlayer *player;
void play();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}

