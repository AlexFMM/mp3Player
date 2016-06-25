#include "mainwindow.h"
#include "ui_mainwindow.h"
/*!
  * \file mainwindow.cpp
*/

#include <QCoreApplication>
/*!
 * \brief MainWindow::MainWindow Criação da janela principal do programa
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString dbLoc = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                            + QDir::separator() + "dotPlay";
    if(!QDir(dbLoc).exists())
        QDir().mkdir(dbLoc);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbLoc + QDir::separator() + "player.db");
    db.open();
    if (db.tables().size() == 0){
        QMessageBox::information(this, "Primeira Utilização"
                      , "Não foi encontrada base de dados, vai ser ciada uma.");
        createDB();
    }
    else{
        readFromDB();
    }

    confirmed = false;

    this->setWindowIcon(QIcon("icon.ico"));

    if(!QDir(folder).exists())
        QDir().mkdir(folder);

    addAlbum = new AddAlbumForm();
    addSong = new AddMusicForm();
    addPlay = new AddPlaylistForm();
    edit = new EditInfo();
    conf = new Config();

    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    idSong = -1;

    albumModel = new QStandardItemModel();
    tempSong = new QStandardItemModel();
    playModel = new QStandardItemModel();
    tempPlay = new QStandardItemModel();
    searchResults = new QStandardItemModel();

    ui->Viewer->setCurrentIndex(0);
    ui->Viewer->setTabEnabled(2,false);

    updateAlbumList();
    updatePlaylist();
    ui->listPlay->setModel(playModel);
    ui->listPlay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listPlay->horizontalHeader()->hide();
    ui->listPlay->verticalHeader()->hide();
    ui->listPlay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listPlay->setShowGrid(false);

    ui->listObjs->setModel(albumModel);
    ui->listObjs->horizontalHeader()->hide();
    ui->listObjs->verticalHeader()->hide();
    ui->listObjs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listObjs->setShowGrid(false);
    ui->listObjs->setIconSize(QSize(80, 80));
    ui->listObjs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listObjs->resizeRowsToContents();
    ui->listObjs->resizeRowsToContents();

    ui->listResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listResults->setShowGrid(false);
    ui->listResults->horizontalHeader()->hide();
    ui->listResults->verticalHeader()->hide();
    ui->listResults->setSelectionBehavior(QAbstractItemView::SelectRows);

    selAlbum = -1;
    selPlay = -1;

    ui->listObjs->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listPlay->setContextMenuPolicy(Qt::CustomContextMenu);


    ui->playToggle->setText("Play");
    ui->volumeSlider->setValue(100);
    ui->repetir->setText("Não repetir");
    ui->aleatorio->setText("Sequencial");
    ui->songArtist->setText("Artista(s)");
    ui->songName->setText("Música");


    //connect the events
    connect(ui->playToggle,SIGNAL(clicked()),
                        this,SLOT(play()));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),
                        this, SLOT(setVolume()));
    connect(player, SIGNAL(durationChanged(qint64)),
                        this, SLOT(setEndTime()));
    connect(player, SIGNAL(positionChanged(qint64)),
                        this, SLOT(setBarPosition()));
    connect(ui->songPosition, SIGNAL(sliderReleased()),
                        this, SLOT(setSongPosition()));
    connect(ui->songPosition, SIGNAL(sliderMoved(int)),
                        this, SLOT(movingSlider()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
                        this, SLOT(songEnd()));
    connect(ui->listObjs, SIGNAL(clicked(QModelIndex)),
                        this, SLOT(changeList()));
    connect(ui->listPlay, SIGNAL(clicked(QModelIndex)),
                        this, SLOT(changePlayLists()));
    connect(addAlbum, SIGNAL(finished(int)),
                        this, SLOT(dialogAlbumFinished(int)));
    connect(addSong, SIGNAL(finished(int)),
                        this, SLOT(dialogMusicFinished(int)));
    connect(edit, SIGNAL(finished(int)),
                        this, SLOT(dialogEditFinished(int)));
    connect(addPlay, SIGNAL(finished(int)),
                        this, SLOT(dialogPlayListFinished(int)));
    connect(conf, SIGNAL(finished(int)),
                        this, SLOT(dialogConfigFinished(int)));
    connect(ui->btnAddAlbum, SIGNAL(clicked(bool)),
                        this, SLOT(on_actionAdicionarAlbum_triggered()));
    connect(ui->btnAddSong, SIGNAL(clicked(bool)),
                        this, SLOT(on_actionAdicionarMusica_triggered()));
    connect(ui->searchBox, SIGNAL(textChanged(QString)),
                        this, SLOT(search()));
    connect(ui->listObjs,SIGNAL(customContextMenuRequested(const QPoint &)),
                        this,SLOT(ProvideContextMenu(const QPoint &)));
    connect(ui->listPlay,SIGNAL(customContextMenuRequested(const QPoint &)),
                        this,SLOT(ProvideContextMenuPlay(const QPoint &)));
    connect(playlist, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(changeInfo()));
    connect(ui->orderChoice, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(order()));
}
/*!
 * \brief MainWindow::~MainWindow Eliminação da janela principal do programa
 */
MainWindow::~MainWindow()
{
    db.close();
    delete ui;
    delete player;
    delete addAlbum;
    delete addSong;
}
/*!
 * \brief MainWindow::play Reprodução de música
 */
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
/*!
 * \brief MainWindow::songEnd Fim de música
 */
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
/*!
 * \brief MainWindow::setVolume Ajustador de volume
 */
void MainWindow::setVolume(){
    player->setVolume(ui->volumeSlider->value());
}
/*!
 * \brief MainWindow::setEndTime Tempo de fim da música
 */
