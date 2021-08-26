#include "mainwindow.h"

#include "myinclude.h"
#include "mlocal.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//        w.setWindowState(Qt::WindowMaximized);  //  Open full screen for now.
        w.OnCreate();
        w.show();
        return a.exec();
}
