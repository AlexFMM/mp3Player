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
    QString getNome();
    QString getDescricao();
    QList<Musica*> songs;

private:
    QString nome;
    QString descricao;
    QString imageFile;
};

#endif // ALBUM_H
