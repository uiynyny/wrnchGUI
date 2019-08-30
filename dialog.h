#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include "model.h"

#include <QDebug>
class ProcessMonitor : public QObject {
   Q_OBJECT

public slots:

    void notifyStart(){qDebug()<<"start\n";}

    void handleError(QProcess::ProcessError e){qDebug()<<e;}

    void notifyStop(int exitCode, QProcess::ExitStatus ex){qDebug()<<exitCode<<" "<<ex;}
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    Model *getModel() const;
    void setModel(Model *value);

private slots:
    void on_buttonBox_accepted();

    void on_input_lineEdit_textChanged(const QString &arg1);

    void on_modeldir_lineEdit_textChanged(const QString &arg1);

    void on_json_lineEdit_textChanged(const QString &arg1);

    void on_proto_lineEdit_textChanged(const QString &arg1);

    void on_inspect_proto_lineEdit_textChanged(const QString &arg1);

    void on_protovideo_lineEdit_textChanged(const QString &arg1);

    void on_outmedia_lineEdit_textChanged(const QString &arg1);

    void on_green_checkBox_stateChanged(int arg1);

    void on_head_checkBox_stateChanged(int arg1);

    void on_hand_checkBox_stateChanged(int arg1);

    void on_d3d_checkBox_stateChanged(int arg1);

    void on_ik_checkBox_stateChanged(int arg1);

    void on_tracker_kind_comboBox_currentTextChanged(const QString &arg1);

    void on_serialize_lineEdit_textChanged(const QString &arg1);
    void on_fbx_lineEdit_textChanged(const QString &arg1);

    void on_inspect_lineEdit_textChanged(const QString &arg1);

    void on_model_regex_lineEdit_textChanged(const QString &arg1);

    void on_min_valid_joint_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_track_groupBox_toggled(bool arg1);

private:

    void updateView();
    Ui::Dialog *ui;
    Model *model;
    QString exefile;
    ProcessMonitor *pm;
};
#endif // DIALOG_H
