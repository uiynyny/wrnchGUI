#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include <QThread>
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,Model* model=nullptr);

    ~MainWindow();

    void setModel(Model *model);

    void updateView();

    void loadSettings();

    void saveSettings();

private slots:

    void updateStatus(QString);

    void on_actionexecute_triggered();

    void on_camera_radioButton_clicked();

    void on_camidx_spinBox_valueChanged(int arg1);

    void on_media_radioButton_clicked();

    void on_media_pushButton_clicked();

    void on_media_lineEdit_textEdited(const QString &arg1);

    void on_models_pushButton_clicked();

    void on_body_groupBox_clicked(bool checked);

    void on_head_groupBox_clicked(bool checked);

    void on_body_comboBox_currentIndexChanged(const QString &arg1);

    void on_head_comboBox_currentIndexChanged(const QString &arg1);

    void on_hand_groupBox_clicked(bool checked);

    void on_hand_comboBox_currentIndexChanged(const QString &arg1);

    void on_width_LineEdit_textChanged(const QString &arg1);

    void on_height_LineEdit_textChanged(const QString &arg1);

    void on_render3d_checkBox_stateChanged(int arg1);

    void on_actioncli_triggered();

    void on_json_lineEdit_textChanged(const QString &arg1);

    void on_ik_checkBox_stateChanged(int arg1);

    void on_csv_checkBox_stateChanged(int arg1);

    void on_video_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Dialog dialog;
    Model *model;
    QString m_settingFile;
    QThread workerThread;
};

#endif // MAINWINDOW_H
