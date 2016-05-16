#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    folder = "C:/";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/alexf/Documents/QT/MusicPlayer/player.db");
    if (!db.open())
        QMessageBox::information(this, "Alerta", "Não se consegue connectar a base de dados!");

    readFromDB();

    addAlbum = new AddAlbumForm();
    addSong = new AddMusicForm();
    edit = new EditInfo();
    conf = new Config();

    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    idSong = 0;

    albumModel = new QStandardItemModel();
    tempSong = new QStandardItemModel();
    searchResults = new QStandardItemModel();

    editing = false;

    updateAlbumList();
    ui->listObjs->setModel(albumModel);
    ui->listObjs->setViewMode(QListView::IconMode);
    ui->listObjs->setIconSize(QSize(80, 80));
    ui->listObjs->setMovement(QListView::Static);
    ui->editSong->hide();
    selAlbum = -1;

    ui->playToggle->setText("Play");
    ui->volumeSlider->setValue(100);

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
    connect(edit, SIGNAL(finished(int)), this, SLOT(dialogEditFinished(int)));
    connect(ui->btnAddAlbum, SIGNAL(clicked(bool)), this, SLOT(on_actionAdicionarAlbum_triggered()));
    connect(ui->btnAddSong, SIGNAL(clicked(bool)), this, SLOT(on_actionAdicionarMusica_triggered()));
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
    delete player;
    delete addAlbum;
    delete addSong;
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

    if(keyevent->modifiers() == Qt::ControlModifier && keyevent->key() == Qt::Key_F){
        ui->searchBox->setFocus();
    }
}

void MainWindow::changeList(){
    int sel=ui->listObjs->currentIndex().row();
    if(ui->listObjs->model() == tempSong && selAlbum != -1){
        if(sel-2 >= albuns[selAlbum]->getTotalSongs())
                return;
        if(sel == 0){
            ui->listObjs->setModel(albumModel);
            ui->listObjs->setViewMode(QListView::IconMode);
            selAlbum = -1;
            ui->editSong->hide();
            return;
        }
        if(sel == 1){
            edit->setData(2, albuns[selAlbum]);
            edit->exec();
            return;
        }
        if(editing){
            edit->setData(1, albuns[selAlbum]->getSong(sel-2));
            edit->exec();
            editing = false;
            idSong = sel-2;
            return;
        }
        player->stop();
        if(playlist->mediaCount() > 0)
            playlist->removeMedia(playlist->mediaCount()-1);
        QString file = folder + albuns[selAlbum]->getSong(sel-2)->getFileName();
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(albuns[selAlbum]->getSong(sel-2)->getName());
        ui->songArtist->setText(albuns[selAlbum]->getSong(sel-2)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
        play();
    }
    else if(sel >= albuns.count())
        return;
    else{
        tempSong->clear();
        selAlbum = sel;
        updateSongList(selAlbum);
        ui->listObjs->setModel(tempSong);
        ui->listObjs->setViewMode(QListView::ListMode);
        ui->editSong->show();
    }
}

void MainWindow::on_actionAdicionarMusica_triggered()
{
    if(selAlbum == -1){
        QMessageBox msg;
        msg.setText("Nenhum Album selecionado, deseja criar um novo album?");
        msg.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        int re = msg.exec();
        if(re == QMessageBox::No)
            return;
        addAlbum->exec();
        if(addAlbum->result() != QDialog::Accepted)
            return;
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
        else if(list.count() == 3){
            albuns.append(new Album(list[0], list[1], list[2]));
            QSqlQuery query;
            query.prepare("INSERT INTO Albuns (Name, Description, ImagePath) "
                          "VALUES (:n, :d, :i)");
            query.bindValue(":n", list[0]);
            query.bindValue(":d", list[1]);
            query.bindValue(":i", list[2]);
            query.exec();
        }
        else{
            albuns.append(new Album(list[0], list[1]));
            QSqlQuery query;
            query.prepare("INSERT INTO Albuns (Name, Description, ImagePath) "
                          "VALUES (:n, :d, :i)");
            query.bindValue(":n", list[0]);
            query.bindValue(":d", list[1]);
            query.bindValue(":i", "");
            query.exec();
        }
        updateAlbumList();
        db.commit();
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
            if(selAlbum != -1){
                albuns[selAlbum]->addMusica(new Musica(list[0], list[2], l));
                QSqlQuery query;
                query.prepare("INSERT INTO Musica (Name, Artists, FilePath, Genre, DateAdded, Album) "
                              "VALUES (:n, :a, :f, :g, :d, :al)");
                query.bindValue(":n", list[0]);
                query.bindValue(":a", list[1]);
                query.bindValue(":f", list[2]);
                query.bindValue(":g", "");
                query.bindValue(":d", albuns[selAlbum]->getSong(
                                    albuns[selAlbum]->getTotalSongs()-1)->getDate());
                query.bindValue(":al", selAlbum);
                query.exec();
            }
            else{
                albuns.last()->addMusica(new Musica(list[0], list[2], l));
            }
        }
        updateSongList(selAlbum);
    }
}

