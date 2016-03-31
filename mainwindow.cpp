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
    connect(btn,SIGNAL(clicked()),this,SLOT(play()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete btn;
    delete player;
}

void MainWindow::play(){
    btn->setText("Clicked");
    player->setMedia(QUrl::fromLocalFile("C:/Users/alexf/Music/The_Hunter-Mastodon.mp3"));
    player->setVolume(100);
    player->play();
}