void MainWindow::setEndTime(){
    int duration = player->duration();
    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;
    QString time;
    if(hours > 0)
        time.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                    QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    else
        time.append(QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    ui->totalTime->setText(time);
    ui->songPosition->setMaximum(duration);
}
/*!
 * \brief MainWindow::setBarPosition Posição da barra de progresso da música
 */
void MainWindow::setBarPosition(){
    int duration = player->duration();
    if(duration == 0)
        return;
    int position = player->position();
    int seconds = (position/1000) % 60;
    int minutes = (position/60000) % 60;
    int hours = (position/3600000) % 24;
    QString time;
    if(hours > 0)
        time.append(QString("%1").arg(hours, 1, 10, QLatin1Char('0')) + ":" +
                    QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    else
        time.append(QString("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                    QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
    ui->currentTime->setText(time);
    if (!moving)
        ui->songPosition->setValue(position);
}
/*!
 * \brief MainWindow::setSongPosition posição da música
 */
void MainWindow::setSongPosition(){
    player->play();
}
/*!
 * \brief MainWindow::movingSlider Controlo da barra de progresso da música
 */
void MainWindow::movingSlider(){
    player->pause();
    moving = true;
    player->setPosition(ui->songPosition->value());
}

//Handle the keyboard press event
void MainWindow::keyPressEvent(QKeyEvent *keyevent){
    //Media Play/Pause and the space key toggle the play status
    if(keyevent->key() == Qt::Key_MediaTogglePlayPause ||
        (keyevent->modifiers() == Qt::ControlModifier
                                            && keyevent->key() == Qt::Key_P)){
        play();
    }
    //Esc key resets the listView
    else if(keyevent->key() == Qt::Key_Escape){
        ui->listObjs->setModel(albumModel);
        //ui->listObjs->setViewMode(QListView::IconMode);
        selAlbum = -1;
    }
    else if(keyevent->key() == Qt::Key_Enter
                                    || keyevent->key() == Qt::Key_Return){
        ui->searchBox->setFocus();
        search();
    }
    //Use the combo Ctrl+F to give focus to the search box
    if(keyevent->modifiers() == Qt::ControlModifier
                                    && keyevent->key() == Qt::Key_F){
        ui->searchBox->setFocus();
    }
    //Use the combo Ctrl+X to go to th next song
    if(keyevent->modifiers() == Qt::ControlModifier
                                    && keyevent->key() == Qt::Key_X){
        on_next_clicked();
    }
    //Use the combo Ctrl+Z to go to the previus song
    if(keyevent->modifiers() == Qt::ControlModifier
                                    && keyevent->key() == Qt::Key_Z){
        on_prev_clicked();
    }
}

///Change the list used in the the main listView and handle
///The presses
void MainWindow::changeList(){
    int sel=ui->listObjs->currentIndex().row();
    if (ui->listObjs->model() == searchResults){
        int al = searchResultsIds.at(2*sel);
        int id = searchResultsIds.at(2*sel+1);
        player->stop();
        playlist->clear();
        QString file = folder + QDir::separator() + QString::number(al) + QDir::separator() + QString::number(id);
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(albuns[al]->songs.at(id)->getName());
        ui->songArtist->setText(albuns[al]->songs.at(id)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[al]->getImagePath()));
        playList = false;
        play();
    }
    else if(ui->listObjs->model() == tempSong && selAlbum != -1){
        ///Ignore the selection if the number is higher than the
        ///Total songs in the selected album
        if(sel-2 >= albuns[selAlbum]->songs.count())
                return;
        if(sel == 0){///if the item pressed is the first "back" go back to
                     ///list of all the albums
            ui->listObjs->setModel(albumModel);
            //ui->listObjs->setViewMode(QListView::IconMode);
            selAlbum = -1;
            return;
        }
        if(sel == 1){///If the user presses the item with the album information
                     ///Open the edit form for that album
            edit->setData(2, albuns[selAlbum]);
            edit->exec();
            return;
        }
        player->stop();
        if(!playlist->isEmpty())
            playlist->clear();
        QString file =  folder + QDir::separator() + QString::number(selAlbum)
                + QDir::separator() + QString::number(sel-2) + ".mp3";
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(albuns[selAlbum]->songs.at(sel-2)->getName());
        ui->songArtist->setText(albuns[selAlbum]->songs.at(sel-2)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
        playList = false;
        play();
    }
    else if(sel >= albuns.count())
        return;
    else{
        ///Set the list used in the main listView as the list of songs of the
        ///Selected album
        tempSong->clear();
        selAlbum = sel;
        updateSongList(selAlbum);
        ui->listObjs->setModel(tempSong);
        //ui->listObjs->setViewMode(QListView::ListMode);
    }
}

/*!
 * \brief MainWindow::checkPlayLists Verificação de existência de playlists criadas
 */
void MainWindow::changePlayLists(){
    int sel=ui->listPlay->currentIndex().row();
    if (ui->listPlay->model() == playModel){
        selPlay = sel;
        updatePlaylistSongs(selPlay);
        ui->listPlay->setModel(tempPlay);
        //ui->listPlay->setViewMode(QListView::ListMode);
    }
    else if(ui->listPlay->model() == tempPlay && selPlay != -1){
        ///Ignore the selection if the number is higher than the
        ///Total songs in the selected album
        if(sel-1 >= playlists[selPlay]->getTot())
                return;
        if(sel == 0){///if the item pressed is the first "back" go back to
                     ///list of all the albums
            ui->listPlay->setModel(playModel);
            //ui->listPlay->setViewMode(QListView::ListMode);
            selPlay = -1;
            return;
        }
        sel--;
        player->stop();
        playlist->clear();
        QString file;
        for(int i = sel; i < playlists[selPlay]->getTot(); i++){
            QList<int> l = playlists[selPlay]->getSong(i);
            file = folder + QDir::separator() + QString::number(l[0])
                    + QDir::separator() + QString::number(l[1]) + ".mp3";
            playlist->addMedia(QUrl::fromLocalFile(file));
        }
        QList<int> l = playlists[selPlay]->getSong(sel);
        ui->songName->setText(albuns[l[0]]->songs.at(l[1])->getName());
        ui->songArtist->setText(albuns[l[0]]->songs.at(l[1])->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[l[0]]->getImagePath()));
        playList=true;
        activePlay = selPlay;
        play();
    }
}

///Show the new song form
void MainWindow::on_actionAdicionarMusica_triggered()
{
    if(selAlbum == -1){//If there isn't an album selected
        QMessageBox msg;
        //Warn the user and ask to create a nem album
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
    //Show the form
    addSong->exec();
}

///Show the new album form
void MainWindow::on_actionAdicionarAlbum_triggered()
{
    addAlbum->exec();
}

///Process the information given by the new album Form
///Create a new object Album and store the information
///On the database
void MainWindow::dialogAlbumFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = addAlbum->getInfo();
        if(list.count() < 2){//If the information is incomplete bring back the
            addAlbum->exec();//Form so the user can complete giving the
            return;          //Information
        }
        else if(list.count() == 3){//If the user gave the path to the image
            //Create the new object Album
            albuns.append(new Album(list[0], list[1], list[2]));
            //Insert the information in the database
            QSqlQuery query;
            query.prepare("INSERT INTO Albuns (Name, Description, ImagePath) "
                          "VALUES (:n, :d, :i)");
            query.bindValue(":n", list[0]);
            query.bindValue(":d", list[1]);
            query.bindValue(":i", list[2]);
            query.exec();
        }
        else{//Else use a placeHolder image
            /////////////////////TO BE ADDED
            //Create the new object Album
            albuns.append(new Album(list[0], list[1]));
            QSqlQuery query;
            //Insert the information in the database
            query.prepare("INSERT INTO Albuns (Name, Description, ImagePath) "
                          "VALUES (:n, :d, :i)");
            query.bindValue(":n", list[0]);
            query.bindValue(":d", list[1]);
            query.bindValue(":i", "");
            query.exec();
        }
        QDir().mkdir(folder + QDir::separator() +
                                        QString::number(albuns.count()-1));
        updateAlbumList();
        db.commit();
    }
}

///Process the information given by the new music Form
///Create a new object Musica in the correspondent Album
///And store the information on the database
void MainWindow::dialogMusicFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = addSong->getInfo();
        if(list[0] == "_batch_"){
            QString noInfo = "Sem informação";
            QList<QString> songs = list[1].split(";");
            QSqlQuery query;
            for(int i=0; i < songs.count();i++){
                 albuns[selAlbum]->addMusica(new Musica(noInfo,noInfo,noInfo));
                 //Insert the information in the database
                 QString aux = folder + QDir::separator() + QString::number(selAlbum)
                              + QDir::separator() + QString::number(i) + ".mp3";
                 QFile::copy(songs[i], aux);
                 query.prepare("INSERT INTO Musica (Name, Artists, "
                               " Genre, DateAdded, Album, ID, Playlist) VALUES "
                               "(:n, :a, :g, :d, :al, :i, :p)");
                 query.bindValue(":n", noInfo);
                 query.bindValue(":a", noInfo);
                 query.bindValue(":g", noInfo);
                 query.bindValue(":d", albuns[selAlbum]->songs.last()->getDate());
                 query.bindValue(":al", selAlbum);
                 query.bindValue(":i", albuns[selAlbum]->songs.count()-1);
                 query.bindValue(":p", "");
                 query.exec();
            }
            updateSongList(selAlbum);
            return;
        }
        if(list.count() < 3){//If the information is incomplete bring back the
            addSong->exec(); //Form so the user can complete giving the
            return;          //Information
        }
        else{
            QList<QString> l = list[1].split(", ");//Get the list of Artistas
            //list[2] = list[2].replace(folder, "");
            albuns[selAlbum]->addMusica(new Musica(list[0], l, list[3]));
            QString aux = folder + QDir::separator() + QString::number(selAlbum)
                    + QDir::separator() + QString::number(albuns[selAlbum]->songs.count()-1) + ".mp3";
            QFile::copy(list[2], aux);
            //Insert the information in the database
            QSqlQuery query;
            query.prepare("INSERT INTO Musica (Name, Artists,"
                          " Genre, DateAdded, Album, ID, Playlist) VALUES "
                          "(:n, :a, :g, :d, :al, :i, :p)");
            query.bindValue(":n", list[0]);
            query.bindValue(":a", list[1]);
            query.bindValue(":g", list[3]);
            query.bindValue(":d", albuns[selAlbum]->songs.last()->getDate());
            query.bindValue(":al", selAlbum);
            query.bindValue(":i", albuns[selAlbum]->songs.count()-1);
            query.bindValue(":p", "");
            query.exec();
        }
        updateSongList(selAlbum);
    }
}

