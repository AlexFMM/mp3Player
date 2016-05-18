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
#include "addalbumform.h"
#include "addmusicform.h"
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
    void dialogAlbumFinished(int);
    void dialogMusicFinished(int);
    void dialogEditFinished(int);
    void search();
    void on_actionAdicionarMusica_triggered();
    void on_actionAdicionarAlbum_triggered();
    void ProvideContextMenu(const QPoint &);

    void on_editSong_clicked();

    void on_actionConfigura_o_triggered();


private:
    void updateAlbumList();
    void updateSongList(int);
    void createDB();
    void readFromDB();
    bool removeAlbum(int);
    bool removeMusica(int, int);
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    bool moving;
    bool editing;
    bool menu;
    int idSong;
    QString folder;
    QList<Album*> albuns;
    QStandardItemModel *albumModel;
    QStandardItemModel *tempSong;
    QStandardItemModel *searchResults;
    QList<int> searchResultsIds;
    QList<QString> artistas;
    int selAlbum;
    QSqlDatabase db;
    AddAlbumForm *addAlbum;
    AddMusicForm *addSong;
    EditInfo *edit;
    Config *conf;
};

#endif // MAINWINDOW_H
