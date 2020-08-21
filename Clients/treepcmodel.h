#ifndef TREEPCMODEL_H
#define TREEPCMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreePCModel : public QAbstractItemModel
{
public:
    explicit TreePCModel(uint ID, QObject *parent = nullptr);
    ~TreePCModel();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    void setupModelData(TreeItem *parent);
    uint objectID;
    TreeItem *rootItem;
};

#endif // TREEPCMODEL_H
