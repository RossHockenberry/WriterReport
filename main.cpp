#include "mainwindow.h"

#include "myinclude.h"
#include "mlocal.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


        a.setOrganizationName("REH");
        a.setOrganizationDomain("");
        a.setApplicationName("WriterData");
        a.setApplicationVersion("v 1.00 r .01");     //  Need to update this when neceassary.

//        w.setWindowState(Qt::WindowMaximized);  //  Open full screen for now.
        w.OnCreate();
        w.show();
        return a.exec();
}
