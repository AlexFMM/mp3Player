#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    folder = "C:/Users/alexf/Music/";

    addAlbum = new AddAlbumForm();
    addSong = new AddMusicForm();

    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    //song = new Musica("The Hunter", "The_Hunter-Mastodon.mp3", "Mastodon", "Metal");
    albuns.append(new Album("Coisa mais nice", "Isto tem cenas bue de fixes", "C:/Users/alexf/Pictures/foto001.jpg"));
    albuns[0]->addMusica(new Musica("The Hunter", "The_Hunter-Mastodon.mp3", "Mastodon", "Metal"));

    ui->playToggle->setText("Play");
    ui->volumeSlider->setValue(100);
    connect(ui->playToggle,SIGNAL(clicked()),this,SLOT(play()));
    //connect(this,SIGNAL(),this,SLOT(play()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)), this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setBarPosition()));
    connect(ui->songPosition, SIGNAL(sliderReleased()), this, SLOT(setSongPosition()));
    connect(ui->songPosition, SIGNAL(sliderMoved(int)), this, SLOT(movingSlider()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(songEnd()));
    connect(ui->listObjs, SIGNAL(clicked(QModelIndex)), this, SLOT(changeList()));
    connect(addAlbum, SIGNAL(finished(int)), this, SLOT(dialogAlbumFinished(int)));
    //connect(addSong, SIGNAL(finished(int)), this, SLOT(dialogMusicFinished(int)));

    QStandardItem *Items;
    albumModel = new QStandardItemModel();
    tempSong = new QStandardItemModel();
    for(int i=0;i<albuns.count();i++){
        Items = new QStandardItem(albuns[i]->getNome());
        albumModel->appendRow(Items);
    }
    ui->listObjs->setModel(albumModel);
    ui->listObjs->setViewMode(QListView::IconMode);
    ui->listObjs->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for(int i=0;i<20;i++){
        Items = new QStandardItem("Album " + QString::number(i));
        albumModel->appendRow(Items);
    }
    selAlbum = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete addAlbum;
    delete addSong;
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
    else if( keyevent->key() == Qt::Key_Escape && selAlbum !=-1){
        ui->listObjs->setModel(albumModel);
        selAlbum = -1;
    }
}

void MainWindow::changeList(){
    QStandardItem *Items;
    int sel=ui->listObjs->currentIndex().row();
    if(sel >= albuns.count())
        return;
    if(ui->listObjs->model() == tempSong && selAlbum != -1){
        playlist->addMedia(QUrl::fromLocalFile(folder.append(albuns[selAlbum]->getSong(sel)->getFileName())));
        play();
    }
    else{
        tempSong->clear();
        selAlbum = sel;
        for(int i=0;i<albuns[sel]->getTotalSongs();i++){
            Items = new QStandardItem(albuns[sel]->getSong(i)->getName());
            tempSong->appendRow(Items);
        }
        ui->listObjs->setModel(tempSong);
    }
}

void MainWindow::on_actionAdicionarMusica_triggered()
{
    if(selAlbum == -1){
        QMessageBox::information(this, "Erro", "Nenhum Album selecionado");
        return;
    }
    addSong->exec();
}

void MainWindow::on_actionAdicionarAlbum_triggered()
{
    addAlbum->exec();
}

void MainWindow::dialogAlbumFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = addAlbum->getInfo();
        if(list.count() < 2){
            addAlbum->exec();
            return;
        }
        else if(list.count() == 3)
            albuns.append(new Album(list[0], list[1], list[2]));
        else
            albuns.append(new Album(list[0], list[1]));
        QStandardItem *Items;
        Items = new QStandardItem(albuns.last()->getNome());
        albumModel->appendRow(Items);
    }
    delete addAlbum;
    addAlbum = new AddAlbumForm();
}

void MainWindow::dialogMusicFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = addSong->getInfo();
        if(list.count() < 3){
            addSong->exec();
            return;
        }
        else{
            QList<QString> l = list[1].split(", ");
            ui->songArtist->setText(list[2]);
            list[2] = list[2].replace(folder, "");
            ui->songName->setText(list[2]);
            albuns[selAlbum]->addMusica(new Musica(list[0], list[1], l));
        }
        QStandardItem *Items;
        Items = new QStandardItem(albuns[selAlbum]->getSong(albuns[selAlbum]->getTotalSongs()-1)->getName());
        tempSong->appendRow(Items);
    }
    delete addSong;
    addSong = new AddMusicForm();
}
