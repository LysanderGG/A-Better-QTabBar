#include "mainwindow.h"

#include "GGTabBar.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    GGTabBarWidget* tbw = new GGTabBarWidget(this);
    tbw->addTab("Tab1");
    tbw->addTab("Tab2");
    tbw->addTab("Tab3");
    tbw->addTab("Tab4");
    tbw->addTab("Tab5");
    tbw->addTab("Tab6");

    this->setCentralWidget(tbw);
}

MainWindow::~MainWindow()
{

}
