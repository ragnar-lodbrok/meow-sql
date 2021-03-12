#include "trigger_structure_parser.h"
#include "db/entity/trigger_entity.h"
#include "helpers/logger.h"
#include "connection.h"

#include <QDebug>

namespace meow {
namespace db {

TriggerStructureParser::TriggerStructureParser(Connection * connection)
    :_connection(connection)
{

    _regexp.setPatternOptions(
        QRegularExpression::CaseInsensitiveOption
      | QRegularExpression::DotMatchesEverythingOption);

    QString quote = QString(_connection->getIdentQuote()); // TODO: escape

    QString pattern = R"(CREATE\s+)";

    // [DEFINER = user]
    pattern +=
            R"((DEFINER\s*=\s*(\S+|)"; // (DEFINER\s*=\s*(\S+|
    pattern += quote + "[^@" + quote + "]+"
            + quote + "@" + quote
            + "[^" + quote + "]+" + quote
            + R"()\s+)?)"; // )\s+)?

    // TRIGGER trigger_name { BEFORE | AFTER }
    pattern += R"(TRIGGER\s+(.+)\s+(BEFORE|AFTER)\s+)";

    // { INSERT | UPDATE | DELETE }
    pattern += R"((INSERT|UPDATE|DELETE)\s+)";

    // ON tbl_name FOR EACH ROW
    pattern += R"(ON\s+([^\n\r\t]*)\s+(FOR\s+EACH\s+ROW\s+))";

    // trigger_body
    pattern += R"((.+)$)";

    _regexp.setPattern(pattern);
}

void TriggerStructureParser::run(TriggerEntity * trigger)
{
    TriggerStructure * structure = trigger->structure();

    structure->clear();

    QString createSQL = trigger->createCode();

    QRegularExpressionMatch match = _regexp.match(createSQL);

    if (!match.hasMatch()) {
        meowLogCC(Log::Category::Error, _connection)
            << "Unable to parse trigger " << trigger->name();
        return;
    }

    if (_connection->connectionParams()->serverType() == ServerType::MySQL) {
        structure->setDefiner(
                    _connection->dequoteIdentifier(match.captured(2), '@'));
    }

    structure->setActionTime(match.captured(4).toUpper()); // before|after
    structure->setEvent(match.captured(5).toUpper()); // insert|...
    structure->setTableName(_connection->dequoteIdentifier(match.captured(6)));
    structure->setStatement(match.captured(8));

}

} // namespace db
} // namespace meow
