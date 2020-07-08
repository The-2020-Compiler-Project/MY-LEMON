#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qobject.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool fileopen();
    bool filesave();
    bool runcompile();
    int QStringToTxt(QString s,QString txt);
    QString source;
    //void on_textEdit_cursorPositionChanged();

private slots:
    //void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H


