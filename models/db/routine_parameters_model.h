#ifndef MODELS_DB_ROUTINE_PARAMETERS_MODEL_H
#define MODELS_DB_ROUTINE_PARAMETERS_MODEL_H

#include <QAbstractItemModel>

namespace meow {

namespace db {
    class RoutineEntity;
}

namespace models {

namespace forms {
    class RoutineForm;
}

namespace db {

class RoutineParametersModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum class Columns {
        Number = 0,
        Name,
        DataType,
        Context,
        Count
    };

    explicit RoutineParametersModel(meow::models::forms::RoutineForm * parent);

    void setRoutine(meow::db::RoutineEntity * routine);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;

    bool canAddParam() const;
    bool canRemoveParam(const QModelIndex & curIndex) const;
    bool canRemoveAllParams() const;
    bool canMoveParamUp(const QModelIndex & curIndex) const;
    bool canMoveParamDown(const QModelIndex & curIndex) const;

    int insertEmptyDefaultParam(int afterIndex = -1);
    bool removeParamAt(int index);
    void removeAllParams();
    bool moveParamUp(int index);
    bool moveParamDown(int index);

    Q_SIGNAL void modified();

private:

    bool isEditingSupported() const;

    bool setEditData(const QModelIndex &index, const QVariant &value);
    bool isEditingAllowed(int row, int col) const;

    QString textDataAt(int row, int col) const;
    QVariant editDataAt(int row, int col) const;

    void removeData();
    void insertData();

    meow::models::forms::RoutineForm * _form;
    meow::db::RoutineEntity * _routine;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_DB_ROUTINE_PARAMETERS_MODEL_H