///Process the information given by the editing Form
///Set the new information on the corresponding object and update the
///Database
void MainWindow::dialogEditFinished(int result){
    if(result == QDialog::Accepted){
        ///Get the data from the editing form
        QList<QString> list = edit->getData();
        int type = list[0].toInt();
        if(list[1] == "_rem_"){
            if(type == 1){
                removeMusica(selAlbum, idSong);
                return;
            }
            else if(type == 2){
                removeAlbum(selAlbum);
                ui->listObjs->setModel(albumModel);
                //ui->listObjs->setViewMode(QListView::IconMode);
                selAlbum = -1;
                return;
            }
            else if(type == 3){
                removePlaylist(selPlay);
                ui->listPlay->setModel(playModel);
                selPlay = -1;
                return;
            }
        }
        ///Get the type of object (1 for Musica, 2 for Album
        if(type == 1 && list.count() == 4){//case: Musica
            ///Update the information on the object
            albuns[selAlbum]->songs.at(idSong)->setNome(list[1]);
            albuns[selAlbum]->songs.at(idSong)->setArtistas(list[2]);
            albuns[selAlbum]->songs.at(idSong)->setGenero(list[3]);
            ///Update the information on the database
            QSqlQuery query;
            query.prepare("UPDATE Musica SET Name = :n, Artists = :a,"
                          "Genre = :g WHERE Album = :al AND ID = :i");
            query.bindValue(":n", list[1]);
            query.bindValue(":a", list[2]);
            query.bindValue(":g", list[3]);
            query.bindValue(":al", selAlbum);
            query.bindValue(":i", idSong);
            query.exec();
            menu = false;
        }
        else if(type == 2 && list.count() == 4){//case: Album
            ///Update the information on the object
            albuns[selAlbum]->setNome(list[1]);
            albuns[selAlbum]->setDescricao(list[2]);
            albuns[selAlbum]->setImagePath(list[3]);
            ///Update the information on the database
            QSqlQuery query;
            query.prepare("UPDATE Albuns SET Name = :n, Description = :d,"
                          "ImagePath = :i WHERE rowid = :sel");
            query.bindValue(":n", list[1]);
            query.bindValue(":d", list[2]);
            query.bindValue(":i", list[3]);
            query.bindValue(":sel", selAlbum+1);
            query.exec();
            if(menu)
                selAlbum = -1;
            menu = false;
        }
        else if(type == 3){//case: Playlists
            playlists[selPlay]->setNome(list[1]);
            playlists[selPlay]->setDescricao((list[2]));
            QSqlQuery query;
            query.prepare("UPDATE Playlists SET Name = :n, Description = :d,"
                          " WHERE rowid = :sel");
            query.bindValue(":n", list[1]);
            query.bindValue(":d", list[2]);
            query.bindValue(":sel", selPlay+1);
            query.exec();
        }
        else
            return;
    }
    ///Update the list to use in the main listView
    updatePlaylist();
    updateAlbumList();
    updateSongList(selAlbum);
}

