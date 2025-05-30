#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QStyle>
#include <QDateTime>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , fileModel(new QFileSystemModel(this))
{
    ui->setupUi(this);
    setupFileSystem();
    setupContextMenu();

    connect(ui->addButton, &QPushButton::clicked, this, &Widget::createNewFolder);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &Widget::openFile);
    connect(ui->toggleHiddenButton, &QPushButton::clicked, this, &Widget::toggleHiddenFiles);
    connect(ui->upButton, &QPushButton::clicked, this, &Widget::navigateUp);
    connect(ui->homeButton, &QPushButton::clicked, this, &Widget::navigateHome);
    connect(ui->pathComboBox, &QComboBox::editTextChanged, this, &Widget::onPathChanged);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &Widget::updatePathDisplay);
            
    // Initialize home path
    navigateHome();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setupFileSystem()
{
    // Set the root path to show all drives
    fileModel->setRootPath("");
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    
    ui->treeView->setModel(fileModel);
    // Don't set a root index to show all drives
    ui->treeView->setRootIndex(QModelIndex());
    
    // Show all columns and adjust their sizes
    for (int i = 0; i < fileModel->columnCount(); ++i) {
        ui->treeView->setColumnWidth(i, 200);
    }

    // Enable drag and drop
    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);

    // Initialize to home directory
    QModelIndex homeIndex = fileModel->index(QDir::homePath());
    ui->treeView->setCurrentIndex(homeIndex);
    ui->treeView->scrollTo(homeIndex);
    updatePathDisplay(homeIndex);
}

void Widget::setupContextMenu()
{
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &Widget::showContextMenu);
}

void Widget::createNewFolder()
{
    QString path = getSelectedPath();
    if (path.isEmpty()) {
        path = QDir::homePath();
    }

    bool ok;
    QString folderName = QInputDialog::getText(this, tr("New Folder"),
                                             tr("Folder name:"), QLineEdit::Normal,
                                             tr("New Folder"), &ok);
    if (ok && !folderName.isEmpty()) {
        QDir dir(path);
        if (dir.mkdir(folderName)) {
            QModelIndex index = fileModel->index(path);
            ui->treeView->expand(index);
            ui->treeView->scrollTo(index);
        } else {
            QMessageBox::warning(this, tr("Error"),
                               tr("Failed to create folder. Check permissions."));
        }
    }
}

void Widget::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu contextMenu(this);
    QAction* openAction = contextMenu.addAction(tr("Open"));
    QAction* renameAction = contextMenu.addAction(tr("Rename"));
    QAction* deleteAction = contextMenu.addAction(tr("Delete"));
    QAction* propertiesAction = contextMenu.addAction(tr("Properties"));

    QString path = fileModel->filePath(index);
    QFileInfo fileInfo(path);
    
    // Only show Open for files
    openAction->setVisible(fileInfo.isFile());

    connect(openAction, &QAction::triggered, this, &Widget::openFile);
    connect(renameAction, &QAction::triggered, this, &Widget::renameItem);
    connect(deleteAction, &QAction::triggered, this, &Widget::deleteItem);
    connect(propertiesAction, &QAction::triggered, this, &Widget::showProperties);

    contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

void Widget::renameItem()
{
    QString path = getSelectedPath();
    if (path.isEmpty()) return;

    QFileInfo fileInfo(path);
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename"),
                                          tr("New name:"),
                                          QLineEdit::Normal,
                                          fileInfo.fileName(), &ok);
    
    if (ok && !newName.isEmpty()) {
        QFile file(path);
        QString newPath = fileInfo.absolutePath() + "/" + newName;
        if (!file.rename(newPath)) {
            QMessageBox::warning(this, tr("Error"),
                               tr("Failed to rename. Check permissions."));
        }
    }
}

void Widget::deleteItem()
{
    QString path = getSelectedPath();
    if (path.isEmpty()) return;

    if (confirmDelete(path)) {
        QFileInfo fileInfo(path);
        if (fileInfo.isDir()) {
            QDir dir(path);
            if (!dir.removeRecursively()) {
                QMessageBox::warning(this, tr("Error"),
                                   tr("Failed to delete folder. Check permissions."));
            }
        } else {
            QFile file(path);
            if (!file.remove()) {
                QMessageBox::warning(this, tr("Error"),
                                   tr("Failed to delete file. Check permissions."));
            }
        }
    }
}

