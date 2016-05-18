#include "mainwindow.h"
#include "config.h"
#include "ui_config.h"
#include <QFileDialog>

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
}

Config::~Config()
{
    delete ui;
}

void Config::on_pushButton_clicked()
{

    QFile::copy("C:/Users/Filipe/Desktop/songs/albuns","C:/Users/Filipe/Downloads/caucenus.png");
    QString mkdir = QFileDialog::getExistingDirectory(this,
                                                      "Open Directory","C:/",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);

    ui->textEdit->setText(mkdir);
}

void Config::on_textEdit_objectNameChanged(const QString &objectName)
{
  //  mkdir.show();
}
