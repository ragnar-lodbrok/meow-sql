#ifndef MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
#define MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H

#include <QObject>
#include <QList>
#include <QVariant>

namespace meow {

namespace db {
class TableIndex;
}

namespace models {
namespace forms {


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
        Root = None,
        COUNT
    };

    explicit ITableIndexesModelItem(ITableIndexesModelItem * parent = nullptr);
    virtual ~ITableIndexesModelItem() {}

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
    virtual int row() const {
        if (_parent) {
            return _parent->rowOf(const_cast<ITableIndexesModelItem *>(this));
        }
        return 0;
    }
    virtual Type type() const { return Type::None; }

    virtual int rowOf(ITableIndexesModelItem * child) const {
        Q_UNUSED(child);
        return 0;
    }
    virtual QVariant icon() const { return QVariant(); }

protected:
    ITableIndexesModelItem * _parent;
};

// -----------------------------------------------------------------------------

// Item of type Index
class TableIndexesModelItemIndex : public ITableIndexesModelItem
{
public:
    TableIndexesModelItemIndex(db::TableIndex * index,
                               ITableIndexesModelItem * parent = nullptr);

    ~TableIndexesModelItemIndex();

    virtual QString data(int col) const override;

    virtual int rowOf(ITableIndexesModelItem * child) const override;

    virtual int childCount() const override { return _columns.size(); }

    virtual ITableIndexesModelItem * child(int row) const override {
        return _columns.value(row);
    }
    virtual Type type() const { return Type::Index; }

    virtual QVariant icon() const override;

    void reinitColumns();
private:
    db::TableIndex * _index;
    QList<ITableIndexesModelItem *> _columns;
};


// -----------------------------------------------------------------------------

// Item of type index's column
class TableIndexesModelItemColumn : public ITableIndexesModelItem
{
public:
    TableIndexesModelItemColumn(db::TableIndex * index,
                                ITableIndexesModelItem * parent = nullptr);

    virtual QString data(int col) const override;

    virtual Type type() const { return Type::Column; }

    virtual QVariant icon() const override;

private:
    db::TableIndex * _index;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
