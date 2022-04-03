#ifndef UI_PRESENTERS_CR_DATA_FILTER_FORM_H
#define UI_PRESENTERS_CR_DATA_FILTER_FORM_H

#include <QObject>
#include <QString>

namespace meow {
namespace db {
    class Entity;
}

namespace ui {
namespace presenters {

class CentralRightDataFilterForm : public QObject
{
public:

    CentralRightDataFilterForm();

    void setDBEntity(db::Entity * tableOrViewEntity);

    bool hasRecentFilters() const;

    QStringList recentFilters() const;

    void setFilterEditText(const QString & text);

    QString SQLFiltertext() const;

private:

    QString SQLByFilterText(const QString & text);

    db::Entity * _entity;
    QString _sqlFilterText;

};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_PRESENTERS_CR_DATA_FILTER_FORM_H

