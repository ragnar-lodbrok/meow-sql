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
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;

private:

    QString textDataAt(int row, int col) const;

    void removeData();
    void insertData();

    meow::models::forms::RoutineForm * _form;
    meow::db::RoutineEntity * _routine;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_DB_ROUTINE_PARAMETERS_MODEL_H
