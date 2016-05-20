/*!
  * \file config.cpp
*/
#include "config.h"
#include "ui_config.h"
#include <QFileDialog>
/*!
 * \brief Config::Config Cria janela de configurações
 * \param parent
 */
Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
}
/*!
 * \brief Config::~Config Elimina janela de configurações
 */
Config::~Config()
{
    delete ui;
}
/*!
 * \brief Config::on_pushButton_clicked Procura directoria para guardar ficheiros
 */
void Config::on_pushButton_clicked()
{
    QString mkdir = QFileDialog::getExistingDirectory(this,
                                                      "Open Directory","C:/",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    ui->textEdit->setText(mkdir);
}

void Config::on_textEdit_objectNameChanged(const QString &objectName)
{
  //  mkdir.show();
}
