#ifndef ALBUM_H
#define ALBUM_H

#include "musica.h"

class Album
{
public:
    Album();
    Album(QString nome, QString descricao);
    Album(QString nome, QString descricao, QString imagem);
    Album(const char *nome, const char *descricao);
    Album(const char *nome, const char *descricao, const char *imagem);
    ~Album();
    void setNome(QString);
    void setDescricao(QString);
    void setImagePath(QString);
    QString getImagePath();
    void addMusica(Musica*);
    Musica* getSong(int id);
    int getTotalSongs();
    QString getNome();

private:
    QString nome;
    QString descricao;
    QString imageFile;
    QList<Musica*> songs;
};

#endif // ALBUM_H
