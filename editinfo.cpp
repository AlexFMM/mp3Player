#include "editinfo.h"
#include "ui_editinfo.h"

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

EditInfo::~EditInfo()
{
    delete ui;
}

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

QList<QString> EditInfo::getData(){
    QList<QString> list;
    list.append(QString::number(this->type));
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    list.append(ui->imageFile->toPlainText());
    return list;
}


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
