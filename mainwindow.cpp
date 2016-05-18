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
    if (!db.open()){
        QMessageBox::information(this, "Alerta"
                                , "Não se consegue connectar a base de dados!");
        //createDB();
    }

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


    ///////
    ui->listObjs->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(ui->listObjs,SIGNAL(customContextMenuRequested(const QPoint &)),
       this,SLOT(ProvideContextMenu(const QPoint &)));

    ///////



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
    connect(ui->searchBox, SIGNAL(textChanged(QString)), this, SLOT(search()));
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

//Handle the keyboard press event
void MainWindow::keyPressEvent(QKeyEvent *keyevent){
    //Media Play/Pause and the space key toggle the play status
    if(keyevent->key() == Qt::Key_MediaTogglePlayPause
                                    || keyevent->key() == Qt::Key_Space){
        play();
    }
    //Esc key resets the listView
    else if(keyevent->key() == Qt::Key_Escape){
        ui->listObjs->setModel(albumModel);
        ui->listObjs->setViewMode(QListView::IconMode);
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
}

///Change the list used in the the main listView and handle
///The presses
void MainWindow::changeList(){
    int sel=ui->listObjs->currentIndex().row();
    if (ui->listObjs->model() == searchResults){
        int al = searchResultsIds.at(2*sel);
        int id = searchResultsIds.at(2*sel+1);
        player->stop();
        if(playlist->mediaCount() > 0)
            playlist->removeMedia(playlist->mediaCount()-1);
        QString file = folder + albuns[al]->songs.at(id)->getFileName();
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(albuns[al]->songs.at(id)->getName());
        ui->songArtist->setText(albuns[al]->songs.at(id)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[al]->getImagePath()));
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
            ui->listObjs->setViewMode(QListView::IconMode);
            selAlbum = -1;
            editing = false;
            ui->editSong->hide();
            return;
        }
        if(sel == 1){///If the user presses the item with the album information
                     ///Open the edit form for that album
            edit->setData(2, albuns[selAlbum]);
            edit->exec();
            editing = false;
            return;
        }
        if(editing){///If the user presses any other song after pressing
                    ///The button "Editar Musica" open the edit form for
                    ///That song
            edit->setData(1, albuns[selAlbum]->songs.at(sel-2));
            edit->exec();
            editing = false;
            idSong = sel-2;
            return;
        }
        player->stop();
        if(playlist->mediaCount() > 0)
            playlist->removeMedia(playlist->mediaCount()-1);
        QString file = folder + albuns[selAlbum]->songs.at(sel-2)->getFileName();
        playlist->addMedia(QUrl::fromLocalFile(file));
        ui->songName->setText(albuns[selAlbum]->songs.at(sel-2)->getName());
        ui->songArtist->setText(albuns[selAlbum]->songs.at(sel-2)->getArtistas());
        ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
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
        ui->listObjs->setViewMode(QListView::ListMode);
        ui->editSong->show();
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
        if(list.count() < 3){//If the information is incomplete bring back the
            addSong->exec(); //Form so the user can complete giving the
            return;          //Information
        }
        else{
            QList<QString> l = list[1].split(", ");//Get the list of Artistas
            /*for(int i = 0;i < artistas.count();i++){
                for(int j = 0;j < l.count(); j++){
                    if(!artistas[i].toLower()==l[j].toLower())
                        artistas.append(l[j]);
                }
            }*/
            list[2] = list[2].replace(folder, "");
            albuns[selAlbum]->addMusica(new Musica(list[0], list[2],
                                                                   l, list[3]));
            //Insert the information in the database
            QSqlQuery query;
            query.prepare("INSERT INTO Musica (Name, Artists, FilePath,"
                          " Genre, DateAdded, Album, ID) VALUES "
                          "(:n, :a, :f, :g, :d, :al, :i)");
            query.bindValue(":n", list[0]);
            query.bindValue(":a", list[1]);
            query.bindValue(":f", list[2]);
            query.bindValue(":g", list[3]);
            query.bindValue(":d", albuns[selAlbum]->songs.last()->getDate());
            query.bindValue(":al", selAlbum);
            query.bindValue(":i", albuns[selAlbum]->songs.count()-1);
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
                ui->listObjs->setViewMode(QListView::IconMode);
                selAlbum = -1;
                editing = false;
                ui->editSong->hide();
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
                          "Genre = :g WHERE rowid = :sel");
            query.bindValue(":n", list[1]);
            query.bindValue(":a", list[2]);
            query.bindValue(":g", list[3]);
            query.bindValue(":sel", idSong+1);
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
        else
            return;
    }
    ///Update the list to use in the main listView
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
}

