#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QtConcurrent/qtconcurrentrun.h>

#include "wrnch.h"

MainWindow::MainWindow(QWidget *parent,Model* model):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_settingFile=QApplication::applicationDirPath()+"/settings.ini";
    this->model=model;
    loadSettings();
    updateView();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::updateView()
{
    if(model->getType()==Model::Type::CAM)
        ui->camera_radioButton->click();
    else if(model->getType()==Model::Type::FILE)
        ui->media_radioButton->click();
    ui->models_lineEdit->setText(model->getModel_dir());
    ui->media_lineEdit->setText(model->getFile());
    ui->render3d_checkBox->setCheckState(model->getRender3D()? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->camidx_spinBox->setValue(model->getCam_index());
    ui->body_comboBox->setCurrentText(model->getBodyType());
    ui->body_groupBox->setChecked(model->getBodyRender());
    ui->head_comboBox->setCurrentText(model->getHeadType());
    ui->head_groupBox->setChecked(model->getHeadRender());
    ui->hand_comboBox->setCurrentText(model->getHandType());
    ui->hand_groupBox->setChecked(model->getHandRender());
    ui->csv_checkBox->setCheckState(model->getCsv()?Qt::CheckState::Checked:Qt::CheckState::Unchecked);
    ui->video_checkBox->setCheckState(model->getVideo()?Qt::CheckState::Checked:Qt::CheckState::Unchecked);
    saveSettings();
}

void MainWindow::loadSettings()
{
    QSettings settings(m_settingFile,QSettings::IniFormat);
    model->setModel_dir(settings.value("model_dir").toString());
    model->setFile(settings.value("file").toString());
    model->setCam_index(settings.value("Cam_index").toInt());
    model->setType(static_cast<Model::Type>(settings.value("type").toInt()));
}

void MainWindow::saveSettings()
{
    QSettings settings(m_settingFile,QSettings::IniFormat);
    settings.setValue("model_dir",model->getModel_dir());
    settings.setValue("file",model->getFile());
    settings.setValue("Cam_index",model->getCam_index());
    settings.setValue("type",model->getType());
}

void MainWindow::updateStatus(QString message)
{
    statusBar()->showMessage(message);
}

void MainWindow::on_actionexecute_triggered()
{
    qDebug()<<"execute command";
    Wrnch* wrnch=new Wrnch(model);
    disconnect(wrnch,SIGNAL(updateStatus(QString)),this,SLOT(updateStatus(QString)));
    connect(wrnch,SIGNAL(updateStatus(QString)),this,SLOT(updateStatus(QString)),Qt::UniqueConnection);
    wrnch->start();
}

//Panel elements signals
void MainWindow::on_camera_radioButton_clicked()
{
    model->setType(Model::Type::CAM);
    ui->camera_groupBox->setEnabled(true);
    ui->media_groupBox->setEnabled(false);
}

void MainWindow::on_camidx_spinBox_valueChanged(int arg1)
{
    model->setCam_index(arg1);
    updateView();
}

void MainWindow::on_media_radioButton_clicked()
{
    model->setType(Model::Type::FILE);
    ui->camera_groupBox->setEnabled(false);
    ui->media_groupBox->setEnabled(true);
}

void MainWindow::on_media_lineEdit_textEdited(const QString &arg1)
{
    model->setFile(arg1);
    updateView();
}

void MainWindow::on_media_pushButton_clicked()
{
    QString file=QFileDialog::getOpenFileName(this,tr("open files"),"",tr("files (*.avi *.mp4 *.mov *.mpg);;Allfiles(*)"));
    if(!file.isNull()){
        model->setFile(file);
    }
    updateView();
}

void MainWindow::on_models_pushButton_clicked()
{
    QString model_folder=QFileDialog::getExistingDirectory(this,tr("open model folder"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(!model_folder.isNull()){
        model->setModel_dir(model_folder);
    }
    updateView();
}

void MainWindow::on_body_groupBox_clicked(bool checked)
{
    model->setBodyRender(checked);
}

void MainWindow::on_body_comboBox_currentIndexChanged(const QString &arg1)
{
    model->setBodyType(arg1);
}

void MainWindow::on_head_groupBox_clicked(bool checked)
{
    model->setHeadRender(checked);
}

void MainWindow::on_head_comboBox_currentIndexChanged(const QString &arg1)
{
    model->setHeadType(arg1);
}

void MainWindow::on_hand_groupBox_clicked(bool checked)
{
    model->setHandRender(checked);
}

void MainWindow::on_hand_comboBox_currentIndexChanged(const QString &arg1)
{
    model->setHandType(arg1);
}

void MainWindow::on_width_LineEdit_textChanged(const QString &arg1)
{
    model->setResolution_width(arg1.toInt());
}

void MainWindow::on_height_LineEdit_textChanged(const QString &arg1)
{
    model->setResolution_height(arg1.toInt());
}

void MainWindow::on_render3d_checkBox_stateChanged(int arg1)
{
    model->setRender3D(arg1);
}

void MainWindow::on_actioncli_triggered()
{
    dialog.setModel(this->model);
    dialog.show();
}

void MainWindow::on_json_lineEdit_textChanged(const QString &arg1)
{
    model->setJson(arg1);
}

void MainWindow::on_ik_checkBox_stateChanged(int arg1)
{
    model->setUseIk(arg1);
}

void MainWindow::on_csv_checkBox_stateChanged(int arg1)
{
    model->setCsv(arg1);
}

void MainWindow::on_video_checkBox_stateChanged(int arg1)
{
    model->setVideo(arg1);
}
