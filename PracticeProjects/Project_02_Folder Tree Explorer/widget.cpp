#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QStyle>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setupTreeWidget();
    setupContextMenu();

    connect(ui->addButton, &QPushButton::clicked, this, &Widget::addFolder);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setupTreeWidget()
{
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDropIndicatorShown(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
    
    // Add root folder
    QTreeWidgetItem* rootItem = createFolder("Root Folder");
    ui->treeWidget->addTopLevelItem(rootItem);
    rootItem->setExpanded(true);
}

void Widget::setupContextMenu()
{
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested,
            this, &Widget::showContextMenu);
}

QTreeWidgetItem* Widget::createFolder(const QString &name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    return item;
}

void Widget::addFolder()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("New Folder"),
                                             tr("Folder name:"), QLineEdit::Normal,
                                             tr("New Folder"), &ok);
    if (ok && !folderName.isEmpty()) {
        QTreeWidgetItem* newItem = createFolder(folderName);
        QTreeWidgetItem* current = ui->treeWidget->currentItem();
        
        if (current) {
            current->addChild(newItem);
            current->setExpanded(true);
        } else {
            ui->treeWidget->addTopLevelItem(newItem);
        }
    }
}

void Widget::showContextMenu(const QPoint &pos)
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(pos);
    if (!item) return;

    QMenu contextMenu(this);
    QAction* renameAction = contextMenu.addAction(tr("Rename"));
    QAction* deleteAction = contextMenu.addAction(tr("Delete"));

    connect(renameAction, &QAction::triggered, this, &Widget::renameFolder);
    connect(deleteAction, &QAction::triggered, this, &Widget::deleteFolder);

    contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
}

void Widget::renameFolder()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    if (!current) return;

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Folder"),
                                          tr("New folder name:"),
                                          QLineEdit::Normal,
                                          current->text(0), &ok);
    
    if (ok && !newName.isEmpty()) {
        current->setText(0, newName);
    }
}

void Widget::deleteFolder()
{
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    if (!current) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Delete Folder"),
                                tr("Are you sure you want to delete this folder and all its contents?"),
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        delete current;
    }
}

