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
    connect(btn,SIGNAL(clicked()),this,SLOT(play()));
    connect(vol,SIGNAL(valueChanged(int)), this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setBarPosition()));
    connect(pos, SIGNAL(sliderReleased()), this, SLOT(setSongPosition()));
    //connect(pos, SIGNAL(), this, SLOT());
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
        player->setMedia(QUrl::fromLocalFile("C:/Users/alexf/Music/These_Days-Foo_Fighters.mp3"));
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
    QString time;
    if(seconds < 10)
        time = QString::number(minutes) + ":0" +  QString::number(seconds);
    else
        time = QString::number(minutes) + ":" +  QString::number(seconds);
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
    QString time;
    if(seconds < 10)
        time = QString::number(minutes) + ":0" +  QString::number(seconds);
    else
        time = QString::number(minutes) + ":" +  QString::number(seconds);
    curL->setText(time);
    pos->setValue(position);
}

void MainWindow::setSongPosition(){
    player->pause();
    player->setPosition(pos->value());
    player->play();
}
