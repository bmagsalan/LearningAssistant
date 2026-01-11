#include "mainwindow.h"

#include "ttsviewmodel.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_vm = new TtsViewModel(this);

    QWidget* root = new QWidget(this);
    QGridLayout* grid = new QGridLayout(root);

    m_txt1 = new QTextEdit(root);
    // m_txt1->setText("Making a Raspberry Pi web application \"portable\" usually involves two distinct goals: Software Portability (easy to deploy on other Pis) and Operational Portability (turning the Pi into a standalone \"appliance\" you can carry around and use anywhere). Given your stack (uStreamer + Flask + Hardware Access), Docker is the industry standard for the software side, while Access Point Mode is the standard for the operational side. Here is the step-by-step guide to making your system portable.");
    m_txt1->setText("Making a Raspberry Pi web application.");
    QPushButton* m_btn1 = new QPushButton("Speak",root);
    QPushButton* m_btn2 = new QPushButton("Stop",root);
    QLineEdit* m_ln1 = new QLineEdit("Status: Stopped",root);
    grid->addWidget(m_txt1,0,0,1,2);
    grid->addWidget(m_btn1,1,0);
    grid->addWidget(m_btn2,1,1);
    grid->addWidget(m_ln1,2,0,1,2);

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

    connect(m_vm,&TtsViewModel::speakingChanged,[m_ln1](bool speaking){
        m_ln1->setText(QString("Status: %1").arg(speaking ? "Speaking" : "Stopped"));

    });




}

MainWindow::~MainWindow() {}