void Widget::openFile()
{
    QString path = getSelectedPath();
    if (path.isEmpty()) return;

    QFileInfo fileInfo(path);
    if (fileInfo.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}

void Widget::showProperties()
{
    QString path = getSelectedPath();
    if (path.isEmpty()) return;

    QFileInfo fileInfo(path);
    QString type = fileInfo.isDir() ? "Folder" : "File";
    QString size = QString::number(fileInfo.size() / 1024.0, 'f', 2) + " KB";
    QString modified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    QString created = fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss");

    QString info = tr("Type: %1\nSize: %2\nModified: %3\nCreated: %4")
                      .arg(type)
                      .arg(size)
                      .arg(modified)
                      .arg(created);

    QMessageBox::information(this, tr("Properties"), info);
}

void Widget::toggleHiddenFiles()
{
    QDir::Filters filters = fileModel->filter();
    if (filters & QDir::Hidden) {
        filters &= ~QDir::Hidden;
    } else {
        filters |= QDir::Hidden;
    }
    fileModel->setFilter(filters);
}

QString Widget::getSelectedPath() const
{
    QModelIndex index = ui->treeView->currentIndex();
    return index.isValid() ? fileModel->filePath(index) : QString();
}

bool Widget::confirmDelete(const QString &path)
{
    QFileInfo fileInfo(path);
    QString message = fileInfo.isDir() ?
        tr("Are you sure you want to delete this folder and all its contents?") :
        tr("Are you sure you want to delete this file?");

    return QMessageBox::question(this, tr("Confirm Delete"),
                               message,
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void Widget::navigateUp()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QModelIndex parentIndex = currentIndex.parent();
    
    if (parentIndex.isValid()) {
        ui->treeView->setCurrentIndex(parentIndex);
        ui->treeView->scrollTo(parentIndex);
    } else {
        // If no valid parent, try to go up from the current directory
        QDir currentDir(fileModel->filePath(currentIndex));
        if (currentDir.cdUp()) {
            QModelIndex upIndex = fileModel->index(currentDir.absolutePath());
            ui->treeView->setCurrentIndex(upIndex);
            ui->treeView->scrollTo(upIndex);
        }
    }
}

void Widget::navigateHome()
{
    QString homePath = QDir::homePath();
    QModelIndex homeIndex = fileModel->index(homePath);
    ui->treeView->setCurrentIndex(homeIndex);
    ui->treeView->scrollTo(homeIndex);
    updatePathDisplay(homeIndex);
}

void Widget::onPathChanged(const QString &path)
{
    QString cleanPath = QDir::cleanPath(path);
    QFileInfo fileInfo(cleanPath);
    
    // Handle root paths like "D:" or "D:/"
    if (cleanPath.length() >= 2 && cleanPath.at(1) == ':') {
        // Ensure proper drive path format (e.g., "D:/")
        QString drivePath = cleanPath.left(2) + "/";
        QDir dir(drivePath);
        if (dir.exists()) {
            QModelIndex index = fileModel->index(drivePath);
            ui->treeView->setRootIndex(fileModel->index(drivePath));
            ui->treeView->setCurrentIndex(index);
            ui->treeView->scrollTo(index);
            return;
        }
    }
    
    // Handle normal paths
    if (fileInfo.exists() && fileInfo.isDir()) {
        QModelIndex index = fileModel->index(cleanPath);
        ui->treeView->setCurrentIndex(index);
        ui->treeView->scrollTo(index);
    }
}

void Widget::updatePathDisplay(const QModelIndex &index)
{
    if (!index.isValid()) return;
    
    QString path = fileModel->filePath(index);
    int idx = ui->pathComboBox->findText(path);
    
    if (idx == -1) {
        // Add path to history if it's not already there
        ui->pathComboBox->addItem(path);
        idx = ui->pathComboBox->count() - 1;
    }
    
    ui->pathComboBox->setCurrentIndex(idx);
}

