#include "addalbumform.h"
#include "ui_addalbumform.h"

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

AddAlbumForm::~AddAlbumForm()
{
    delete ui;
}

void AddAlbumForm::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image file", "c:/Users/alexf/music/", "Image Files (*.jpg)");
    ui->imageFile->setText(fileName);
}

QList<QString> AddAlbumForm::getInfo(){
    QList<QString> list;
    list.append(ui->nome->toPlainText());
    list.append(ui->descricao->toPlainText());
    if(ui->imageFile->toPlainText() != "")
        list.append(ui->imageFile->toPlainText());
    return list;
}


void AddAlbumForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK"){
        this->reject();
        return;
    }
    if(ui->nome->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher o nome");
    }
    else if(ui->descricao->toPlainText() == ""){
        QMessageBox::information(this, "Erro", "Falta preencher a descrição");
    }
    else
        this->accept();
}
