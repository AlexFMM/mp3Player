#ifndef ADDPLAYLISTFORM_H
#define ADDPLAYLISTFORM_H

/*!
  * \file addplaylistform.h
*/
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractButton>

namespace Ui {
class AddPlaylistForm;
}
/*!
 * \brief The AddPlaylistForm class Adiciona uma playlist
 */
class AddPlaylistForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddPlaylistForm(QWidget *parent = 0);
    QList<QString> getInfo();
    ~AddPlaylistForm();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::AddPlaylistForm *ui;
};

#endif // ADDPLAYLISTFORM_H
