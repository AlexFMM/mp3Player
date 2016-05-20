#ifndef PLAYLIST_H
#define PLAYLIST_H
/*!
  * \file playlist.h
*/
#include <QString>
#include <QList>
/*!
 * \brief The Playlist class Classe descritiva de uma música
 */
class Playlist
{
public:
    Playlist();
    Playlist(QString nome, QString descricao);
    Playlist(const char *nome, const char *descricao);
    ~Playlist();
    void setNome(QString);
    void setDescricao(QString);
    void addSong(int, int);
    void removeSong(int, int);
    void removeSong(int);
    QString getNome();
    QString getDescricao();
    QList<int> getSong(int);
    int getTot();

private:
    QString nome;
    QString descricao;
    QList<int> album;
    QList<int> song;
};

#endif // PLAYLIST_H
