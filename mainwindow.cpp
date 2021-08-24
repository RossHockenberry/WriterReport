
#include "myinclude.h"
#include "mlocal.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        return;
}

MainWindow::~MainWindow()
{
        return;
}

bool MainWindow::OnCreate()
{
        InitObject();
        return true;
}

bool MainWindow::InitObject()
{
        pDb     =   new MyDataClass();

        return true;
}

bool MainWindow::InitWidgets()
{

        return true;
}

bool MainWindow::SetSlots()
{

        return true;
}
