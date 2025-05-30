#include "personmodel.h"

PersonModel::PersonModel(QObject *parent)
    : QAbstractTableModel{parent}
{

    //Populate with initial data
    persons.append (new Person("Jamie Lannister","red",33));
    persons.append(new Person("Marry Lane","cyan",26));
    persons.append(new Person("Steve Moors","yellow",44));
    persons.append(new Person("Victor Trunk","dodgerblue",30));
    persons.append(new Person("Ariel Geeny","blue",33));
    persons.append(new Person("Knut Vikran","lightblue",26));
}

PersonModel::~PersonModel(){
    qDeleteAll(persons);
}


//Go through insertRows and removeRows
void PersonModel::addPerson(Person* person){
    insertRows(persons.size(),1,QModelIndex());
    setData(index(persons.size()- 1, 0), person->names(), Qt::EditRole);
    setData(index(persons.size()- 1, 1), person->favoriteColor(), Qt::EditRole);
    setData(index(persons.size()- 1, 2), person->age(), Qt::EditRole);
}

void PersonModel::addPerson(){
    Person *person = new Person("Added Person","yellowgreen",45,this);
    addPerson(person);
}

void PersonModel::addPerson(const QString& names, const int age){
    Person *person=new Person(names,"yellowgreen",age);
    addPerson(person);
}


void PersonModel::removePerson(QModelIndex index){
    if(index.isValid()){
        removeRows(index.row(), 1, QModelIndex());
    }
}

//QAbstractItemModel interface
int PersonModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return persons.size();
}

int PersonModel::columnCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return 3;
}


QVariant PersonModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= persons.count())
        return QVariant();

    Person* person = persons[index.row()];

    if(role == Qt::DisplayRole || role == Qt::EditRole){
        if(index.column() == 0){
            //Names
            return person->names();
        }

        if(index.column() == 1){
            //Age
            return person->age();
        }

        if(index.column() == 2){
            return person->favoriteColor();
        }
    }

    if(role == Qt::ToolTipRole || role == NamesRole){
        return person->names();
    }

    if(role == FavoriteColorRole){
        return person->favoriteColor();
    }

    if(role == AgeRole){
        return person->age();
    }

    return QVariant();
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role ) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    if(orientation == Qt::Horizontal){
        switch (section) {
        case 0:
            return QString("Names");
            break;
        case 1:
            return QString("Age");
            break;

        case 2:
            return QString("Favorite color");
            break;
        }
    }

    //Vertical headers
    return QString("Person %1").arg(section);
}


//Methods to make the model editable
bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role){

    if(!index.isValid()){
        return false;
    }

    Person * person = persons[index.row()];
    bool somethingChanged {false};

    switch (role) {

        case Qt::EditRole:
        {
        if(index.column() == 0){
            //Change the names
            if( person->names()!= value.toString()){
                person->setNames(value.toString());
                somethingChanged = true;
            }
        }

        if(index.column() == 1){
            //Change the age
            if(person->age()!=value.toInt()){
                person->setAge(value.toInt());
                somethingChanged  = true;
            }
        }

        if(index.column() == 2){
            //Change the favorite color
            if(person->favoriteColor()!= value.toString()){
                person->setFavoriteColor(value.toString());
                somethingChanged  = true;

            }
        }



        }
        break;
        case NamesRole:

        {

            if(person->names() != value.toString()){
                person->setNames(value.toString());
                somethingChanged = true;
            }
        }
        break;

        case AgeRole:
        {

            if(person->age() != value.toInt()){
                person->setAge(value.toInt());
                somethingChanged = true;
            }
        }
        break;


        case FavoriteColorRole:
        {
            if(person->favoriteColor() != value.toString()){
                person->setFavoriteColor(value.toString());
                somethingChanged = true;
            }
        }

        if(somethingChanged){
            emit dataChanged(index,index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const{

    if(!index.isValid()){
        return QAbstractTableModel::flags(index);
    }
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool PersonModel::insertRows(int row, int count, const QModelIndex& index) {

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for(int i = 0; i < count; ++i){
        persons.insert(row, new Person());  //insertRows will create a new blank person.
    }

    endInsertRows();
    return true;
}


bool PersonModel::removeRows(int row, int count, const QModelIndex& index) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    persons.removeAt(row);

    endRemoveRows();
    return true;
}

QHash<int, QByteArray> PersonModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NamesRole] = "names";
    roles[FavoriteColorRole] = "favoritecolor";
    roles[AgeRole] = "age";
    return roles;
}





















