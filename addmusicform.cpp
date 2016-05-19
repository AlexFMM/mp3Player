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
