#ifndef UI_CENTRAL_RIGHT_QUERY_PRESENTER_H
#define UI_CENTRAL_RIGHT_QUERY_PRESENTER_H

// Main Window
//   Central Right Widget
//      Query Tab

#include <memory>
#include <QString>
#include <QIcon>

namespace meow {

namespace db {

class UserQuery;
class QueryData;

using QueryDataPtr = std::shared_ptr<QueryData>;
}

namespace ui {
namespace presenters {

class CentralRightQueryPresenter
{
public:
    CentralRightQueryPresenter(meow::db::UserQuery * query);

    bool isRunning() const;

    meow::db::UserQuery * query() const {
        return _query;
    }

    bool execQueries(const QString & SQL, int charPosition = -1);

    bool hasError() const;

    QString lastError() const;

    int resultsDataCount() const;

    meow::db::QueryDataPtr resultsDataAt(int index) const;

    QString resultTabCaption(int index) const;

    bool isExecQueryActionEnabled() const {
        return !isRunning();
    }

    bool isExecCurrentQueryActionEnabled() const {
        return !isRunning();
    }

    bool isCancelQueryActionEnabled() const;

    // false on error
    bool cancelQueries();

    QString lastCancelError() const {
        return _lastCancelError;
    }

    void setFilterPattern(const QString & pattern, bool regexp) {
        _filterPattern = pattern;
        _filterPatternIsRegexp = regexp;
    }

    QString filterPattern() const { return _filterPattern; }
    bool filterPatternIsRegexp() const { return _filterPatternIsRegexp; }

private:
    meow::db::UserQuery * _query;
    QString _lastCancelError;

    QString _filterPattern;
    bool _filterPatternIsRegexp;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_PRESENTER_H