void MainWindow::dialogEditFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = edit->getData();
        int type = list[0].toInt();
        if(type == 1 && list.count() == 4){//Musica
            albuns[selAlbum]->getSong(idSong)->setNome(list[1]);
            albuns[selAlbum]->getSong(idSong)->setArtistas(list[2]);
            albuns[selAlbum]->getSong(idSong)->setGenero(list[3]);
            QSqlQuery query;
            query.prepare("UPDATE Musica SET Name = :n, Artists = :a,"
                          "Genre = :g WHERE rowid = :sel");
            query.bindValue(":n", list[1]);
            query.bindValue(":a", list[2]);
            query.bindValue(":g", list[3]);
            query.bindValue(":sel", idSong+1);
            query.exec();
        }
        else if(type == 2 && list.count() == 4){//Album
            albuns[selAlbum]->setNome(list[1]);
            albuns[selAlbum]->setDescricao(list[2]);
            albuns[selAlbum]->setImagePath(list[3]);
            QSqlQuery query;
            query.prepare("UPDATE Albuns SET Name = :n, Description = :d,"
                          "ImagePath = :i WHERE rowid = :sel");
            query.bindValue(":n", list[1]);
            query.bindValue(":d", list[2]);
            query.bindValue(":i", list[3]);
            query.bindValue(":sel", selAlbum+1);
            query.exec();
        }
        else
            return;
    }
    updateAlbumList();
    updateSongList(selAlbum);
}

void MainWindow::updateAlbumList(){
    albumModel->clear();
    QStandardItem *Items;
    for(int i=0;i<albuns.count();i++){
        Items = new QStandardItem(albuns[i]->getNome());
        Items->setIcon(QPixmap(albuns[i]->getImagePath()));
        albumModel->appendRow(Items);
    }
}

void MainWindow::updateSongList(int alb){
    tempSong->clear();
    QStandardItem *Items;
    Items = new QStandardItem("Back");
    tempSong->appendRow(Items);
    Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Tem " + QString::number(albuns[alb]->getTotalSongs())
                             + " músicas");
    Items->setIcon(QPixmap(albuns[alb]->getImagePath()));
    tempSong->appendRow(Items);
    for(int i=0;i<albuns[alb]->getTotalSongs();i++){
        Items = new QStandardItem(albuns[alb]->getSong(i)->getName() + "\t"
                                 + albuns[alb]->getSong(i)->getArtistas()
                                 + "\t" + albuns[alb]->getSong(i)->getGenero());
        tempSong->appendRow(Items);
    }
}

void MainWindow::on_editSong_clicked()
{
    editing = true;
    QMessageBox::information(this, "", "Selecione a musica a editar");
}

void MainWindow::on_actionConfigura_o_triggered()
{
    conf->exec();
}

void MainWindow::readFromDB(){
    QSqlQuery query;
    QSqlRecord record;
    QString name, desc, imFile;
    QString art, file, gen;
    int id;
    QDate date;

    query.exec("SELECT * FROM Albuns");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        desc = record.value(1).toString();
        imFile = record.value(2).toString();
        albuns.append(new Album(name, desc, imFile));
    }
    query.clear();

    query.exec("SELECT * FROM Musica");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        art = record.value(1).toString();
        file = record.value(2).toString();
        gen = record.value(3).toString();
        date = record.value(4).toDate();
        id = record.value(5).toInt();
        albuns.at(id)->addMusica(new Musica(name,file,art,gen,date));
    }
}
