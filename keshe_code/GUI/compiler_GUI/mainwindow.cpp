
#include "mainwindow.h"
#include <QMenuBar>//菜单栏
#include <QMenu>//菜单
#include <QAction>//动作
#include <QDebug>

bool compile(){
    qDebug()<<"start compiling...";
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 添加菜单栏
    QMenuBar *  MenuBar = menuBar();
    QMenu *   EditMenu = MenuBar->addMenu("edit");

    // 在文件的菜单上添加添加菜单项
    QAction *EM_compile = EditMenu->addAction("compile");//添加 "compile" 菜单项动作

    // 4. 新建菜单选项的触发动作信号和槽 使用Lambda表达式的匿名槽函数
    connect(EM_compile,&QAction::triggered,// "新建" 按下后的动作打印信息
            [=](){
            compile();}
            );

}

MainWindow::~MainWindow()
{
    delete ui;
}


