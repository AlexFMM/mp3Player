/*!
  * \file addalbumform.cpp
*/

#include "addalbumform.h"
#include "ui_addalbumform.h"
/*!
 * \brief AddAlbumForm::AddAlbumForm Cria janela de adição de álbum
 * \param parent
 */
AddAlbumForm::AddAlbumForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAlbumForm)
{
    Qt::WindowFlags flags;
    flags = (Qt::WindowSystemMenuHint | Qt::WindowTitleHint) &
              ~Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    ui->setupUi(this);
}
/*!
 * \brief AddAlbumForm::~AddAlbumForm Elimina janela de adição de álbum
 */
AddAlbumForm::~AddAlbumForm()
{
    delete ui;
}
/*!
 * \brief AddAlbumForm::on_pushButton_clicked Mostra imagem do álbum
 */
void AddAlbumForm::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image file", "c:/", "Image Files (*.jpg)");
    ui->imageFile->setText(fileName);
}
/*!
 * \brief AddAlbumForm::getInfo Lista de informções do álbum
 * \return Retorna lista de informações do álbum
 */
QList<QString> AddAlbumForm::getInfo(){
    QList<QString> list;
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    if(ui->imageFile->toPlainText() != "")
        list.append(ui->imageFile->toPlainText());
    return list;
}
/*!
 * \brief AddAlbumForm::on_buttonBox_clicked Botão de confirmação de informações
 * \param button Retorna informações do álbum
 */

void AddAlbumForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK"){
        this->done(QDialog::Rejected);
        return;
    }
    if(ui->nome->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher o nome");
    }
    else if(ui->descricao->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher a descrição");
    }
    else{
        this->done(QDialog::Accepted);
        ui->imageFile->setText("");
        ui->nome->setText("");
        ui->descricao->setText("");
    }
}
