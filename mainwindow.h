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

#include "album.h"
#include "addalbumform.h"
#include "addmusicform.h"

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

    void on_actionAdicionarMusica_triggered();
    void on_actionAdicionarAlbum_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    bool moving;
    QString folder;
    //Musica *song;
    QList<Album*> albuns;
    QStandardItemModel *albumModel;
    QStandardItemModel *tempSong;
    int selAlbum;
    AddAlbumForm *addAlbum;
    AddMusicForm *addSong;
};

#endif // MAINWINDOW_H
