#ifndef UI_CENTRAL_RIGHT_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_WIDGET_MODEL_H


// Main Window
//   Central Right Widget
//      Root Model

#include <QString>
#include <QIcon>
#include "db/entity/entity_holder.h"

namespace meow {

namespace db {
class UserQuery;
}

namespace models {
namespace ui {

// TODO: get rid of CentralRight or move into namespace ?

enum class CentralRightWidgetTabs {
    Host,
    Database,
    Entity, // Table, View, etc
    Data
};

class CentralRightWidgetModel
{
public:
    CentralRightWidgetModel();

    bool setCurrentEntity(const meow::db::EntityPtr & entity);
    meow::db::Entity * currentEntity() const;

    QString titleForHostTab() const;
    QString titleForDatabaseTab() const;
    QString titleForEntityTab() const;
    QString titleForTableTab() const;
    QString titleForViewTab() const;
    QString titleForRoutineTab() const;
    QString titleForTriggerTab() const;
    QString titleForDataTab() const;
    QString titleForQueryTab(int index) const;

    QIcon iconForRoutineTab() const;

    int indexForFirstQueryTab() const;
    int indexForDataTab() const;

    bool connectionChanged() const;
    bool databaseChanged() const;
    bool hasDatabase() const;
    bool hasDataTab() const;
    bool hasEntityTab() const;
    bool hasQueryTab() const;
    bool isQueryTab(int index) const;

    void setCurrentTabIndex(int index) {
        _currentTabIndex = index;
    }
    int currentTabIndex() const {
        return _currentTabIndex;
    }

    int userQueriesCount() const;
    meow::db::UserQuery * userQueryAt(size_t index);
    meow::db::UserQuery * appendNewUserQuery();
    bool removeUserQueryAt(size_t index);
    void onUserQueryTextEdited(size_t index, const QString & query);
    QString userQueryTextAt(size_t index);
    void backupUserQueries();

private:
    meow::db::EntityHolder _entityHolder;
    int _currentTabIndex;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_WIDGET_MODEL_H
