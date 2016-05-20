#ifndef EDITINFO_H
#define EDITINFO_H
/*!
  * \file editinfo.h
*/

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "album.h"
#include "playlist.h"

namespace Ui {
class EditInfo;
}
/*!
 * \brief The EditInfo class Classe de edição de váriaveis informativas de álbuns, músicas e playlists
 */
class EditInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditInfo(QWidget *parent = 0);
    ~EditInfo();
    void setData(int, Musica*);
    void setData(int, Album*);
    void setData(int, Playlist*);
    QList<QString> getData();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_clicked();

    void on_browseButton_clicked();

private:
    Ui::EditInfo *ui;
    int type;
};

#endif // EDITINFO_H
