#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>//菜单栏
#include <QMenu>//菜单
#include<QTextEdit>
#include <QAction>//动作
#include <QDebug>
#include<QFileDialog>
#include<QDir>

#include<fstream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "yufa.h"
#include"youhua.h"
#include"Object_code.h"
#include"main_structure.h"
#include"Symbol_table.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
    ui->setupUi(this);
    myui = ui;
    QMenuBar *  MenuBar = menuBar();

    QMenu *   fileMenu = MenuBar->addMenu("file");
    QAction *fopen = fileMenu->addAction("open");
    QAction *fsave = fileMenu->addAction("save");

    QMenu *   compileMenu = MenuBar->addMenu("compile");
    QAction *run = compileMenu->addAction("run");

    connect(fopen,&QAction::triggered,
            [=](){
            fileopen();}
            );
    connect(fsave,&QAction::triggered,
            [=](){
            filesave();}
            );
    connect(run,&QAction::triggered,
            [=](){
            runcompile();}
            );
}
MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_textEdit_textChanged()
//{
//    on_textEdit_cursorPositionChanged();

//}

//void MainWindow::on_textEdit_cursorPositionChanged()
//{
//    ui->textEdit_2->append("changed");
//    int lastx =  ui->textEdit->cursorRect().x();
//    int lasty =  ui->textEdit->cursorRect().y();
//    int lastpos = ui->textEdit->textCursor().position();
//    qDebug()<<ui->textEdit->cursor().pos().x()<<' '<<ui->textEdit->cursor().pos().y()<<ui->textEdit->textCursor().position();

//    QString to_find_text="main";

//        while(ui->textEdit->find(to_find_text,QTextDocument::FindBackward)){
//            qDebug()<<"pos:"<<ui->textEdit->textCursor().position();
//            QTextCursor find_cursor=ui->textEdit->textCursor();
//            QTextCharFormat plainFormat(find_cursor.charFormat());
//            QTextCharFormat colorFormat = plainFormat;
//            colorFormat.setForeground(Qt::red);
//            ui->textEdit->mergeCurrentCharFormat(colorFormat);
//            ui->textEdit->textCursor().setPosition(lastpos);
//            //ui->textEdit->cursor().setPos(lastx,lasty);
//           // QPalette palette=ui->textEdit->palette();
//            //palette.setColor(QPalette::Highlight,QColor(107,194,53));
//            //ui->textEdit->setPalette(palette);
//        }
//}

bool MainWindow::fileopen(){
    //定义文件对话框类
        QFileDialog *fileDialog = new QFileDialog(this);

        fileDialog->setWindowTitle(tr("open source file"));//定义文件对话框标题

        fileDialog->setDirectory(".");//设置默认文件路径

        fileDialog->setNameFilter(tr("only *.txt"));//设置文件过滤器

        fileDialog->setFileMode(QFileDialog::ExistingFiles);//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles

        fileDialog->setViewMode(QFileDialog::Detail);//设置视图模式

        QStringList fileNames;//打印所有选择的文件的路径
        if(fileDialog->exec())
        {
            fileNames = fileDialog->selectedFiles();
        }
//        for(auto tmp:fileNames)
//            qDebug()<<tmp<<endl;
        QString displayString ;
        QString filename =  fileNames.first();
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
            return false;
        }
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            qDebug()<< str;
            displayString.append(str);
        }
        ui->textEdit->setText(displayString);
        return true;
}

bool MainWindow::filesave(){
    if(ui->textEdit->document()->isEmpty()){
        ui->textEdit_2->append("please input some codes!\n");
        return false;
    }
    else{
        source = ui->textEdit->toPlainText();
        QStringToTxt(source,("./data0.txt"));
        //DeNotes();
        return true;
    }
}
bool MainWindow::runcompile(){
    if(filesave()){
            initTerm();
            FuncL_p2 = FuncL_Head;
            ArrL_p2 = ArrL_Head;
            SymL_p = SymL_Head[0];
            Symbol_List* p = SymL_Head[1];
            if (!infile)
            {
                ui->textEdit_2->append("error: cannot open the file!\n");
                return false;
            }
            Term* arrow = Head;
            arrow = Start(arrow);
            ui->textEdit_2->append("Grammatical analysis passed!\n");
            Print_Table();
            QtToFile();//四元式输出到文件
            DivideBaseblock();//优化
            NewQtToFile();//优化四元式输出到文件
            active_info();
            objectcode_asm(0, NewQt.size());       //汇编代码生成
            printtoken();//token输出
            obcode_to_file();//汇编输出到文件
            return true;
    }


}


int MainWindow::QStringToTxt(QString s,QString txt)
{
    QFile file(txt);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "open txt is fail! txt maybe exist";
        return 0;
    }
    QTextStream filestream(&file);
    int i = 0;

    while(i<s.length())
    {
        if(s[i]!='\n')
        {
            filestream << s[i];
        }
        else
        {
        filestream << "\n";
        }
        i++;
    }
    file.close();
    return 1;

}

