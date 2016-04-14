#include "addmusicform.h"
#include "ui_addmusicform.h"

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

AddMusicForm::~AddMusicForm()
{
    delete ui;
}

void AddMusicForm::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open a Song", "c:/", "Music Files (*.mp3)");
    ui->pathToFile->setText(fileName);
}

QList<QString> AddMusicForm::getInfo(){
    QList<QString> list;
    list.append(ui->nome->toPlainText());
    list.append(ui->artistas->toPlainText());
    list.append(ui->pathToFile->toPlainText());
    return list;
}

void AddMusicForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK"){
        this->done(QDialog::Rejected);
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
    }
}
