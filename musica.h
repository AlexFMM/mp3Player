#ifndef MUSICA_H
#define MUSICA_H

#include <QList>
#include <QString>
#include <QDate>

class Musica
{
public:
    Musica();
    Musica(QString nome, QString file, QList<QString> artistas, QString genero);
    Musica(const char *nome, const char *file, const char *artistas, const char *genero);
    Musica(const char * file);
    ~Musica();
    void setNome(QString);
    void setFileName(QString);
    void setGenero(QString);
    void setArtistas(QList<QString>);
    QString getName();
    QString getFileName();
    QString getGenero();
    QString getArtistas();


private:
    QString nome;
    QString fileName;
    QList<QString> artistas;
    QString genero;
    QDate dataDeAdicao;
};

#endif // MUSICA_H
