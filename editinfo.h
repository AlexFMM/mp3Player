#ifndef EDITINFO_H
#define EDITINFO_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "album.h"

namespace Ui {
class EditInfo;
}

class EditInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditInfo(QWidget *parent = 0);
    ~EditInfo();
    void setData(int, Musica*);
    void setData(int, Album*);
    QList<QString> getData();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::EditInfo *ui;
    int type;
};

#endif // EDITINFO_H
