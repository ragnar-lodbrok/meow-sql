#ifndef MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
#define MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H

#include <QObject>

namespace meow {

namespace db {
class TableIndex;
}

namespace models {
namespace forms {

class ITableIndexesModelItem //: public QObject
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

    virtual QString data() const { return QString(); }
    virtual int childCount() { return 0; }

    virtual ITableIndexesModelItem * child(int row) {
        Q_UNUSED(row);
        return nullptr;
    }
    virtual int row() const { return 0; }
    virtual Type type() const { return Type::None; }
    //virtual QVariant icon() const { return QVariant(); }

protected:
    ITableIndexesModelItem * _parent;
};

class TableIndexesModelItemIndex : public ITableIndexesModelItem
{
    TableIndexesModelItemIndex(db::TableIndex * index,
                               ITableIndexesModelItem * parent = nullptr);

    //~TableIndexesModelItemIndex();
private:
    db::TableIndex * _index;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TABLE_INDEXES_MODEL_ITEM_H
