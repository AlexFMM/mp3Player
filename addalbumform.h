#ifndef ADDALBUMFORM_H
#define ADDALBUMFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class AddAlbumForm;
}

class AddAlbumForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddAlbumForm(QWidget *parent = 0);
    QList<QString> getInfo();
    ~AddAlbumForm();

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::AddAlbumForm *ui;
};

#endif // ADDFORM_H
