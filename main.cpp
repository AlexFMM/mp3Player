#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QPushButton>
/*!
  * \file main.cpp
*/

QMediaPlayer *player;
void play();
/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}

