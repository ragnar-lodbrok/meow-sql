#ifndef MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
#define MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H

#include <QList>
#include <QVariant>

namespace meow {

namespace db {
class TableIndex;
}

namespace ui {
namespace models {


// Intent: table indexes (tree) model item
// These classes should not contain any data, only refer it, so we can
// easily modify their order/data, thus we don't pass rows and strings.
// We may (pbly) get rid of these classes, but this approach worked for me before.

class ITableIndexesModelItem
{
public:

    enum class Type {
        None,
        Index,
        Column,
        Root = None
    };

    explicit ITableIndexesModelItem(ITableIndexesModelItem * parent = nullptr);
    virtual ~ITableIndexesModelItem();

    ITableIndexesModelItem * parent() const { return _parent; }

    virtual QString data(int col) const {
        Q_UNUSED(col);
        return QString();
    }
    virtual int childCount() const { return 0; }

    virtual ITableIndexesModelItem * child(int row) const {
        Q_UNUSED(row);
        return nullptr;
    }
    virtual int row() const;
    virtual Type type() const { return Type::None; }

    virtual int rowOf(ITableIndexesModelItem * child) const {
        Q_UNUSED(child);
        return 0;
    }
    virtual QVariant icon() const { return QVariant(); }

    virtual meow::db::TableIndex * tableIndex() const { return nullptr; }

protected:
    ITableIndexesModelItem * _parent;
};

// -----------------------------------------------------------------------------

// Item of type Index
class TableIndexesModelItemIndex : public ITableIndexesModelItem
{
public:
    TableIndexesModelItemIndex(meow::db::TableIndex * index,
                               ITableIndexesModelItem * parent = nullptr);

    ~TableIndexesModelItemIndex() override;

    virtual QString data(int col) const override;

    virtual int rowOf(ITableIndexesModelItem * child) const override;

    virtual int childCount() const override { return _columns.size(); }

    virtual ITableIndexesModelItem * child(int row) const override {
        return _columns.value(row);
    }
    virtual Type type() const override { return Type::Index; }

    virtual QVariant icon() const override;

    virtual meow::db::TableIndex * tableIndex() const override {
        return _index;
    }

    void reinitColumns();
    void removeColumnAt(int index);
    ITableIndexesModelItem * addColumnAt(int index);

private:
    meow::db::TableIndex * _index;
    QList<ITableIndexesModelItem *> _columns;
};


// -----------------------------------------------------------------------------

// Item of type index's column
class TableIndexesModelItemColumn : public ITableIndexesModelItem
{
public:
    TableIndexesModelItemColumn(meow::db::TableIndex * index,
                                ITableIndexesModelItem * parent = nullptr);

    virtual QString data(int col) const override;

    virtual Type type() const override { return Type::Column; }

    virtual QVariant icon() const override;

    virtual meow::db::TableIndex * tableIndex() const override {
        return _index;
    }

private:
    meow::db::TableIndex * _index;
};

} // namespace models
} // namespace ui
} // namespace meow

#endif // MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