///Update the list of songs to show in the main listView
void MainWindow::updateSongList(int alb){
    tempSong->clear();
    QStandardItem *Items;
    //Item to go back to the mais screen
    Items = new QStandardItem("Back");
    tempSong->appendRow(Items);
    //Informations of the current Album
    int totSongs = albuns[alb]->songs.count();
    if(totSongs == 0){
        Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Não tem músicas");
    }
    else if(albuns[alb]->songs.count() == 1){
        Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Tem uma música");
    }
    else{
        Items = new QStandardItem(albuns[alb]->getNome()+ "\t\t"
                             + albuns[alb]->getDescricao() + "\t\t"
                             + "Tem " + totSongs + " músicas");
    }
    Items->setIcon(QPixmap(albuns[alb]->getImagePath()));
    tempSong->appendRow(Items);
    //List the songs of the Album
    for(int i=0; i < totSongs; i++){
        Items = new QStandardItem(albuns[alb]->songs.at(i)->getName() + "\t"
                             + albuns[alb]->songs.at(i)->getArtistas()
                             + "\t" + albuns[alb]->songs.at(i)->getGenero());
        tempSong->appendRow(Items);
    }
}

void MainWindow::on_editSong_clicked()
{
    //Set edditing mode
    editing = true;
    //When a song is clicked next the edditing form will be presented
    QMessageBox::information(this, "", "Selecione a musica a editar");
}

///Show the configuration form
void MainWindow::on_actionConfigura_o_triggered()
{
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
               "FilePath  STRING,"
               "Genre     STRING,"
               "DateAdded DATETIME,"
               "Album     INTEGER,"
               "ID        INTEGER,"
               "Playlist  STRING"
           ")");
    //Create the table for the playlists
    query.exec("CREATE TABLE Playlists ("
    		   "ID          INTEGER,"
               "Name        STRING,"
               "Description STRING,"
           ")");
}

