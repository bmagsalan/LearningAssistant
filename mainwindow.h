#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QtWidgets>

class ControllerTts;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ControllerTts* tts = nullptr;
    QThread* ttsThread = nullptr;
};
#endif // MAINWINDOW_H
