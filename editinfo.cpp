/*!
  * \file editinfo.cpp
*/


#include "editinfo.h"
#include "ui_editinfo.h"
/*!
 * \brief EditInfo::EditInfo Cria janela de edição de parâmetros
 * \param parent
 */
EditInfo::EditInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditInfo)
{
    Qt::WindowFlags flags;
    flags = (Qt::WindowSystemMenuHint | Qt::WindowTitleHint) &
              ~Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    ui->setupUi(this);
}
/*!
 * \brief EditInfo::~EditInfo Elimina janela de ediçaõ de parâmetros
 */
EditInfo::~EditInfo()
{
    delete ui;
}
/*!
 * \brief EditInfo::setData Altera dados de uma música
 * \param type Variável de inficação de música, álbum ou playlist
 * \param song Variável música a editar
 */
void EditInfo::setData(int type, Musica *song){
    if(type == 1){
        this->type = type;
        ui->browseButton->hide();
        ui->albumImage->hide();
        ui->title->setText(song->getName());
        ui->nome->setText(song->getName());
        ui->lSecondary->setText("Artista(s)");
        ui->descricao->setText(song->getArtistas());
        ui->lImage->setText("Genero");
        ui->imageFile->setText(song->getGenero());
    }
}
/*!
 * \brief EditInfo::setData
 * \param type Variável de inficação de música, álbum ou playlist
 * \param alb Variável álbum a editar
 */
void EditInfo::setData(int type, Album *alb){
    if(type == 2){
        this->type = type;
        ui->lImage->show();
        ui->imageFile->show();
        ui->browseButton->show();
        ui->albumImage->show();
        ui->title->setText(alb->getNome());
        ui->nome->setText(alb->getNome());
        ui->lSecondary->setText("Descrição");
        ui->descricao->setText(alb->getDescricao());
        ui->imageFile->setText(alb->getImagePath());
        ui->albumImage->setPixmap(alb->getImagePath());
    }
}
/*!
 * \brief EditInfo::setData
 * \param type Variável de inficação de música, álbum ou playlist
 * \param play Variável playlist a editar
 */
void EditInfo::setData(int type, Playlist *play){
    if(type == 3){
        ui->browseButton->hide();
        ui->albumImage->hide();
        ui->lImage->hide();
        ui->imageFile->hide();
        ui->title->setText(play->getNome());
        ui->nome->setText(play->getNome());
        ui->lSecondary->setText("Descrição");
        ui->descricao->setText(play->getDescricao());
    }
}
/*!
 * \brief EditInfo::getData Carrega dados de um álbum
 * \return Retorna dados de um álbum
 */
QList<QString> EditInfo::getData(){
    QList<QString> list;
    list.append(QString::number(this->type));
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    list.append(ui->imageFile->toPlainText());
    return list;
}

/*!
 * \brief EditInfo::on_buttonBox_clicked Verificação de coerência de dados
 * \param button Botão de confirmação
 */
void EditInfo::on_buttonBox_clicked(QAbstractButton *button)
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
    }
}
/*!
 * \brief EditInfo::on_pushButton_clicked
 */
void EditInfo::on_pushButton_clicked()
{
    ui->nome->setText("_rem_");
    this->done(QDialog::Accepted);
}
/*!
 * \brief EditInfo::on_browseButton_clicked
 */
void EditInfo::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image file", "c:/", "Image Files (*.jpg)");
    ui->imageFile->setText(fileName);
}
