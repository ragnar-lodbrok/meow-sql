#include "view_structure_parser.h"
#include "connection.h"
#include "entity/view_entity.h"
#include "helpers/logger.h"
#include <QDebug>

namespace meow {
namespace db {

ViewStructureParser::ViewStructureParser(Connection * connection)
    :_connection(connection)
{

    _regexp.setPatternOptions(
        QRegularExpression::CaseInsensitiveOption
      | QRegularExpression::InvertedGreedinessOption
      | QRegularExpression::DotMatchesEverythingOption); // AS is multiline in PG

    QString quote = QString(_connection->getIdentQuote()); // TODO: escape

    QString pattern
            = R"(CREATE\s+(OR\s+REPLACE\s+)?)"; // CREATE\s+(OR\s+REPLACE\s+)?
    pattern +=
            R"((ALGORITHM\s*=\s*(\w*)\s*)?)"; // (ALGORITHM\s*=\s*(\w*)\s*)?

    pattern +=
            R"((DEFINER\s*=\s*(\S+|)"; // (DEFINER\s*=\s*(\S+|
    pattern += quote + "[^@" + quote + "]+"
            + quote + "@" + quote
            + "[^" + quote + "]+" + quote
            + R"()\s+)?)"; // )\s+)?

    pattern += R"((SQL\s+SECURITY\s+(\S+)\s+)?)"; // (SQL\s+SECURITY\s+(\S+)\s+)?

    pattern += R"(VIEW\s+[^\(]+\s+)"; // VIEW\s+[^\(]+\s+

    pattern += R"((\([^\)]+\)\s+)?)"; // (\([^\)]+\)\s+)? -- column list

    pattern += R"(AS\s+(.+)(\s+WITH\s+(\w+\s+)?CHECK\s+OPTION\s*)?$)";

    _regexp.setPattern(pattern);

}

void ViewStructureParser::run(ViewEntity * view)
{

    ViewStructure * structure = view->structure();

    structure->clear();

    QString createSQL = view->createCode();

    QRegularExpressionMatch match = _regexp.match(createSQL);

    if (!match.hasMatch()) {
        meowLogCC(Log::Category::Error, _connection)
            << "Unable to parse view " << view->name();
        return;
    }

    if (_connection->connectionParams()->serverType() == ServerType::MySQL) {
        structure->algorithm = match.captured(3).toUpper();
        if (structure->algorithm.isEmpty()) {
            structure->algorithm = "UNDEFINED";
        }
        structure->definer = _connection->dequoteIdentifier(
                    match.captured(5), '@');
        structure->sqlSecurity = match.captured(7).toUpper();
        if (structure->sqlSecurity.isEmpty()) {
            structure->sqlSecurity = "DEFINER";
        }
        structure->checkOption = match.captured(11).toUpper();
    }

    structure->setSelectStatement(match.captured(9));
}

} // namespace db
} // namespace meow
