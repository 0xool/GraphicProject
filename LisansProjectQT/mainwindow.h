#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_ExitBtn_clicked();

    void on_startAlgorithmBtn_clicked();

    void on_getGraphBtn_clicked();

    void on_getShapedBtn_clicked();

    void parseMeshData(QString text);

    void parseInnerShapeData(QString text);

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
