#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

private slots:
    void on_pushButton_clicked();

    void on_textEdit_objectNameChanged(const QString &objectName);

private:
    Ui::Config *ui;
};

#endif // CONFIG_H
