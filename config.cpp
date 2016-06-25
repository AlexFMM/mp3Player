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
 * \brief EditInfo::setData Altera dados de uma música
 * \param path Variável do path utilizado
 */
void Config::setData(QString path){
    ui->path->setText(path);
}

QString Config::getData(){
    return ui->path->text();
}

void Config::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK"){
        this->done(QDialog::Rejected);
        return;
    }
    if(ui->path->text() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher o caminho da diretoria");
    }
    else{
        this->done(QDialog::Accepted);
    }
}
/*!
 * \brief Config::on_pushButton_clicked Procura directoria para guardar ficheiros
 */
void Config::on_browse_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this,
                                    "Open Directory",ui->path->text(),
                                    QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);
    ui->path->setText(dir);
}
