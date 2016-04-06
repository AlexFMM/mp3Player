#include "album.h"

Album::Album()
{

}

Album::Album(QString nome, QString descricao)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = "";
}

Album::Album(QString nome, QString descricao, QString imagem)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = imagem;
}

Album::Album(const char *nome, const char *descricao)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = "";
}

Album::Album(const char *nome, const char *descricao, const char *imagem)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = imagem;
}

void Album::setNome(QString nome){
    this->nome = nome;
}
void Album::setDescricao(QString descricao){
    this->descricao = descricao;
}
void Album::setImagePath(QString path){
    this->imageFile = path;
}

void Album::addMusica(){
    songs.append(new Musica("The Hunter", "The_Hunter-Mastodon.mp3", "Mastodon", "Metal"));
}

Musica* Album::getSong(int id){
    return songs[id];
}

QString Album::getImagePath(){
    return this->imageFile;
}
