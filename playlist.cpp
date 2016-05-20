/*!
  * \file playlist.cpp
*/

#include "playlist.h"
Playlist::Playlist()
{

}
/*!
 * \brief Playlist::Playlist Construtor por parâmetros de uma playlist
 * \param nome Nome da playlist
 * \param descricao Descrição da playlist
 */
Playlist::Playlist(QString nome, QString descricao)
{
    this->nome = nome;
    this->descricao = descricao;
}
/*!
 * \brief Playlist::Playlist Construtor por cópia de uma playlist
 * \param nome Nome da playlist
 * \param descricao Descrição da playlist
 */
Playlist::Playlist(const char *nome, const char *descricao)
{
    this->nome = nome;
    this->descricao = descricao;
}
/*!
 * \brief Playlist::~Playlist Destrutor de uma playlist
 */
Playlist::~Playlist(){
    this->song.clear();
    this->album.clear();
}
/*!
 * \brief Playlist::addSong Adiciona uma música à playlist
 * \param al Álbum da música
 * \param s Música a adicionar
 */
void Playlist::addSong(int al, int s){
    this->album.append(al);
    this->song.append(s);
}
/*!
 * \brief Playlist::removeSong Remove música da playlist
 * \param al Álbum da música a remover
 * \param s Música a remover
 */
void Playlist::removeSong(int al, int s){
    for(int i =0; i < this->album.count();i++){
        if(this->album[i] == al && this->song[i] == s){
            this->album.removeAt(i);
            this->song.removeAt(i);
        }
    }
}
/*!
 * \brief Playlist::removeSong Remove música da playlist pelo seu id
 * \param id Do álbum e música
 */
void Playlist::removeSong(int id){
    this->album.removeAt(id);
    this->song.removeAt(id);

}
/*!
 * \brief Playlist::getSong Carrega música para a playlist
 * \param id Id da música
 * \return Retorna a música
 */
QList<int> Playlist::getSong(int id){
    QList<int> aux;
    aux.append(this->album[id]);
    aux.append(this->song[id]);
    return aux;
}
/*!
 * \brief Playlist::setNome Define nome da playlist
 * \param nome Nome da playlist
 */
void Playlist::setNome(QString nome){
    this->nome = nome;
}
/*!
 * \brief Playlist::setDescricao Define Descrição da playlist
 * \param desc Descrição da playlist
 */
void Playlist::setDescricao(QString desc){
    this->descricao = desc;
}
/*!
 * \brief Playlist::getDescricaoDefine Descrição da playlist
 * \param desc Descrição da playlist
 */
QString Playlist::getDescricao(){
    return this->descricao;
}
/*!
 * \brief Playlist::getNome Carrega nome da playlist
 * \return Retorna nome da playlist
 */
QString Playlist::getNome(){
    return this->nome;
}
/*!
 * \brief Playlist::getTot Total de álbuns na playlist
 * \return Retorna Total de álbuns na playlist
 */
int Playlist::getTot(){
    return this->album.count();
}
