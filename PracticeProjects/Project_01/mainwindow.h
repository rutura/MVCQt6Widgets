#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addStudentButton_clicked();
    void on_clearAllButton_clicked();
    void on_tableWidget_itemChanged(QTableWidgetItem *item);
    void updateAverageScore();
    bool validateScore(const QString &scoreText, double &score);

private:
    Ui::MainWindow *ui;
    QLineEdit *nameEdit;
    QLineEdit *subjectEdit;
    QLineEdit *scoreEdit;
    QTableWidget *tableWidget;
    QLabel *averageScoreLabel;
};

#endif // MAINWINDOW_H
