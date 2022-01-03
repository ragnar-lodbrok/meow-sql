#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_WIDGETMODEL_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_WIDGETMODEL_H

#include <QString>
#include <QIcon>

namespace meow {
namespace ui {
namespace presenters {

enum class RoutineInfoWidgetTabs {
    Options,
    Parameters
};


class RoutineInfoWidgetModel
{
public:
    RoutineInfoWidgetModel();
    const QString tabTitle(RoutineInfoWidgetTabs tab) const;
    const QIcon tabIcon(RoutineInfoWidgetTabs tab) const;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_WIDGETMODEL_H
