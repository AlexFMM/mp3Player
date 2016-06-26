#include "album.h"
/*!
  * \file album.cpp
*/
Album::Album()
{

}
/*!
 * \brief Album::Album Construtor por parâmetros de um álbum
 * \param nome Nome do álbum
 * \param descricao descrição do álbum
 */
Album::Album(QString nome, QString descricao)
{
    this->nome= nome;
    this->descricao = descricao;
}
/*!
 * \brief Album::Album Construtor por parâmetros de um álbum
 * \param nome Nome do álbum
 * \param descricao Descrição do álbum
 * \param imagem Imagem do álbum
 */
Album::Album(QString nome, QString descricao, QString imagem)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = imagem;
}
/*!
 * \brief Album::Album Construtor por cópia de um álbum
 * \param nome Nome do álbum
 * \param Descrição do álbum
 */
Album::Album(const char *nome, const char *descricao)
{
    this->nome= nome;
    this->descricao = descricao;
}
/*!
 * \brief Album::Album Construtor por cópia de um álbum
 * \param nome Nome do álbum
 * \param Descrição do álbum
 * \param imagem Imagem do álbum
 */
Album::Album(const char *nome, const char *descricao, const char *imagem)
{
    this->nome= nome;
    this->descricao = descricao;
    this->imageFile = imagem;
}
/*!
 * \brief Album::setNome Define o nome do álbum
 * \param nome Nome do álbum
 */
void Album::setNome(QString nome){
    this->nome = nome;
}
/*!
 * \brief Album::setDescricao Define descrição do álbum
 * \param descricao Descrição do álbum
 */
void Album::setDescricao(QString descricao){
    this->descricao = descricao;
}
/*!
 * \brief Album::setImagePath Define imagem do álbum
 * \param path Define directoria da imagem do álbum
 */
void Album::setImagePath(QString path){
    this->imageFile = path;
}
/*!
 * \brief Album::addMusica Adiciona música a um álbum
 * \param song Música para o álbum
 */
void Album::addMusica(Musica *song){
    this->songs.append(song);
}
/*!
 * \brief Album::getImagePath Carrega o caminho para a imagem do álbum
 * \return Retorna o caminho para a imagem do álbum
 */
QString Album::getImagePath(){
    return this->imageFile;
}
/*!
 * \brief Album::getNome Carrega o nome do álbum
 * \return Retorna o nome do álbum
 */
QString Album::getNome(){
    return this->nome;
}
/*!
 * \brief Album::getDescricao Carrega descrição do álbum
 * \return Retorna descrição do álbum
 */
QString Album::getDescricao(){
    return this->descricao;
}

