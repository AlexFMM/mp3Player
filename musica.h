#ifndef MUSICA_H
#define MUSICA_H
/*!
  * \file musica.h
*/
#include <QList>
#include <QString>
#include <QDate>
/*!
 * \brief The Musica class Classe descritiva de uma música
 */
class Musica
{
public:
    Musica();
    Musica(QString nome, QList<QString> artistas);
    Musica(QString nome, QList<QString> artistas, QString genero);
    Musica(QString nome, QString artistas, QString genero);
    Musica(QString nome, QString artistas, QString genero, QDate data);
    ~Musica();
    void setNome(QString);
    void setGenero(QString);
    void setArtistas(QList<QString>);
    void setArtistas(QString);
    QString getName();
    QString getGenero();
    QString getArtistas();
    QDate getDate();


private:
    QString nome;
    QList<QString> artistas;
    QString genero;
    QDate dataDeAdicao;
};

#endif // MUSICA_H
