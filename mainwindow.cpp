#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer();

    btn = this->findChild<QPushButton*>("playToggle");
    btn->setText("Play");
    vol = this->findChild<QSlider*>("volumeSlider");
    vol->setValue(100);
    pos = this->findChild<QSlider*>("songPosition");
    curL = this->findChild<QLabel*>("currentTime");
    totL = this->findChild<QLabel*>("totalTime");
    moving = false;
    connect(btn,SIGNAL(clicked()),this,SLOT(play()));
    connect(vol,SIGNAL(valueChanged(int)), this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setBarPosition()));
    connect(pos, SIGNAL(sliderReleased()), this, SLOT(setSongPosition()));
    connect(pos, SIGNAL(sliderMoved(int)), this, SLOT(movingSlider()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(songEnd()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete btn;
    delete vol;
    delete pos;
    delete curL;
    delete totL;
}

void MainWindow::play(){
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        btn->setText("Play");
    }
    else if(player->state() == QMediaPlayer::PausedState){
        player->play();
        btn->setText("Pause");
    }
    else{
        player->setMedia(QUrl::fromLocalFile("C:/Users/alex/Music/The_Hunter-Mastodon.mp3"));
        player->setVolume(vol->value());
        player->play();
        btn->setText("Pause");
    }
}

void MainWindow::setVolume(){
    player->setVolume(vol->value());
}

void MainWindow::setEndTime(){
    int duration = player->duration();
    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;
    QString time;
    if(hours > 0)
        time.append(QString("%1").arg(hours, 1, 10) + ":" +
                    QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    else
        time.append(QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    totL->setText(time);
    pos->setMaximum(duration);
}

void MainWindow::setBarPosition(){
    int duration = player->duration();
    if(duration == 0)
        return;
    int position = player->position();
    int seconds = (position/1000) % 60;
    int minutes = (position/60000) % 60;
    int hours = (duration/3600000) % 24;
    QString time;
    if(hours > 0)
        time.append(QString("%1").arg(hours, 1, 10) + ":" +
                    QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    else
        time.append(QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    curL->setText(time);
    if (!moving)
        pos->setValue(position);
}

void MainWindow::setSongPosition(){
    player->play();
}

void MainWindow::movingSlider(){
    player->pause();
    moving = true;
    player->setPosition(pos->value());
}

void MainWindow::songEnd(){
    if(player->state() == QMediaPlayer::StoppedState){
        btn->setText("Play");
    }
}
