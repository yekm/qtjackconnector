#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class JackModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateView();
private:
    Ui::MainWindow *ui;

    JackModel *m_jack_model;
};

#endif // MAINWINDOW_H
