#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("BioMech");
    a.setOrganizationDomain("uottawa.biomech");
    a.setApplicationName("wrnchGUI");
    a.setApplicationVersion("0.0.1");
    Model* m=Model::getInstance();
    MainWindow w(nullptr,m);
    w.show();
    return a.exec();
}