///Update the list of Albums to show in the main listView
void MainWindow::updateAlbumList(){
    albumModel->clear();
    //List all the albums in stored
    QStandardItem *Items;
    for(int i=0;i<albuns.count();i++){
        Items = new QStandardItem(albuns[i]->getNome());
        Items->setIcon(QPixmap(albuns[i]->getImagePath()));
        albumModel->appendRow(Items);
    }
    ui->listObjs->resizeRowsToContents();
}

///
void MainWindow::updatePlaylist(){
    playModel->clear();
    QStandardItem *Items;
    for(int i=0;i<playlists.count();i++){
        Items = new QStandardItem(playlists[i]->getNome());
        playModel->appendRow(Items);
    }
    ui->listObjs->resizeRowsToContents();
}

///Update the list of songs to show in the main listView
void MainWindow::updateSongList(int alb){
    tempSong->clear();
    QStandardItem *Items;
    QList<QStandardItem*> listInfo;
    QList<QStandardItem*> list;
    //Item to go back to the mais screen
    Items = new QStandardItem("Back");
    tempSong->appendRow(Items);
    tempSong->setColumnCount(3);
    //Informations of the current Album
    int totSongs = albuns[alb]->songs.count();
    if(totSongs == 0){
        listInfo.append(new QStandardItem(albuns[alb]->getNome()));
        listInfo.append(new QStandardItem(albuns[alb]->getDescricao()));
        listInfo.append(new QStandardItem("Não tem músicas"));
        /*Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Não tem músicas");*/
    }
    else if(albuns[alb]->songs.count() == 1){
        listInfo.append(new QStandardItem(albuns[alb]->getNome()));
        listInfo.append(new QStandardItem(albuns[alb]->getDescricao()));
        listInfo.append(new QStandardItem("Tem uma música"));
        /*Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Tem uma música");*/
    }
    else{
        listInfo.append(new QStandardItem(albuns[alb]->getNome()));
        listInfo.append(new QStandardItem(albuns[alb]->getDescricao()));
        listInfo.append(new QStandardItem("Tem " + QString::number(totSongs) + " músicas"));
        /*Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Tem " + QString::number(totSongs) + " músicas");*/
    }
    Items->setIcon(QPixmap(albuns[alb]->getImagePath()));
    tempSong->appendRow(listInfo);

    //List the songs of the Album
    for(int i=0; i < totSongs; i++){
        list.clear();
        list.append(new QStandardItem(albuns[alb]->songs.at(i)->getName()));
        list.append(new QStandardItem(albuns[alb]->songs.at(i)->getArtistas()));
        list.append(new QStandardItem(albuns[alb]->songs.at(i)->getGenero()));

        /*Items = new QStandardItem(albuns[alb]->songs.at(i)->getName() + "\t"
                             + albuns[alb]->songs.at(i)->getArtistas()
                             + "\t" + albuns[alb]->songs.at(i)->getGenero());*/
        tempSong->appendRow(list);
    }
    ui->listObjs->resizeRowsToContents();
}

///Show the configuration form
void MainWindow::on_actionConfigura_o_triggered()
{
    conf->setData(folder);
    conf->exec();
}

//create db
void MainWindow::createDB(){
    QSqlQuery query;
    //Create the table for the albums
    query.exec("CREATE TABLE Albuns ("
               "Name        STRING,"
               "Description STRING,"
               "ImagePath   STRING"
           ")");
    //Create the table for the songs
    query.exec("CREATE TABLE Musica ("
               "Name      STRING,"
               "Artists   STRING,"
               "Genre     STRING,"
               "DateAdded DATETIME,"
               "Album     INTEGER,"
               "ID        INTEGER,"
               "Playlist  STRING"
           ")");
    //Create the table for the playlists
    query.exec("CREATE TABLE Playlists ("
               "Name        STRING,"
               "Description STRING"
           ")");
    //Create table for the settings
    query.exec("CREATE TABLE Settings ("
               "Path        STRING"
           ")");

    folder = "C:\\songs";
    query.prepare("INSERT INTO Settings (Path) VALUES (:f)");
    query.bindValue(":f", folder);
    query.exec();
}

///This function will read the information stored in the SQLite database
///So the information can be stored from different sessions of the program
///The information is read from de database and then the corresponding
///Object is created with the iformation
void MainWindow::readFromDB(){
    QSqlQuery query;
    QSqlRecord record;
    QString name, desc, imFile;
    QString art,gen;
    int id;
    QDate date;

    //At first the information of all the albums stored in the Player
    query.exec("SELECT * FROM Albuns");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        desc = record.value(1).toString();
        imFile = record.value(2).toString();
        albuns.append(new Album(name, desc, imFile));
    }
    query.clear();

    //Then the information of the playlists
    query.exec("SELECT * FROM Playlists");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        desc = record.value(1).toString();
        if(!name.isEmpty() && !desc.isEmpty())
            playlists.append(new Playlist(name, desc));
    }
    query.clear();

    //And the last step is to restore all the songs
    query.exec("SELECT * FROM Musica");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        art = record.value(1).toString();
        gen = record.value(3).toString();
        date = record.value(4).toDate();
        id = record.value(5).toInt();
        QString pl = record.value(7).toString();
        QList<QString> l;
        if(!pl.isEmpty())
            l=pl.split(",");
        ///Add to the album
        albuns.at(id)->addMusica(new Musica(name,art,gen,date));
        ///Add to the corresponding playlists
        for(int i = 0; i < l.count(); i++){
            if(!l[i].isEmpty())
                playlists[l[i].toInt()]->addSong(id, albuns.at(id)->songs.count()-1);
        }
    }
    query.exec("SELECT * FROM Settings");
    query.next();
    record = query.record();
    folder = record.value(0).toString();
}
/*!
 * \brief MainWindow::reloadPlaylists Carrega playlists
 */
