#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void addFolder();
    void showContextMenu(const QPoint &pos);
    void renameFolder();
    void deleteFolder();

private:
    Ui::Widget *ui;
    QTreeWidgetItem *createFolder(const QString &name);
    void setupTreeWidget();
    void setupContextMenu();
};
#endif // WIDGET_H
