#include "musica.h"

/*!
  * \file musica.cpp
*/

/*!
 * \brief Musica::Musica Define data de adição da música
 */
Musica::Musica()
{
    this->dataDeAdicao = QDate::currentDate();
}
/*!
 * \brief Musica::Musica Construtor por parâmetro de uma música
 * \param nome Nome da música
 * \param artistas Artistas da música
 */
Musica::Musica(QString nome, QList<QString> artistas)
{
    this->nome= nome;
    this->artistas = artistas;
    this->genero = genero;
    this->dataDeAdicao = QDate::currentDate();
}
/*!
 * \briefMusica::Musica Construtor por parâmetro de uma música
 * \param nome Nome da música
 * \param artistas Artistas da música
 * \param genero Género da música
 */
Musica::Musica(QString nome, QList<QString> artistas, QString genero)
{
    this->nome= nome;
    this->artistas = artistas;
    this->genero = genero;
    this->dataDeAdicao = QDate::currentDate();
}
/*!
 * \brief Musica::Construtor por parâmetro de uma música para vários artistas
 * \param nome Nome da música
 * \param artistas Artistas da música
 * \param genero Género da música
 */
Musica::Musica(QString nome, QString artistas, QString genero){
    this->nome= nome;
    this->genero = genero;
    this->artistas = artistas.split(", ");
    this->dataDeAdicao = QDate::currentDate();
}
/*!
 * \brief MConstrutor por parâmetro de uma música
 * \param nome Nome da música
 * \param artistas Artistas da música
 * \param genero Género da música
 * \param data Data de adição da música
 */
Musica::Musica(QString nome, QString artistas, QString genero, QDate data){
    this->nome= nome;
    this->genero = genero;
    this->artistas = artistas.split(", ");
    this->dataDeAdicao = data;
}
/*!
 * \brief Musica::setNome Define o nome da música
 * \param nome Nome da música
 */
void Musica::setNome(QString nome){
    this->nome = nome;
}
/*!
 * \brief Musica::setArtistas Define artistas da música
 * \param artistas Artistas da música
 */
void Musica::setArtistas(QList<QString> artistas){
    this->artistas = artistas;
}
/*!
 * \brief Musica::setArtistas Define vários artistas da música
 * \param artistas Vários artistas da música
 */
void Musica::setArtistas(QString artistas){
    this->artistas = artistas.split(", ");
}
/*!
 * \brief Musica::setGenero Define género da música
 * \param genero Género da música
 */
void Musica::setGenero(QString genero){
    this->genero = genero;
}
/*!
 * \brief Musica::getName Carrega nome da música
 * \return  Retorna nome da música
 */
QString Musica::getName(){
    return this->nome;
}
/*!
 * \brief Musica::getArtistas Carrega artistas da música
 * \return Retorna artistas da música
 */
QString Musica::getArtistas(){
    QString aux;
    for(int i=0; i < this->artistas.count();i++){
        aux.append(this->artistas.at(i));
        if(i < this->artistas.count()-1)
            aux.append(", ");
    }
    return aux;
}
/*!
 * \brief Musica::getGenero Carrega género da música
 * \return Retorna género da música
 */
QString Musica::getGenero(){
    return this->genero;
}
/*!
 * \brief Musica::getDate Carrega data da música
 * \return Retorna data da música
 */
QDate Musica::getDate(){
    return this->dataDeAdicao;
}
