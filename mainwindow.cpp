#include "mainwindow.h"
#include "controllertts.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tts = ControllerTts::instance(nullptr);



    ttsThread = new QThread(this);
    tts->moveToThread(ttsThread);
    ttsThread->start();

    QMetaObject::invokeMethod(tts,"init",Qt::QueuedConnection);
    // tts->init();

    QWidget* root = new QWidget;
    QGridLayout* gridLayout = new QGridLayout;
    root->setLayout(gridLayout);

    QToolButton* btn1 = new QToolButton;
    btn1->setText(tr("Speak"));
    btn1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QTextEdit* txt1 = new QTextEdit;

    txt1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    gridLayout->addWidget(txt1,0,0);
    gridLayout->addWidget(btn1,1,0);

    gridLayout->setRowStretch(1,2);
    gridLayout->setRowStretch(0,8);



    connect(btn1,&QToolButton::clicked,tts,[&,txt1](){
        std::string text = txt1->toPlainText().toStdString();
        tts->speak(text);
    },Qt::ConnectionType::QueuedConnection);

    setCentralWidget(root);
}

MainWindow::~MainWindow() {}
