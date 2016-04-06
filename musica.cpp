#include "musica.h"

Musica::Musica()
{
    this->dataDeAdicao = QDate::currentDate();
}

Musica::Musica(QString nome, QString file, QList<QString> artistas, QString genero)
{
    this->nome= nome;
    this->fileName = file;
    this->artistas = artistas;
    this->genero = genero;
    this->dataDeAdicao = QDate::currentDate();
}

Musica::Musica(const char *nome, const char *file, const char *artistas, const char *genero){
    this->nome= nome;
    this->fileName = file;
    this->genero = genero;
    this->artistas = QString::fromUtf8(artistas).split(",");
    this->dataDeAdicao = QDate::currentDate();
}

Musica::Musica(const char *file){
    this->fileName = file;
    this->dataDeAdicao = QDate::currentDate();
}

void Musica::setNome(QString nome){
    this->nome = nome;
}
void Musica::setFileName(QString file){
    this->fileName = file;
}
void Musica::setArtistas(QList<QString> artistas){
    this->artistas = artistas;
}
void Musica::setGenero(QString genero){
    this->genero = genero;
}

QString Musica::getFileName(){
    return this->fileName;
}
QString Musica::getName(){
    return this->nome;
}
QString Musica::getArtistas(){
    QString aux;
    for(int i=0; i < this->artistas.count();i++){
        aux.append(this->artistas.at(i));
        if(i < this->artistas.count()-1)
            aux.append(", ");
    }
    return aux;
}
QString Musica::getGenero(){
    return this->genero;
}