#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    folder = "C:/Users/alexf/Music/";

    player = new QMediaPlayer();
    //song = new Musica("The Hunter", "The_Hunter-Mastodon.mp3", "Mastodon", "Metal");
    albuns.append(new Album("Coisa mais nice", "Isto tem cenas bue de fixes", "C:/Users/alexf/Pictures/foto001.jpg"));
    albuns[0]->addMusica();

    ui->playToggle->setText("Play");
    ui->volumeSlider->setValue(100);
    moving = false;
    connect(ui->playToggle,SIGNAL(clicked()),this,SLOT(play()));
    connect(this,SIGNAL(),this,SLOT(play()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)), this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setBarPosition()));
    connect(ui->songPosition, SIGNAL(sliderReleased()), this, SLOT(setSongPosition()));
    connect(ui->songPosition, SIGNAL(sliderMoved(int)), this, SLOT(movingSlider()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(songEnd()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    //delete song;
    delete ui->playToggle;
    delete ui->volumeSlider;
    delete ui->songPosition;
    delete ui->currentTime;
    delete ui->totalTime;
    delete ui->songName;
    delete ui->songArtist;
}

void MainWindow::play(){
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        ui->playToggle->setText("Play");
    }
    else if(player->state() == QMediaPlayer::PausedState){
        player->play();
        ui->playToggle->setText("Pause");
    }
    else{
        player->setMedia(QUrl::fromLocalFile(folder.append(albuns[0]->getSong(0)->getFileName())));
        ui->songName->setText(albuns[0]->getSong(0)->getName());
        ui->songArtist->setText(albuns[0]->getSong(0)->getArtistas());
        //QPixmap *item = new QPixmap(albuns[0]->getImagePath());
        //item = item->scaled(ui->albumImage->width(), ui->albumImage->height(),Qt::KeepAspectRatio)
        //ui->albumImage->setPixmap(*item);
        player->setVolume(ui->volumeSlider->value());
        player->play();
        ui->playToggle->setText("Pause");
    }
}

void MainWindow::setVolume(){
    player->setVolume(ui->volumeSlider->value());
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
    ui->totalTime->setText(time);
    ui->songPosition->setMaximum(duration);
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
    ui->currentTime->setText(time);
    if (!moving)
        ui->songPosition->setValue(position);
}

void MainWindow::setSongPosition(){
    player->play();
}

void MainWindow::movingSlider(){
    player->pause();
    moving = true;
    player->setPosition(ui->songPosition->value());
}

void MainWindow::songEnd(){
    if(player->state() == QMediaPlayer::StoppedState){
        ui->playToggle->setText("Play");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *keyevent){
    if(keyevent->key() == Qt::Key_MediaTogglePlayPause || keyevent->key() == Qt::Key_Space){
        play();
    }
}
