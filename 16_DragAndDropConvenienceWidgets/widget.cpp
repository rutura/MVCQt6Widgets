#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //Populate the list view
    ui->listWidget->addItems(QColor::colorNames());
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setAcceptDrops(true);
    ui->listWidget->setDropIndicatorShown(true);
    
    //Move the data and don't create a copy
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);// Move the data

    //Data for the table
    table.append({"John","Doe","32","Farmer","Single","Gounduana","Mestkv","89"});
    table.append({"Mary","Jane","27","Teacher","Married","Verkso","Tukk","55"});
    table.append({"Donald","Milton","32","Farmer","Single","Gounduana","Mestkv","67"});
    table.append({"Samatha","Chan","27","Teacher","Married","Verkso","Tukk","78"});
    table.append({"Andrew","Knut","32","Farmer","Single","Gounduana","Mestkv","51"});

    QStringList labels;
    labels << "First Name"<<"Last Name"<<"Age"<<"Proffession"<<"Marital Status"
           <<"Country"<<"City"<<"Social Score";
    ui->tableWidget->setHorizontalHeaderLabels(labels);


    int rows = table.size();
    int columns = table[0].size();

    for( int row = 0; row < rows ; ++ row){
        newRow(); // Create a new row with emtpy cells of data
        for( int col =0; col < columns; ++col){
            ui->tableWidget->item(row,col)->setText(table[row][col]);
            ui->tableWidget->item(row,col)->setData(Qt::ToolTipRole,QString("item [%1,%2]")
                                                                          .arg(row).arg(col));

        }
    }

    //Configure the table widget for drag/drop operations
    ui->tableWidget->setDragEnabled(true);
    ui->tableWidget->setAcceptDrops(true);
    ui->tableWidget->setDropIndicatorShown(true);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::newRow()
{
    int row = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(row);

    QTableWidgetItem * first = nullptr;

    for( int i = 0 ; i < 8 ; i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        if( i == 0)
            first = item;
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, i, item);
    }

    if(first)
        ui->tableWidget->setCurrentItem(first);
}
