#include "playlist.h"

Playlist::Playlist()
{

}

Playlist::Playlist(QString nome, QString descricao)
{
    this->nome = nome;
    this->descricao = descricao;
}

Playlist::Playlist(const char *nome, const char *descricao)
{
    this->nome = nome;
    this->descricao = descricao;
}

Playlist::~Playlist(){
    this->song.clear();
    this->album.clear();
}

void Playlist::addSong(int al, int s){
    this->album.append(al);
    this->song.append(s);
}

void Playlist::removeSong(int al, int s){
    for(int i =0; i < this->album.count();i++){
        if(this->album[i] == al && this->song[i] == s){
            this->album.removeAt(i);
            this->song.removeAt(i);
        }
    }
}

void Playlist::removeSong(int id){
    this->album.removeAt(id);
    this->song.removeAt(id);

}

QList<int> Playlist::getSong(int id){
    QList<int> aux;
    aux.append(this->album[id]);
    aux.append(this->song[id]);
    return aux;
}

void Playlist::setNome(QString nome){
    this->nome = nome;
}

void Playlist::setDescricao(QString desc){
    this->descricao = desc;
}

QString Playlist::getDescricao(){
    return this->descricao;
}

QString Playlist::getNome(){
    return this->nome;
}

int Playlist::getTot(){
    return this->album.count();
}
