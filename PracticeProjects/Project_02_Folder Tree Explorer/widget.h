#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

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
    void createNewFolder();
    void showContextMenu(const QPoint &pos);
    void renameItem();
    void deleteItem();
    void openFile();
    void showProperties();
    void toggleHiddenFiles();
    void navigateUp();
    void navigateHome();
    void onPathChanged(const QString &path);
    void updatePathDisplay(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QFileSystemModel *fileModel;
    void setupFileSystem();
    void setupContextMenu();
    QString getSelectedPath() const;
    bool confirmDelete(const QString &path);
};
#endif // WIDGET_H
