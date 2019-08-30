#include "dialog.h"
#include "ui_dialog.h"
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QFileDialog>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QRegExp re("^[1-9][0-9]{2}x[1-9][0-9]{2}$");
    ui->net_res_lineEdit->setValidator(new QRegExpValidator(re,this));
    exefile=QDir::currentPath()+"/wrCLI.exe";
    pm=new ProcessMonitor;
}

Dialog::~Dialog()
{
    delete ui;
}

#include <QMessageBox>
void Dialog::on_buttonBox_accepted()
{
    QProcess *proc=new QProcess(this);
    connect(proc,SIGNAL(error(QProcess::ProcessError)),pm,SLOT(handleError(QProcess::ProcessError)));
    connect(proc,SIGNAL(started()),pm,SLOT(notifyStart()));
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),pm,SLOT(notifyStop(int, QProcess::ExitStatus)));

    proc->setWorkingDirectory(QDir::currentPath());
    qDebug()<<model->getArgs();
    proc->start(QString("cmd.exe /C start %1 %2").arg(exefile).arg(model->getArgs()));
    proc->waitForFinished();
        QString result = QString(proc->readAllStandardOutput());
        qDebug()<<result;
        if(!result.isNull()){
            QMessageBox(QMessageBox::NoIcon,"result",result,QMessageBox::Ok,this,Qt::Dialog).exec();
        }
}

Model *Dialog::getModel() const
{
    return model;
}

void Dialog::setModel(Model *value)
{
    model = value;
    updateView();
}

void Dialog::on_input_lineEdit_textChanged(const QString &arg1)
{
    model->setFile(arg1);
}

void Dialog::on_modeldir_lineEdit_textChanged(const QString &arg1)
{
    model->setModel_dir(arg1);
}

void Dialog::on_json_lineEdit_textChanged(const QString &arg1)
{
    model->setJson(arg1);
}

void Dialog::on_proto_lineEdit_textChanged(const QString &arg1)
{
    model->setProto(arg1);
}

void Dialog::on_inspect_proto_lineEdit_textChanged(const QString &arg1)
{
    model->setInspect_proto(arg1);
}

void Dialog::on_protovideo_lineEdit_textChanged(const QString &arg1)
{
    model->setProto_video(arg1);
}

void Dialog::on_outmedia_lineEdit_textChanged(const QString &arg1)
{
    model->setOutmedia(arg1);
}

void Dialog::on_green_checkBox_stateChanged(int arg1)
{
    model->setGreen_screen(arg1);
}

void Dialog::on_head_checkBox_stateChanged(int arg1)
{
    model->setHeadRender(arg1);
}

void Dialog::on_hand_checkBox_stateChanged(int arg1)
{
    model->setHandRender(arg1);
}

void Dialog::on_d3d_checkBox_stateChanged(int arg1)
{
    model->setRender3D(arg1);
}

void Dialog::on_ik_checkBox_stateChanged(int arg1)
{
    model->setUseIk(arg1);
}

void Dialog::on_tracker_kind_comboBox_currentTextChanged(const QString &arg1)
{
    model->setTracking_kind(arg1);
}

void Dialog::on_serialize_lineEdit_textChanged(const QString &arg1)
{
    model->setDeserialize("");
    model->setSerialize("");
    if(ui->deserialize_radioButton->isChecked()) model->setDeserialize(arg1);
    if(ui->serialize_radioButton->isChecked()) model->setSerialize(arg1);
}

void Dialog::on_fbx_lineEdit_textChanged(const QString &arg1)
{
    model->setFbx(arg1);
}

void Dialog::on_inspect_lineEdit_textChanged(const QString &arg1)
{
    model->setInspect(arg1);
}

void Dialog::on_model_regex_lineEdit_textChanged(const QString &arg1)
{
    model->setModel_regex(arg1);
}

void Dialog::on_min_valid_joint_lineEdit_textChanged(const QString &arg1)
{
    model->setMin_valid_joints(arg1);
}

void Dialog::updateView()
{
    ui->input_lineEdit->setText(model->getFile());
    ui->modeldir_lineEdit->setText(model->getModel_dir());
    ui->model_regex_lineEdit->setText(model->getModel_regex());
    ui->json_lineEdit->setText(model->getJson());
    ui->proto_lineEdit->setText(model->getProto());
    ui->inspect_proto_lineEdit->setText(model->getInspect_proto());
    ui->protovideo_lineEdit->setText(model->getProto_video());
    ui->outmedia_lineEdit->setText(model->getOutmedia());
    ui->green_checkBox->setCheckState(model->getGreen_screen()?Qt::Checked:Qt::Unchecked);
    ui->head_checkBox->setCheckState(model->getHeadRender()?Qt::Checked:Qt::Unchecked);
    ui->hand_checkBox->setCheckState(model->getHandRender()?Qt::Checked:Qt::Unchecked);
    ui->track_groupBox->setChecked(model->getTracking()?Qt::Checked:Qt::Unchecked);
    ui->d3d_checkBox->setCheckState(model->getRender3D()?Qt::Checked:Qt::Unchecked);
    ui->ik_checkBox->setCheckState(model->getUseIk()?Qt::Checked:Qt::Unchecked);
    ui->tracker_kind_comboBox->setCurrentText(model->getTracking_kind().isEmpty()?"V2":model->getTracking_kind());
}

void Dialog::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("wrCLI file"),QDir::currentPath(),tr("exe files (*.exe)"));
    if(!filename.isEmpty()){
        exefile=filename;
        qDebug()<<exefile;
    }
}

void Dialog::on_track_groupBox_toggled(bool arg1)
{
     model->setTracking(arg1);
}
