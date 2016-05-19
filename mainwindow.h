#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QKeyEvent>
#include <QPixmap>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QtSql>

#include "album.h"
#include "playlist.h"
#include "addalbumform.h"
#include "addmusicform.h"
#include "addplaylistform.h"
#include "editinfo.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* keyevent);

private slots:
    void play();
    void setVolume();
    void setBarPosition();
    void setSongPosition();
    void movingSlider();
    void setEndTime();
    void songEnd();
    void changeList();
    void checkPlayLists();
    void dialogAlbumFinished(int);
    void dialogMusicFinished(int);
    void dialogEditFinished(int);
    void dialogPlayListFinished(int);
    void search();
    void on_actionAdicionarMusica_triggered();
    void on_actionAdicionarAlbum_triggered();
    void ProvideContextMenu(const QPoint &);
    void ProvideContextMenuPlay(const QPoint &);
    void changeInfo();
    void on_actionConfigura_o_triggered();
    void on_next_clicked();
    void on_prev_clicked();
    void on_repetir_clicked();
    void on_aleatorio_clicked();
    void on_btnAddPlay_clicked();

private:
    void updateAlbumList();
    void updateSongList(int);
    void updatePlaylist();
    void createDB();
    void readFromDB();
    bool removeAlbum(int);
    bool removeMusica(int, int);
    bool removePlaylist(int);
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    bool moving;
    bool menu;
    bool playList;
    int idSong;
    int activeAlbum;
    int activePlay;
    int prevMode;
    QString folder;
    QList<Album*> albuns;
    QList<Playlist *> playlists;
    QStandardItemModel *albumModel;
    QStandardItemModel *tempSong;
    QStandardItemModel *playModel;
    QStandardItemModel *tempPlay;
    QStandardItemModel *searchResults;
    QList<int> searchResultsIds;
    QList<QString> artistas;
    int selAlbum;
    int selPlay;
    QSqlDatabase db;
    AddAlbumForm *addAlbum;
    AddMusicForm *addSong;
    AddPlaylistForm *addPlay;
    EditInfo *edit;
    Config *conf;
};

#endif // MAINWINDOW_H