///This function will read the information stored in the SQLite database
///So the information can be stored from different sessions of the program
///The information is read from de database and then the corresponding
///Object is created with the iformation
void MainWindow::readFromDB(){
    QSqlQuery query;
    QSqlRecord record;
    QString name, desc, imFile;
    QString art, file, gen;
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
    /*query.exec("SELECT * FROM Playlists");
    while (query.next()) {

    }
    query.clear();*/

    //And the last step is to restore all the songs
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

///search
void MainWindow::search(){
    QString se = ui->searchBox->text();
    if(se.isEmpty()){
        ui->listObjs->setModel(albumModel);
        ui->listObjs->setViewMode(QListView::IconMode);
        selAlbum = -1;
        searchResultsIds.clear();
        return;
    }
    QSqlQuery query;
    QSqlRecord record;
    int al, id;
    QStandardItem *Items;
    query.prepare("SELECT * FROM Musica WHERE Name LIKE :s"
                                                " OR Artists LIKE :s"
                                                " OR DateAdded LIKE :s");

    se = "%"+se+"%";
    query.bindValue(":s", se);
    query.exec();
    searchResults->clear();
    searchResultsIds.clear();
    while (query.next()){
        record = query.record();
        al = record.value(5).toInt();
        id = record.value(6).toInt();
        record = query.record();
        Items = new QStandardItem(albuns[al]->songs.at(id)->getName() + "\t"
                            + albuns[al]->songs.at(id)->getArtistas()
                            + "\t" + albuns[al]->songs.at(id)->getGenero()
                            + "\t in the album " + albuns[al]->getNome());
        searchResults->appendRow(Items);
        searchResultsIds.append(al);
        searchResultsIds.append(id);
    }
    ui->listObjs->setModel(searchResults);
    ui->listObjs->setViewMode(QListView::ListMode);
}

///Create the context menu for the main listView
void MainWindow::ProvideContextMenu(const QPoint &pos){
    QPoint item = ui->listObjs->mapToGlobal(pos);
    QMenu submenu;
    //If the right click is inside an existing item create the complete menu
    if(ui->listObjs->indexAt(pos).isValid()){
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
    if(ui->listObjs->model() == albumModel){//If it's the list of albums
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //The play option plays the entire album
            selAlbum = ui->listObjs->indexAt(pos).row();
            player->stop();
            QString file;
            playlist->clear();
            for(int i = 0; i < albuns[selAlbum]->songs.count(); i++){
                file = folder + albuns[selAlbum]->songs.at(i)->getFileName();
                playlist->addMedia(QUrl::fromLocalFile(file));
            }
            ui->songName->setText(albuns[selAlbum]->songs.first()->getName());
            ui->songArtist->setText(albuns[selAlbum]->songs.first()->getArtistas());
            ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
            play();
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            //The add option opens the form to add an album
            addAlbum->exec();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            //The edit option open the edit form for the selected album
            selAlbum = ui->listObjs->indexAt(pos).row();
            edit->setData(2, albuns[selAlbum]);
            edit->exec();
            editing = false;
            menu=true;
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Remover")){
            //The remove option removes the album from the program
            int alb = ui->listObjs->indexAt(pos).row();
            removeAlbum(alb);
        }

    }
    else if(ui->listObjs->model() == tempSong){//If it's the list of songs
        if(rightClickItem && rightClickItem->text().contains("Play")){
            //The play option plays the song
            idSong = ui->listObjs->indexAt(pos).row();
            player->stop();
            QString file;
            playlist->clear();
            file = folder + albuns[selAlbum]->songs.at(idSong)->getFileName();
            playlist->addMedia(QUrl::fromLocalFile(file));
            ui->songName->setText(albuns[selAlbum]->songs.at(idSong)->getName());
            ui->songArtist->setText(albuns[selAlbum]->songs.at(idSong)->getArtistas());
            ui->albumImage->setPixmap(QPixmap(albuns[selAlbum]->getImagePath()));
            play();
        }
        else if (rightClickItem && rightClickItem->text().contains("Adicionar")){
            //The add option opens the form to add a song
            addSong->exec();
            return;
        }
        else if (rightClickItem && rightClickItem->text().contains("Editar")){
            idSong = ui->listObjs->indexAt(pos).row();
            ui->songArtist->setText(QString::number(idSong));
            if(idSong == 1){
                //The edit option open the edit form for the active album
                edit->setData(2, albuns[selAlbum]);
                edit->exec();
                editing = false;
                menu=true;
                return;
            }
            if(idSong>=2){
                //The edit option open the edit form for the selected song
                idSong -= 2;
                edit->setData(1, albuns[selAlbum]->songs[idSong]);
                edit->exec();
                editing = false;
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
            ui->songArtist->setText(QString::number(selAlbum) + QString::number(song));
            removeMusica(selAlbum, song);
        }
    }
}

///Removes a song from an album
/// @param alb an interger that indicates the album
/// @param song an interger that indicates the song
bool MainWindow::removeMusica(int alb, int song){
    //Removes the song from the program
    albuns[alb]->songs.removeAt(song);
    //Updates the list of songs
    updateSongList(alb);
    //Updates the database
    QSqlQuery query;
    query.prepare("DELETE FROM Musica WHERE Album =:a AND ID =:i");
    query.bindValue(":a", alb);
    query.bindValue(":i", song);
    return query.exec();
}

///Removes an album
/// @param alb an interger that indicates the album
bool MainWindow::removeAlbum(int alb){
    //Save the name and description
    QString auxN, auxD;
    auxN = albuns[alb]->getNome();
    auxD = albuns[alb]->getDescricao();
    int tot = albuns[alb]->songs.count()-1;
    for(int i = tot; i >= 0; i--){
        //Removes all the songs from the album
        removeMusica(alb, i);
    }
    //Removes the album from the program
    albuns.removeAt(alb);
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
    return res;
}
