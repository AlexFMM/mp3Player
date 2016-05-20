
/*!
  * \file addplaylistform.cpp
*/


#include "addplaylistform.h"
#include "ui_addplaylistform.h"
/*!
 * \brief AddPlaylistForm::AddPlaylistForm Cria janela de adição de playlist
 * \param parent
 */
AddPlaylistForm::AddPlaylistForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPlaylistForm)
{
    Qt::WindowFlags flags;
    flags = (Qt::WindowSystemMenuHint | Qt::WindowTitleHint) &
              ~Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    ui->setupUi(this);
}
/*!
 * \brief AddPlaylistForm::~AddPlaylistForm Elimina janela de adição de playlist
 */
AddPlaylistForm::~AddPlaylistForm()
{
    delete ui;
}
/*!
 * \brief AddPlaylistForm::getInfo Obtém informações das playlists
 * \return
 */
QList<QString> AddPlaylistForm::getInfo(){
    QList<QString> list;
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    return list;
}
/*!
 * \brief AddPlaylistForm::on_buttonBox_clicked Cria uma playlist
 * \param button
 */
void AddPlaylistForm::on_buttonBox_clicked(QAbstractButton *button)
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
        ui->nome->setText("");
        ui->descricao->setText("");
    }
}
