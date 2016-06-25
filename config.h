#ifndef CONFIG_H
#define CONFIG_H

/*!
  * \file config.h
*/
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class Config;
}
/*!
 * \brief The Config class Classe de configurações do player
 */
class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();
    void setData(QString);
    QString getData();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_browse_clicked();

private:
    Ui::Config *ui;
};

#endif // CONFIG_H