void MainWindow::reloadPlaylists(){
    playlists.clear();
    QSqlQuery query;
    QSqlRecord record;
    QString name, desc;
    //Then the information of the playlists
    query.exec("SELECT * FROM Playlists");
    while (query.next()) {
        record = query.record();
        name = record.value(0).toString();
        desc = record.value(1).toString();
        if(!name.isEmpty() && !desc.isEmpty())
            playlists.append(new Playlist(name, desc));
    }
}

///search
void MainWindow::search(){
    QString se = ui->searchBox->text();
    if(se.isEmpty()){
        ui->Viewer->setTabEnabled(2,false);
        selAlbum = -1;
        searchResultsIds.clear();
        return;
    }
    if(se.length() < 2)
        return;
    ui->Viewer->setTabEnabled(2,true);
    QSqlQuery query;
    QSqlRecord record;
    QList<QStandardItem*> list;
    int al, id;

    query.prepare("SELECT * FROM Musica WHERE Name LIKE :st"
                                                " OR Artists LIKE :st"
                                                " OR DateAdded LIKE :st");

    se = "%"+se+"%";
    query.bindValue(":st", se);
    query.exec();
    searchResults->clear();
    searchResultsIds.clear();
    while (query.next()){
        record = query.record();
        al = record.value(5).toInt();
        id = record.value(6).toInt();
        if(al < 0 || id < 0)
            return;
        list.clear();
        list.append(new QStandardItem(albuns[al]->songs.at(id)->getName()));
        list.append(new QStandardItem(albuns[al]->songs.at(id)->getArtistas()));
        list.append(new QStandardItem(albuns[al]->songs.at(id)->getGenero()));
        list.append(new QStandardItem("in the album " + albuns[al]->getNome()));

        searchResults->appendRow(list);
        searchResultsIds.append(al);
        searchResultsIds.append(id);
    }
    ui->Viewer->setCurrentIndex(2);
    ui->listResults->setModel(searchResults);
}

///Create the context menu for the main listView
void MainWindow::ProvideContextMenu(const QPoint &pos){
    QPoint item = ui->listObjs->mapToGlobal(pos);
    QMenu submenu;
    //If the right click is inside an existing item create the complete menu
    if(ui->listObjs->indexAt(pos).isValid()){
        submenu.addAction("Play");
        submenu.addAction("Adicionar a playlist");
        submenu.addAction("Editar");
        submenu.addAction("Remover");
    }
    else{//Else only give an option no add a new element
        submenu.addAction("Adicionar");
    }

    QAction* rightClickItem = submenu.exec(item);

    //Handle the user chosen option according to the active model
    if(ui->listObjs->model() == albumModel){//If it's the list of albums
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //The play option plays the entire album
            selAlbum = ui->listObjs->indexAt(pos).row();
            player->stop();
            activeAlbum = selAlbum;
            QString file;
            playlist->clear();
            for(int i = 0; i < albuns[selAlbum]->songs.count(); i++){
                file = folder + QDir::separator() + QString::number(selAlbum)
                        + QDir::separator() + QString::number(i) + ".mp3";
                playlist->addMedia(QUrl::fromLocalFile(file));
            }
            ui->songName->setText(albuns[selAlbum]->songs.first()->getName());
            ui->songArtist->setText(albuns[selAlbum]->songs.first()->getArtistas());
            ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
            playList = false;
            play();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar a playlist")){
            //The add option adds the album to a playlist
            selAlbum = ui->listObjs->indexAt(pos).row();
            if(idSong < 0)
                return;
            QList<QString> pl;
            for(int i = 0; i < playlists.count(); i++){
                pl.append(playlists[i]->getNome());
            }
            if(pl.count() == 0)
                return;
            QSqlQuery query;
            QString aux = QInputDialog::getItem(this, "Escolha a playlist", "Playlists", pl);
            int id = pl.indexOf(aux);
            for(int i = 0; i < albuns[selAlbum]->songs.count(); i++){
                playlists[id]->addSong(selAlbum, i);
                query.prepare("UPDATE Musica SET Playlist = Playlist || :p"
                                               " WHERE Album = :a AND ID = :i");
                query.bindValue(":p", QString::number(id)+",");
                query.bindValue(":a", selAlbum);
                query.bindValue(":i", i);
                query.exec();
            }
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            //The edit option open the edit form for the selected album
            selAlbum = ui->listObjs->indexAt(pos).row();
            edit->setData(2, albuns[selAlbum]);
            edit->exec();
            menu=true;
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Remover")){
            //The remove option removes the album from the program
            int alb = ui->listObjs->indexAt(pos).row();
            removeAlbum(alb);
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            addAlbum->exec();
            return;
        }

    }
    else if(ui->listObjs->model() == tempSong){//If it's the list of songs
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //The play option plays the song
            idSong = ui->listObjs->indexAt(pos).row()-2;
            if(idSong < 0){
                player->stop();
                activeAlbum = selAlbum;
                QString file;
                playlist->clear();
                for(int i = 0; i < albuns[selAlbum]->songs.count(); i++){
                    file = folder + QDir::separator() + QString::number(selAlbum)
                            + QDir::separator() + QString::number(i) + ".mp3";
                    playlist->addMedia(QUrl::fromLocalFile(file));
                }
                ui->songName->setText(albuns[selAlbum]->songs.first()->getName());
                ui->songArtist->setText(albuns[selAlbum]->songs.first()->getArtistas());
                ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
                playList = false;
                play();
            }
            else{
                player->stop();
                QString file;
                playlist->clear();
                file = folder + QDir::separator() + QString::number(selAlbum)
                        + QDir::separator() + QString::number(idSong) + ".mp3";
                playlist->addMedia(QUrl::fromLocalFile(file));
                ui->songName->setText(albuns[selAlbum]->songs.at(idSong)->getName());
                ui->songArtist->setText(albuns[selAlbum]->songs.at(idSong)->getArtistas());
                ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
                play();
            }
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar a playlist")){
            //The add option adds the song to a playlist
            idSong = ui->listObjs->indexAt(pos).row();
            if(idSong == 1){//add the entire album
                QList<QString> pl;
                for(int i = 0; i < playlists.count(); i++){
                    pl.append(playlists[i]->getNome());
                }
                if(pl.count() == 0)
                    return;
                QSqlQuery query;
                QString aux = QInputDialog::getItem(this, "Escolha a playlist", "Playlists", pl);
                int id = pl.indexOf(aux);
                for(int i = 0; i < albuns[selAlbum]->songs.count(); i++){
                    playlists[id]->addSong(selAlbum, i);
                    query.prepare("UPDATE Musica SET Playlist = Playlist || :p"
                                                   " WHERE Album = :a AND ID = :i");
                    query.bindValue(":p", QString::number(id)+",");
                    query.bindValue(":a", selAlbum);
                    query.bindValue(":i", i);
                    query.exec();
                }
                return;
            }
            idSong -= 2;
            if(idSong < 0)
                return;
            QList<QString> pl;
            for(int i = 0; i < playlists.count(); i++){
                pl.append(playlists[i]->getNome());
            }
            QSqlQuery query;
            QString aux = QInputDialog::getItem(this, "Escolha a playlist", "Playlists", pl);
            int id = pl.indexOf(aux);
            playlists[id]->addSong(selAlbum, idSong);
            query.prepare("UPDATE Musica SET Playlist = Playlist || :p"
                                           " WHERE Album = :a AND ID = :i");
            query.bindValue(":p", QString::number(id)+",");
            query.bindValue(":a", selAlbum);
            query.bindValue(":i", idSong);
            query.exec();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            idSong = ui->listObjs->indexAt(pos).row();
            if(idSong == 1){
                //The edit option open the edit form for the active album
                edit->setData(2, albuns[selAlbum]);
                edit->exec();
                menu=true;
                return;
            }
            if(idSong>=2){
                //The edit option open the edit form for the selected song
                idSong -= 2;
                edit->setData(1, albuns[selAlbum]->songs[idSong]);
                edit->exec();
                menu=true;
                return;
            }
            else return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Remover")){
            //The remove option removes the album from the program
            int song = ui->listObjs->indexAt(pos).row();
            if(song == 1){
                removeAlbum(selAlbum);
                return;
            }
            song -= 2;
            if(song < 0)
                return;
            removeMusica(selAlbum, song);
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            addSong->exec();
            return;
        }
    }
}
/*!
 * \brief MainWindow::ProvideContextMenuPlay
 * \param pos
 */
