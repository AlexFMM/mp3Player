#ifndef ADDMUSICFORM_H
#define ADDMUSICFORM_H
/*!
  * \file addmusicform.h
*/

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class AddMusicForm;
}
/*!
 * \brief The AddMusicForm class Classe de adição de uma música
 */
class AddMusicForm : public QDialog
{

    Q_OBJECT

public:
    explicit AddMusicForm(QWidget *parent = 0);
    QList<QString> getInfo();
    ~AddMusicForm();

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_batchAdd_toggled(bool checked);

private:
    Ui::AddMusicForm *ui;
};

#endif // ADDMUSICFORM_H
