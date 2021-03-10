#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextBlock>
#include <QEvent>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/logo/Basic.jpg"));
    setWindowTitle("MiniBasic");

    ui->orders->installEventFilter(this);
    ui->ClearButton->setStyleSheet("QPushButton{font-family:'Consolas';font-size:12pt;color:rgb(255,250,250);}\
                                   QPushButton{background-color:rgb(25,25,25)}\
                                   QPushButton:hover{background-color:rgb(80, 80, 80)}");
    ui->LoadButton->setStyleSheet("QPushButton{font-family:'Consolas';font-size:12pt;color:rgb(255,250,250);}\
                                    QPushButton{background-color:rgb(25,25,25)}\
                                    QPushButton:hover{background-color:rgb(80, 80, 80)}");
    ui->RunButton->setStyleSheet("QPushButton{font-family:'Consolas';font-size:12pt;color:rgb(255,250,250);}\
                                    QPushButton{background-color:rgb(25,25,25)}\
                                    QPushButton:hover{background-color:rgb(80, 80, 80)}");
    ui->SaveButton->setStyleSheet("QPushButton{font-family:'Consolas';font-size:12pt;color:rgb(255,250,250);}\
                                  QPushButton{background-color:rgb(25,25,25)}\
                                  QPushButton:hover{background-color:rgb(80, 80, 80)}");
    connect(ui->LoadButton,&QPushButton::clicked,this,&MainWindow::loadProgram);
    connect(ui->RunButton,&QPushButton::clicked,this,&MainWindow::runProgram);
    connect(ui->ClearButton,&QPushButton::clicked,this,&MainWindow::clear);
    connect(ui->SaveButton,&QPushButton::clicked,this, &MainWindow::saveCodes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runProgram() {
    try {
        ui->result->clear();
        ui->tree->clear();
        ui->Tips->clear();
        Program.clear();
        for(QMap<int, QString>::iterator ite = codes.begin(); ite != codes.end();) {
            if(Program.executeLine == 0) {
                QStringList cmdParts = (*ite).split(' ');
                Program.executeCommand(cmdParts);
                if(Program.newResult) {
                    ui->result->append(QString::number(Program.result));
                    Program.newResult = false;
                }
                if(Program.ifInput) {
                    askForInput(Program.inputIden);
                }
                if(Program.ifEnd || (ite+1) == codes.end()) {
                    ui->tree->clear();
                    for(auto string: Program.getTree()) {
                        ui->tree->append(string);
                    }
                    ui->Tips->setText("<font color=\"#00ff00\">ENDED NORMALLY");
                    break;
                }
                ite++;
            }
            else {
                if(codes.contains(Program.executeLine)) {
                    ite = codes.find(Program.executeLine);
                }
                else {
                    throw "NO MATCHING LINENUMBER";
                }
                Program.executeLine = 0;
                continue;
            }
        }
        ui->Tips->setText("<font color=\"#00ff00\">ENDED NORMALLY");
    }  catch (const char* error) {
        ui->Tips->setText(error);
    }
}

void MainWindow::loadProgram() {
    try {
        QString fileName;
        fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File (*.txt)"));
        if(fileName == "") {
            return;
        }
        else {
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw "CANNOT OPEN FILE";
            }
            else {
                QTextStream textStream(&file);
                while (!textStream.atEnd()) {
                    QString command = textStream.readLine();
                    if(command[0] >= '0' && command[0] <= '9') {
                        int number = 0;
                        QString charNum;
                        for(int i = 0; i < command.length() && command[i] >= '0' && command[i] <= '9'; i++) {
                            charNum.append(command[i]);
                        }
                        number = charNum.toInt();

                        if(command.length() != charNum.length()) {
                            codes.insert(number, command);
                        }
                    }
                }
                file.close();
                ui->codes->clear();
                for(auto value: codes) {
                    ui->codes->append(value);
                }
                ui->Tips->setText("<font color=\"#00ff00\">OPEN FILE SUCCESSFULLY");
            }
        }
    }  catch (const char * error) {
        ui->Tips->setText(error);
    }

}

void MainWindow::saveCodes() {
    try {
        if(ui->codes->toPlainText() == "") {
            throw "THE CONTENT CANNOT BE EMPTY";
        }
        else {
            QFileDialog fileDialog;
            QString str = fileDialog.getSaveFileName(this,tr("Open File"),"/newFile",tr("Text File(*.txt)"));
            if(str == "") {
                return;
            }
            QFile filename(str);
            if(!filename.open(QIODevice::WriteOnly | QIODevice::Text)) {
                throw "FAILED TO OPEN FILE";
            }
            else {
                QTextStream textStream(&filename);
                for(auto string: codes) {
                    textStream << string;
                    textStream << "\n";
                }
                filename.close();
                ui->Tips->setText("<font color=\"#00ff00\">SAVE FILE SUCCESSFULLY");
            }

        }
    }  catch (const char* error) {
        ui->Tips->setText(error);
    }

}

void MainWindow::listCodes() {

}

void MainWindow::clear() {
    codes.clear();
    ui->orders->clear();
    ui->codes->clear();
    ui->result->clear();
    ui->Tips->clear();
    ui->tree->clear();
    Program.clear();
}

void MainWindow::askForInput(QString InputIdenName) {
    QString tip = "Please Input ";
    tip.append(InputIdenName);

    bool ok = false;
    QString text = QInputDialog::getText(this, tr("Input"),
        tip, QLineEdit::Normal,
        "", &ok, Qt::CustomizeWindowHint);
    if (ok && !text.isEmpty()) {
        Program.ifInput = false;
        Program.ChangeContext(InputIdenName, text.toInt());
    }
    else {
        throw "NO INPUT";
    }
}

void MainWindow::handleReturnPressed() {
    ui->Tips->clear();
    QString cmd = ui->orders->toPlainText().trimmed();

    if(cmd[0] >= '0' && cmd[0] <= '9') {
        int number = 0;
        QString charNum;
        for(int i = 0; i < cmd.length() && cmd[i] >= '0' && cmd[i] <= '9'; i++) {
            charNum.append(cmd[i]);
        }
        number = charNum.toInt();

        if(cmd.length() != charNum.length()) {
            codes.insert(number, cmd);

            ui->codes->clear();
            for(auto value: codes) {
                ui->codes->append(value);
            }
        }
        else {
            codes.remove(number);

            ui->codes->clear();
            for(auto value: codes) {
                ui->codes->append(value);
            }
        }
    }
    else {
        if(cmd == "RUN") {
            ui->orders->clear();
            runProgram();
            return;
        }
        if(cmd == "LOAD") {
            ui->orders->clear();
            loadProgram();
            return;
        }
        if(cmd == "LIST") {
            ui->orders->clear();
            listCodes();
            return;
        }
        if(cmd == "CLEAR") {
            clear();
            return;
        }
        if(cmd == "SAVE") {
            ui->orders->clear();
            saveCodes();
            return;
        }
        if(cmd == "QUIT") {
            this->close();
        }
        ui->Tips->setText("INVALID INSTRUCTION");
    }
    ui->orders->clear();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->orders && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if(keyEvent -> key() == Qt::Key_Return) {
            handleReturnPressed();
            return true;
        }
    }
    return QObject::eventFilter(object, event);
}

