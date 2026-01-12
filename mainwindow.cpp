#include "mainwindow.h"

#include "ttsviewmodel.h"

#include "articleextractor.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_vm = new TtsViewModel(this);

    QWidget* root = new QWidget(this);
    QGridLayout* grid = new QGridLayout(root);

    m_txt1 = new QTextEdit(root);
    m_txt1->setText("Making a Raspberry Pi web application.");


    QPushButton* m_btn1 = new QPushButton("Speak",root);
    QPushButton* m_btn2 = new QPushButton("Stop",root);
    QLabel* m_lbl1 = new QLabel("Status: Stopped",root);
    QLineEdit* m_ln1 = new QLineEdit("https://www.geeksforgeeks.org/system-design/software-design-patterns/",root);
    QPushButton* m_btn3 = new QPushButton("Send",root);

    grid->addWidget(m_ln1,0,0);
    grid->addWidget(m_btn3,0,1);
    grid->addWidget(m_txt1,1,0,1,2);
    grid->addWidget(m_btn1,2,0);
    grid->addWidget(m_btn2,2,1);
    grid->addWidget(m_lbl1,3,0,1,2);

    setCentralWidget(root);

    connect(m_btn1,&QPushButton::clicked,this,[this](){
        QString txt = m_txt1->toPlainText();
        if( !txt.isEmpty() )
            m_vm->speak(txt.toUtf8());
    });

    connect(m_btn2,&QPushButton::clicked,this,[this](){
        if( m_vm->speaking() )
            m_vm->stop();
    });

    connect(m_vm,&TtsViewModel::htmlReceived,this,[this](const QString& output){
        // m_browser->setHtml(output);
        m_txt1->setHtml(output);

        QApplication::restoreOverrideCursor();
    });

    connect(m_btn3,&QPushButton::clicked,this,[this,m_ln1](){
        QApplication::setOverrideCursor(Qt::WaitCursor);
        m_txt1->setHtml("<i>Loading article…</i>");
        m_vm->sendHtmlRequest(QUrl(m_ln1->text()));
    });

    connect(m_vm,&TtsViewModel::speakingChanged,[m_lbl1](bool speaking){
        m_lbl1->setText(QString("Status: %1").arg(speaking ? "Speaking" : "Stopped"));

    });





}

MainWindow::~MainWindow() {}
