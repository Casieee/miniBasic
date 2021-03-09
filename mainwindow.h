#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "program.h"
#include "statement.h"
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMap<int, QString> codes;
    program Program;
    int input;

    void handleReturnPressed();
    bool eventFilter(QObject *object, QEvent *event);

    void loadProgram();
    void askForInput(QString inputIdenName);
    void getInput(QString name);
    void clear();
    void listCodes();
    void saveCodes();
    void runProgram();
};
#endif // MAINWINDOW_H
