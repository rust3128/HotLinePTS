#include "treepcmodel.h"
#include "treeitem.h"
#include "LoggingCategories/loggingcategories.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>

TreePCModel::TreePCModel(uint ID, QObject *parent)
    : QAbstractItemModel(parent),
      objectID(ID)
{
    rootItem = new TreeItem({"","",""});
    setupModelData(rootItem);
}

TreePCModel::~TreePCModel()
{
    delete rootItem;
}


QModelIndex TreePCModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreePCModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreePCModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreePCModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant TreePCModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

QVariant TreePCModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

Qt::ItemFlags TreePCModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

void TreePCModel::setupModelData(TreeItem *parent)
{
    QVector<TreeItem*> parents;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    qInfo(logInfo()) << "Object ID in model" << objectID;

    QSqlQuery q;
    q.prepare("select t.pctype, p.pos_id, p.ipadress, p.vncpass, p.pcmodel_id, p.pcos_id, p.pc_id from pclist p "
               "left join pctype t on (t.pctype_id = p.pctype_id) "
               "where p.object_id = :objectID "
               "order by 2");
    q.bindValue(":objectID", objectID);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось получить список рабочх мест." << q.lastError().text();
        return;
    }
    QVector<QVariant> columnData;
    int position;
    while (q.next()) {
        position = 0;
        columnData.clear();
        columnData << q.value(0).toString() << q.value(1).toString() << q.value(6).toString();
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        columnData.clear();
        columnData << "IP" << q.value(2).toString()<< q.value(6).toString();
        parents << parents.last()->child(parents.last()->childCount()-1);
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        columnData.clear();
        columnData << "Модель" << q.value(4).toString()<< q.value(6).toString();
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        columnData.clear();
        columnData << "ОС" << q.value(5).toString()<< q.value(6).toString();
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        parents.pop_back();

    }
}