void MainWindow::ProvideContextMenuPlay(const QPoint &pos){
    QPoint item = ui->listPlay->mapToGlobal(pos);
    QMenu submenu;
    //If the right click is inside an existing item create the complete menu
    if(ui->listPlay->indexAt(pos).isValid()){
        submenu.addAction("Play");
        submenu.addAction("Adicionar");
        submenu.addAction("Editar");
        submenu.addAction("Remover");
    }
    else{//Else only give an option no add a new element
        submenu.addAction("Adicionar");
    }

    QAction* rightClickItem = submenu.exec(item);

    //Handle the user chosen option according to the active model
    if(ui->listPlay->model() == playModel){//If it's the list of albums
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //The play option plays the entire playlist
            selPlay = ui->listPlay->indexAt(pos).row();
            player->stop();
            activePlay = selPlay;
            QString file;
            playlist->clear();
            QList<int> l;
            for(int i = 0; i < playlists[selPlay]->getTot(); i++){
                l = playlists[selPlay]->getSong(i);
                file = folder + QDir::separator() + QString::number(l[0])
                        + QDir::separator() + QString::number(l[1]) + ".mp3";
                playlist->addMedia(QUrl::fromLocalFile(file));
            }
            l = playlists[selPlay]->getSong(0);
            ui->songName->setText(albuns[l[0]]->songs[l[1]]->getName());
            ui->songArtist->setText(albuns[l[0]]->songs[l[1]]->getArtistas());
            ui->albumImage->setPixmap(QPixmap(albuns[l[0]]->getImagePath()));
            playList = true;
            activePlay = selPlay;
            play();
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            //The add option opens the form to add an album
            addPlay->exec();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            //The edit option open the edit form for the selected album
            selPlay = ui->listPlay->indexAt(pos).row();
            edit->setData(3, playlists[selPlay]);
            edit->exec();
            menu=true;
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Remover")){
            //The remove option removes the album from the program
            int pl = ui->listPlay->indexAt(pos).row();
            removePlaylist(pl);
        }

    }
    else if(ui->listPlay->model() == tempPlay){//If it's the list of songs
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //Play entire playlist from song
            int sel = ui->listPlay->indexAt(pos).row()-2;
            player->stop();
            playlist->clear();
            QString file;
            for(int i = 0; i < playlists[selPlay]->getTot(); i++){
                QList<int> l = playlists[selPlay]->getSong(i);
                file = folder + QDir::separator() + QString::number(l[0])
                        + QDir::separator() + QString::number(l[1]) + ".mp3";
                playlist->addMedia(QUrl::fromLocalFile(file));
            }
            QList<int> l = playlists[selPlay]->getSong(sel);
            ui->songName->setText(albuns[l[0]]->songs.at(l[1])->getName());
            ui->songArtist->setText(albuns[l[0]]->songs.at(l[1])->getArtistas());
            ui->albumImage->setPixmap(QPixmap(albuns[l[0]]->getImagePath()));
            playlist->setCurrentIndex(sel);
            playList=true;
            activePlay = selPlay;
            play();
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            //The add option opens the form to add a song
            QList<QString> songs;
            QList<int> id;
            for(int i = 0; i < albuns.count() ; i++){
                for(int j = 0; j < albuns[i]->songs.count(); j++){
                    songs.append(albuns[i]->songs[j]->getName() + "   " +
                                 albuns[i]->songs[j]->getArtistas());
                    id.append(i);
                    id.append(j);
                }
            }
            QSqlQuery query;
            QString aux = QInputDialog::getItem(this, "Escolha a música a dicionar", "Lista de músicas", songs);
            int loc = songs.indexOf(aux);
            playlists[selPlay]->addSong(id[loc*2], id[loc*2+1]);
            query.prepare("UPDATE Musica SET Playlist = Playlist || :p"
                                           " WHERE Album = :a AND ID = :i");
            query.bindValue(":p", QString::number(selPlay)+",");
            query.bindValue(":a", id[loc*2]);
            query.bindValue(":i", id[loc*2+1]);
            query.exec();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            idSong = ui->listPlay->indexAt(pos).row();
            if(idSong == 1){
                //The edit option open the edit form for the active album
                edit->setData(2, albuns[selAlbum]);
                edit->exec();
                menu=true;
                return;
            }
            if(idSong>=2){
                //The edit option open the edit form for the selected song
                idSong -= 2;
                edit->setData(1, albuns[selAlbum]->songs[idSong]);
                edit->exec();
                menu=true;
                return;
            }
            else return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Remover")){
            //The remove option removes the album from the program
            int song = ui->listObjs->indexAt(pos).row()-2;
            if(song < 0)
                return;//only accept the songs
            removeMusica(selAlbum, song);
        }
    }
}

