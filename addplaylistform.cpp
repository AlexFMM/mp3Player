#include "addplaylistform.h"
#include "ui_addplaylistform.h"

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

AddPlaylistForm::~AddPlaylistForm()
{
    delete ui;
}

QList<QString> AddPlaylistForm::getInfo(){
    QList<QString> list;
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    return list;
}

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
