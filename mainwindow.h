#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QtWidgets>



class TtsViewModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
private:
    TtsViewModel* m_vm = nullptr;
    QTextEdit* m_txt1 = nullptr;
};

#endif // MAINWINDOW_H