///Removes a song from an album
/// @param alb an interger that indicates the album
/// @param song an interger that indicates the song
bool MainWindow::removeMusica(int alb, int song){
    if (!confirmed){
        int reply = QMessageBox::question(this,
                                    "Remover",
                                    "Tem a certeza que deseja remover a musica "
                                    + this->albuns[alb]->songs[song]->getName(),
                                    QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No)
            return false;
    }
    //Removes the song from the program
    albuns[alb]->songs.removeAt(song);
    //Updates the list of songs
    updateSongList(alb);
    //Updates the database
    QSqlQuery query;
    query.prepare("SELECT * FROM Musica WHERE Album =:a AND ID =:i");
    query.bindValue(":a", alb);
    query.bindValue(":i", song);
    query.exec();
    query.next();
    if(!query.record().value(7).toString().isEmpty()){
        reloadPlaylists();
    }
    query.prepare("DELETE FROM Musica WHERE Album =:a AND ID =:i");
    query.bindValue(":a", alb);
    query.bindValue(":i", song);
    bool res = query.exec();
    query.clear();
    //Update the rest of the songs in the database
    query.prepare("UPDATE Musica SET ID = ID - 1 WHERE Album =:a AND ID > :i");
    query.bindValue(":a", alb);
    query.bindValue(":i", song);
    query.exec();
    if (!confirmed){
        //remove file
        QString aux = folder + QDir::separator() + QString::number(alb)
                           + QDir::separator() + QString::number(song) + ".mp3";
        QString old;
        QFile::remove(aux);
        //update filenames
        for (int i=song;i<albuns[alb]->songs.count();i++){
            aux = folder + QDir::separator() + QString::number(alb)
                              + QDir::separator() + QString::number(i) + ".mp3";
            old = folder + QDir::separator() + QString::number(alb)
                            + QDir::separator() + QString::number(i+1) + ".mp3";
            QFile::rename(old, aux);
        }
    }
    return res;
}

///Removes an album
/// @param alb an interger that indicates the album
bool MainWindow::removeAlbum(int alb){
    int reply = QMessageBox::question(this,
                                "Remover",
                                "Tem a certeza que deseja remover o album "
                                + this->albuns[alb]->getNome(),
                                QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;
    reply = QMessageBox::question(this,
                                "Remover",
                                "Todas as musicas iram ser removidas da aplicação",
                                QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;
    else{
        confirmed = true;
    }
    //Save the name and description
    QString auxN, auxD;
    auxN = albuns[alb]->getNome();
    auxD = albuns[alb]->getDescricao();
    int tot = albuns[alb]->songs.count()-1;
    for(int i = tot; i >= 0; i--){
        //Removes all the songs from the album
        removeMusica(alb, i);
    }
    confirmed = false;
    //Removes the album from the program
    albuns.removeAt(alb);

    //remove file
    QString aux = folder + QDir::separator() + QString::number(alb);
    QString old;
    QDir(aux).removeRecursively();
    QDir dir;
    //update filenames
    for (int i=alb+1;i<albuns.count()+1;i++){
        aux = folder + QDir::separator() + QString::number(i-1);
        old = folder + QDir::separator() + QString::number(i);
        dir.rename(old,aux);
    }

    //update the album list
    updateAlbumList();
    //Update the database
    QSqlQuery query;
    //Use the name and description to delete the album from the database
    query.prepare("DELETE FROM Albuns WHERE Name = :n AND Description = :d");
    query.bindValue(":n", auxN);
    query.bindValue(":d", auxD);
    bool res = query.exec();
    query.clear();
    //Update the rest of the songs in the database
    query.prepare("UPDATE Musica SET Album = Album - 1 WHERE Album > :a");
    query.bindValue(":a", alb);
    query.exec();
    //Reload playlists to fix the album id
    reloadPlaylists();
    return res;
}
///Removes a song from an album
/// @param p an interger that indicates the playlist
/// @param id an interger that indicates the song
bool MainWindow::removeFromPlaylist(int p, int id){
    QSqlQuery query;
    QList<int> l = playlists[p]->getSong(id);
    query.prepare("UPDATE Musica SET Playlist = replace(Playlist, :pl, '')"
                  "WHERE Album = :a AND ID = :i");
    query.bindValue(":pl", QString::number(p)+",");
    query.bindValue("a", l[0]);
    query.bindValue("i", l[1]);
    bool res = query.exec();
    playlists[p]->removeSong(id);
    updatePlaylistSongs(selPlay);
    return res;
}

void MainWindow::updatePlaylistSongs(int p){
    tempPlay->clear();
    QStandardItem *Items;
    QList<QStandardItem*>list;
    QList<int> l;
    //Item to go back to the mais screen
    Items = new QStandardItem("Back");
    tempPlay->appendRow(Items);
    //List the songs of the Album
    int totSongs = playlists[p]->getTot();
    for(int i=0; i < totSongs; i++){
        list.clear();
        l = playlists[p]->getSong(i);
        list.append(new QStandardItem(albuns[l[0]]->songs.at(l[1])->getName()));
        list.append(new QStandardItem(albuns[l[0]]->songs.at(l[1])->getArtistas()));
        list.append(new QStandardItem(albuns[l[0]]->songs.at(l[1])->getGenero()));
        tempPlay->appendRow(list);
    }
}

/*!
 * \brief MainWindow::removePlaylist Remove playlist
 * \param p
 * \return
 */
bool MainWindow::removePlaylist(int p){
    QSqlQuery query;
    //Use the name and description to delete the album from the database
    query.prepare("DELETE FROM Playlists WHERE Name = :n AND Description = :d");
    query.bindValue(":n", playlists[p]->getNome());
    query.bindValue(":d", playlists[p]->getDescricao());
    bool res = query.exec();
    for(int i = 0; i < playlists[p]->getTot(); i++){
        QList<int> l = playlists[p]->getSong(i);
        query.prepare("UPDATE Musica SET Playlist = replace(Playlist, :pl, '')"
                      "WHERE Album = :a AND ID = :i");
        query.bindValue(":pl", QString::number(p)+",");
        query.bindValue("a", l[0]);
        query.bindValue("i", l[1]);
        query.exec();
    }
    playlists.removeAt(p);
    updatePlaylist();
    return res;
}
/*!
 * \brief MainWindow::on_next_clicked
 */
void MainWindow::on_next_clicked()
{
    if(playlist->nextIndex() == -1){
        return;
    }
    int i = playlist->currentIndex();
    playlist->next();
    int j = playlist->currentIndex();
    if(i == j && playlist->playbackMode() == QMediaPlaylist::Random)
        on_next_clicked();
}
/*!
 * \brief MainWindow::on_prev_clicked
 */
void MainWindow::on_prev_clicked()
{
    if(playlist->nextIndex() == -1){
        return;
    }
    playlist->previous();
}
/*!
 * \brief MainWindow::on_repetir_clicked
 */
void MainWindow::on_repetir_clicked()
{
    int rep = playlist->playbackMode();
    if(rep == QMediaPlaylist::CurrentItemOnce){
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->repetir->setText("Repetir uma");
    }
    else if(rep == QMediaPlaylist::CurrentItemInLoop){
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        ui->repetir->setText("Repetir Todas");
    }
    else if(rep == QMediaPlaylist::Loop){
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        ui->repetir->setText("Não repetir");
    }
    else if(rep == QMediaPlaylist::Random){}
    else
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
}
/*!
 * \brief MainWindow::changeInfo
 */
void MainWindow::changeInfo(){
    int id = playlist->currentIndex();
    if(id <= 0){
        return;
    }
    if(playList){
        if(id < 0 || id >= playlists[activePlay]->getTot())
            return;
        QList<int> l = playlists[activePlay]->getSong(id);
        ui->songName->setText(albuns[l[0]]->songs.at(l[1])->getName());
        ui->songArtist->setText(albuns[l[0]]->songs.at(l[1])->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[l[0]]->getImagePath()));
    }
    else{
        ui->songName->setText(albuns[activeAlbum]->songs.at(id)->getName());
        ui->songArtist->setText(albuns[activeAlbum]->songs.at(id)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[activeAlbum]->getImagePath()));
    }
}
/*!
 * \brief MainWindow::on_aleatorio_clicked Ativado o modo aleatório
 */
