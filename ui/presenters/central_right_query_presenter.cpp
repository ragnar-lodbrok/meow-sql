#include "central_right_query_presenter.h"
#include "db/user_query/user_query.h"
#include "db/user_query/sentences_parser.h"
#include "db/connection_query_killer.h"
#include "helpers/formatting.h"

namespace meow {
namespace ui {
namespace presenters {

CentralRightQueryPresenter::CentralRightQueryPresenter(db::UserQuery * query)
    : _query(query)
    , _filterPatternIsRegexp(false)
{
    Q_ASSERT(_query != nullptr);
}

bool CentralRightQueryPresenter::isRunning() const
{
    return _query->isRunning();
}

bool CentralRightQueryPresenter::execQueries(
        const QString & SQL, int charPosition)
{
    meow::db::user_query::SentencesParser parser;
    QList<meow::db::user_query::Sentence> sentences
        = parser.parseByDelimiter(SQL);

    QStringList queries;
    for (const meow::db::user_query::Sentence & sentence : sentences) {

        if (charPosition == -1) {
            queries << sentence.text;
        } else {
            if (sentence.position <= charPosition
                    && charPosition <= (sentence.position + sentence.text.length())) {
                queries << sentence.text;
                break;
            }
        }
    }

    if (queries.isEmpty()) return false;

    _query->runInCurrentConnection(queries);

    return true;
}

bool CentralRightQueryPresenter::hasError() const
{
    return !_query->lastError().isEmpty();
}

QString CentralRightQueryPresenter::lastError() const
{
    return _query->lastError();
}

int CentralRightQueryPresenter::resultsDataCount() const
{
    return _query->resultsDataCount();
}

db::QueryDataPtr CentralRightQueryPresenter::resultsDataAt(int index) const
{
    return _query->resultsDataAt(index);
}

QString CentralRightQueryPresenter::resultTabCaption(int index) const
{
    meow::db::QueryDataPtr queryData = _query->resultsDataAt(index);
    QString caption =  QObject::tr("Result") + " #" + QString::number(index+1);
    caption += QString(" (%1×%2)")
            .arg(helpers::formatNumber(queryData->columnCount()))
            .arg(helpers::formatNumber(queryData->rowCount()));
    return caption;
}

bool CentralRightQueryPresenter::isCancelQueryActionEnabled() const
{
    db::Connection * connection = _query->lastRunningConnection();

    if (connection
            && connection->features()->supportsCancellingQuery()) {
        return isRunning();
    }
    return false;
}

bool CentralRightQueryPresenter::cancelQueries()
{
    if (!isRunning()) return true;

    _query->abort();

    db::Connection * connection = _query->lastRunningConnection();

    if (!connection) return true;

    db::ConnectionQueryKillerPtr killer = connection->createQueryKiller();

    try {
        killer->run();
    } catch(meow::db::Exception & ex) {
        _lastCancelError = ex.message();
        return false;
    }

    return true;
}

} // namespace presenters
} // namespace ui
} // namespace meow
