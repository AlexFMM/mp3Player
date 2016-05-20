/*!
  * \file addmusicform.cpp
*/

#include "addmusicform.h"
#include "ui_addmusicform.h"
/*!
 * \brief AddMusicForm::AddMusicForm Cria janela de adição de música
 * \param parent
 */
AddMusicForm::AddMusicForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMusicForm)
{
    Qt::WindowFlags flags;
    flags = (Qt::WindowSystemMenuHint | Qt::WindowTitleHint) &
              ~Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    ui->setupUi(this);
}
/*!
 * \brief AddMusicForm::~AddMusicForm Elimina janela de adição de música
 */
AddMusicForm::~AddMusicForm()
{
    delete ui;
}
/*!
 * \brief AddMusicForm::on_pushButton_clicked Na adição de músicas que podem ser aceites fica limitada apenas a .mp3
 */
void AddMusicForm::on_pushButton_clicked()
{
    QString fileName;
    if(ui->batchAdd->checkState() == Qt::Checked){
        QList<QString> files = QFileDialog::getOpenFileNames(this,
            "Open Songs", "C:/", "Music Files (*.mp3)");

        for(int i=0; i < files.count();i++){
            fileName.append(files.at(i));
            if(i < files.count()-1)
                fileName.append(";");
        }
    }
    else{
        fileName = QFileDialog::getOpenFileName(this,
            "Open a Song", "C:/", "Music Files (*.mp3)");
    }
    ui->pathToFile->setText(fileName);
}
/*!
 * \brief AddMusicForm::getInfo Lista de obtenção de informações da música
 * \return Retorna lista de informações da música
 */
QList<QString> AddMusicForm::getInfo(){
    QList<QString> list;
    if(ui->batchAdd->checkState() == Qt::Checked){
        list.append("_batch_");
        list.append(ui->pathToFile->toPlainText());
    }
    else{
        list.append(ui->nome->toPlainText());
        list.append(ui->artistas->toPlainText());
        list.append(ui->pathToFile->toPlainText());
        list.append(ui->genero->toPlainText());
    }
    return list;
}
/*!
 * \brief AddMusicForm::on_buttonBox_clicked Botão de confirmação de informações
 * \param button Retorna informações da música
 */
void AddMusicForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK"){
        this->done(QDialog::Rejected);
        return;
    }
    if(ui->batchAdd->checkState() == Qt::Checked){
        this->done(QDialog::Accepted);
        ui->pathToFile->setText("");
        ui->nome->setText("");
        ui->artistas->setText("");
        ui->genero->setText("");
        return;
    }
    if(ui->nome->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher o nome");
    }
    else if(ui->artistas->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher o(s) artista(s)");
    }
    else if(ui->pathToFile->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta indicar o ficheiro");
    }
    else{
        this->done(QDialog::Accepted);
        ui->pathToFile->setText("");
        ui->nome->setText("");
        ui->artistas->setText("");
        ui->genero->setText("");
    }
}
/*!
 * \brief AddMusicForm::on_batchAdd_toggled
 * \param checked
 */
void AddMusicForm::on_batchAdd_toggled(bool checked)
{
    if(checked){
        ui->nome->setEnabled(false);
        ui->artistas->setEnabled(false);
        ui->genero->setEnabled(false);
    }
    else{
        ui->nome->setEnabled(true);
        ui->artistas->setEnabled(true);
        ui->genero->setEnabled(true);
    }
}