void MainWindow::on_aleatorio_clicked()
{
    int rep = playlist->playbackMode();
    if(rep != QMediaPlaylist::Random){
        prevMode = playlist->playbackMode();
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        ui->aleatorio->setText("Aleatório");
    }
    else{
        switch (prevMode) {
        case 0:
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
            break;
        case 1:
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        case 3:
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
        default:
            break;
        }
        ui->aleatorio->setText("Sequencial");
    }
}
/*!
 * \brief MainWindow::on_btnAddPlay_clicked Botão de reprodução
 */
void MainWindow::on_btnAddPlay_clicked()
{
    addPlay->exec();
}
/*!
 * \brief MainWindow::dialogPlayListFinished
 * \param result
 */
void MainWindow::dialogPlayListFinished(int result){
    if(result == QDialog::Accepted){
        QList<QString> list = addPlay->getInfo();
        if(list.count() < 2){//If the information is incomplete bring back the
            addPlay->exec();//Form so the user can complete giving the
            return;          //Information
        }
        else{//If the user gave the path to the image
            //Create the new object Album
            playlists.append(new Playlist(list[0], list[1]));
            //Insert the information in the database
            QSqlQuery query;
            query.prepare("INSERT INTO Playlists (Name, Description) "
                          "VALUES (:n, :d)");
            query.bindValue(":n", list[0]);
            query.bindValue(":d", list[1]);
            query.exec();
        }
        updatePlaylist();
        db.commit();
        ui->listPlay->setModel(playModel);
    }
}

void MainWindow::dialogConfigFinished(int result){
    if(result == QDialog::Accepted){
        QString dir = conf->getData();
        dir.replace("/", "\\");
        if (!QString::compare(folder, dir)){
            return;
        }

        //Copy files to new directory
        int res = QMessageBox::warning(this, "Mudança de directoria"
                      , "Todos os ficheiros vão ser movidos para a nova directoria.\nEste processo pode ser demorado",
                             QMessageBox::Yes | QMessageBox::No);
        if(res == QMessageBox::No){
            QMessageBox::information(this, "Mudança de directoria"
                                  , "A mudança foi anulada");
            return;
        }
        /*QMessageBox copy;
        copy.setWindowTitle("A mover os ficheiros");
        copy.setText("Aguarde um pouco");
        copy.setIcon(QMessageBox::Information);
        //copy.setStandardButtons(QMessageBox::NoButton);
        //copy.setWindowModality(Qt::ApplicationModal);
        QProgressDialog copy;
        copy.setWindowTitle("A mover os ficheiros");
        copy.setText("Aguarde um pouco");
        copy.setIcon(QMessageBox::Information);
        copy.show();*/
        QString src, dest, file;
        for(int i = 0; i< albuns.count();i++){
            src = folder+QDir::separator()+QString::number(i);
            dest = dir+QDir::separator()+QString::number(i);
            QDir().mkdir(dest);
            for(int j = 0; j< albuns[i]->songs.count();j++){
                file = QDir::separator()+QString::number(j)+".mp3";
                QFile::copy(src+file, dest+file);
            }
        }
        QDir(folder).removeRecursively();
        //copy.hide();

        folder = dir;
        QSqlQuery query;
        query.prepare("UPDATE Settings SET Path = (:f)");
        query.bindValue(":f", folder);
        query.exec();
    }
}

void MainWindow::order(){
    int choice = ui->orderChoice->currentIndex();
}
