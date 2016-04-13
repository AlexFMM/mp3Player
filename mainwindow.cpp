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

    albuns.append(new Album("Coisa mais nice", "Isto tem cenas bue de fixes", "C:/Users/alexf/Music/img.jpg"));
    albuns[0]->addMusica(new Musica("The Hunter", "The_Hunter-Mastodon.mp3", "Mastodon", "Metal"));

    albumModel = new QStandardItemModel();
    tempSong = new QStandardItemModel();
    searchResults = new QStandardItemModel();

    QStandardItem *Items;
    for(int i=0;i<albuns.count();i++){
        Items = new QStandardItem(albuns[i]->getNome());
        Items->setIcon(QPixmap(albuns[i]->getImagePath()));
        albumModel->appendRow(Items);
    }
    ui->listObjs->setModel(albumModel);
    ui->listObjs->setViewMode(QListView::IconMode);
    ui->listObjs->setIconSize(QSize(80, 80));
    ui->listObjs->setMovement(QListView::Static);
    selAlbum = -1;

    ui->playToggle->setText("Play");
    ui->volumeSlider->setValue(100);

    ui->songArtist->setText(QString::number(albuns[0]->getTotalSongs()));

    //connect the events
    connect(ui->playToggle,SIGNAL(clicked()),this,SLOT(play()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)), this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setBarPosition()));
    connect(ui->songPosition, SIGNAL(sliderReleased()), this, SLOT(setSongPosition()));
    connect(ui->songPosition, SIGNAL(sliderMoved(int)), this, SLOT(movingSlider()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(songEnd()));
    connect(ui->listObjs, SIGNAL(clicked(QModelIndex)), this, SLOT(changeList()));
    connect(addAlbum, SIGNAL(finished(int)), this, SLOT(dialogAlbumFinished(int)));
    connect(addSong, SIGNAL(finished(int)), this, SLOT(dialogMusicFinished(int)));
    connect(ui->btnAddAlbum, SIGNAL(clicked(bool)), this, SLOT(on_actionAdicionarAlbum_triggered()));
    connect(ui->btnAddSong, SIGNAL(clicked(bool)), this, SLOT(on_actionAdicionarMusica_triggered()));
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
    }
    else if(player->state() == QMediaPlayer::PausedState){
        player->play();
    }
    else{
        player->setVolume(ui->volumeSlider->value());
        player->play();
    }
}

void MainWindow::songEnd(){
    if(player->state() == QMediaPlayer::StoppedState){
        ui->playToggle->setText("Play");
    }
    else if(player->state() == QMediaPlayer::PausedState){
        ui->playToggle->setText("Play");
    }
    else if(player->state() == QMediaPlayer::PlayingState){
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

void MainWindow::keyPressEvent(QKeyEvent *keyevent){
    if(keyevent->key() == Qt::Key_MediaTogglePlayPause || keyevent->key() == Qt::Key_Space){
        play();
    }
    else if( keyevent->key() == Qt::Key_Escape && selAlbum !=-1){
        ui->listObjs->setModel(albumModel);
        ui->listObjs->setViewMode(QListView::IconMode);
        selAlbum = -1;
    }

    if(keyevent->modifiers() == Qt::ControlModifier && keyevent->key() == Qt::Key_S){
        ui->searchBox->setFocus();
    }
}

void MainWindow::changeList(){
    QStandardItem *Items;
    int sel=ui->listObjs->currentIndex().row();
    if(ui->listObjs->model() == tempSong && selAlbum != -1){
        if(sel-2 >= albuns[selAlbum]->getTotalSongs())
                return;
        if(sel == 0){
            ui->listObjs->setModel(albumModel);
            ui->listObjs->setViewMode(QListView::IconMode);
            selAlbum = -1;
            return;
        }
        player->stop();
        if(playlist->mediaCount() > 0)
            playlist->removeMedia(playlist->mediaCount()-1);
        QString file = folder + albuns[selAlbum]->getSong(sel-2)->getFileName();
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(file);
        ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
        play();
    }
    else if(sel >= albuns.count())
        return;
    else{
        tempSong->clear();
        selAlbum = sel;
        Items = new QStandardItem("Back");
        tempSong->appendRow(Items);
        Items = new QStandardItem(albuns[selAlbum]->getNome()+ "\t\t"
                                 + "Tem " + QString::number(albuns[selAlbum]->getTotalSongs())
                                  + " músicas");
        Items->setIcon(QPixmap(albuns[selAlbum]->getImagePath()));
        tempSong->appendRow(Items);
        for(int i=0;i<albuns[sel]->getTotalSongs();i++){
            Items = new QStandardItem(albuns[sel]->getSong(i)->getName() + "\t\t"
                                      + albuns[sel]->getSong(i)->getArtistas()
                                      + "\t\t" + albuns[sel]->getSong(i)->getGenero());
            tempSong->appendRow(Items);
        }
        ui->listObjs->setModel(tempSong);
        ui->listObjs->setViewMode(QListView::ListMode);
    }
}

void MainWindow::on_actionAdicionarMusica_triggered()
{
    if(selAlbum == -1){
        QMessageBox::information(this, "Alerta", "Nenhum Album selecionado, vai ser criado um novo album");
        addAlbum->exec();
        selAlbum = albuns.count()-1;
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
        Items->setIcon(QPixmap(albuns.last()->getImagePath()));
        albumModel->appendRow(Items);
    }
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
            list[2] = list[2].replace(folder, "");
            if(selAlbum != -1)
                albuns[selAlbum]->addMusica(new Musica(list[0], list[2], l));
            else{
                albuns.last()->addMusica(new Musica(list[0], list[2], l));
            }
        }
        QStandardItem *Items;
        int idSong = albuns[selAlbum]->getTotalSongs()-1;
        Items = new QStandardItem(albuns[selAlbum]->getSong(idSong)->getName() + "\t"
                                 + albuns[selAlbum]->getSong(idSong)->getArtistas()
                                 + "\t" + albuns[selAlbum]->getSong(idSong)->getGenero());
        tempSong->appendRow(Items);
        ui->songArtist->setText(QString::number(albuns[selAlbum]->getTotalSongs()));
    }
}
