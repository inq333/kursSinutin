#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addEntry();
    void removeEntry();
    void saveFile();
    void openFile();

private:
    QTableWidget *tableWidget;

    void setupUI();
};

#endif // MAINWINDOW_H
