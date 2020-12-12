#include "routine_structure_parser.h"
#include "db/entity/routine_entity.h"
#include "db/routine_structure.h"
#include "helpers/logger.h"
#include "db/connection.h"
#include <QRegularExpression>
#include <QDebug>

namespace meow {
namespace db {

namespace  {

bool isMySQL(db::Connection * connection)
{
    return connection->connectionParams()->serverType()
            == db::ServerType::MySQL;
}

}

RoutineStructureParser::RoutineStructureParser(Connection * connection)
    :_connection(connection)
{

}

void RoutineStructureParser::run(RoutineEntity * routine)
{
    RoutineStructure * structure = routine->structure();

    structure->clear();

    QString createSQL = routine->createCode();

    // CREATE DEFINER=`user`@`host` FUNCTION `name`

    QRegularExpression definerRegexp;

    QString definerPattern
        = R"(^CREATE\s+(DEFINER\s*=\s*(\S+))?\s*(FUNCTION|PROCEDURE))";

    definerRegexp.setPattern(definerPattern); // TODO: remember pattern in obj

    QRegularExpressionMatch match = definerRegexp.match(createSQL);

    if (!match.hasMatch()) {
        meowLogCC(Log::Category::Error, _connection)
            << "Unable to parse routine " << routine->name();
        return;
    }

    structure->definer = _connection->dequoteIdentifier(match.captured(2), '@');

    QString quoteChars = QString(_connection->getIdentQuote());

    int pos = match.capturedEnd(3);
    int parenthesesCount = 0;
    QString paramsSQL;
    bool inLiteral = false;

    for (; pos<createSQL.length(); ++pos) {
        if (createSQL[pos] == ')' && !inLiteral) {
            --parenthesesCount;
            if (parenthesesCount <= 0) break;
        }
        if (quoteChars.contains(createSQL[pos])) {
            inLiteral = !inLiteral;
        }
        if (parenthesesCount >= 1) {
            paramsSQL += createSQL[pos];
        }
        if (createSQL[pos] == '(' && !inLiteral) {
            ++parenthesesCount;
        }
    }

    QString paramsPattern =
        R"((^|,)\s*((IN|OUT|INOUT)\s+)?(\S+)\s+([^\s,\(]+(\([^\)]*\))?[^,]*))";

    QRegularExpression paramsRegexp;
    paramsRegexp.setPattern(paramsPattern);

    if (!paramsSQL.isEmpty()) {
        auto it = paramsRegexp.globalMatch(paramsSQL);
        while (it.hasNext()) {
             QRegularExpressionMatch match = it.next();

             RoutuneStructureParam param;
             param.context = match.captured(3).trimmed();
             if (param.context.isEmpty()) {
                 param.context = "IN";
             }
             param.name = _connection->dequoteIdentifier(
                            match.captured(4).trimmed());
             param.dataType = match.captured(5).trimmed();

             structure->params << param;
         }
    }

    QString body = createSQL.mid(pos+1);

    QString characteristicsPattern = R"(^\s*()" \
    R"(RETURNS\s+(\S+(\s+UNSIGNED)?(\s+CHARSET\s+\S+)?(\s+COLLATE\s\S+)?)|)" \
    R"(LANGUAGE\s+SQL|)" \
    R"((NOT\s+)?DETERMINISTIC|)" \
    R"(CONTAINS\s+SQL|)" \
    R"(NO\s+SQL|)" \
    R"(READS\s+SQL\s+DATA|)" \
    R"(MODIFIES\s+SQL\s+DATA|)" \
    R"(SQL\s+SECURITY\s+(DEFINER|INVOKER))" \
    R"()\s)"; // )\s
    QRegularExpression characteristicsRegexp;
    characteristicsRegexp.setPattern(characteristicsPattern);
    characteristicsRegexp.setPatternOptions(QRegularExpression::MultilineOption);

    int characteristicsLength = 0;
    auto it = characteristicsRegexp.globalMatch(body);
    while (it.hasNext()) {
         QRegularExpressionMatch match = it.next();
         QString fullOption = match.captured(1).toUpper();
         if (fullOption.startsWith("RETURNS")) {
             structure->returnType = match.captured(2);
         } else if (fullOption.startsWith("DETERMINISTIC")) {
             structure->deterministic = true;
         } else if (fullOption.startsWith("NOT DETERMINISTIC")) {
             structure->deterministic = false;
         } else if (fullOption.startsWith("SQL SECURITY")) {
             structure->sqlSecurity = match.captured(7);
         } else if (fullOption.startsWith("CONTAINS SQL")
                    || fullOption.startsWith("NO SQL")
                    || fullOption.startsWith("READS SQL DATA")
                    || fullOption.startsWith("MODIFIES SQL DATA")) {
             structure->dataAccess = fullOption.toUpper();
         }
         characteristicsLength += match.capturedLength(0);
    }

    if (structure->sqlSecurity.isEmpty() && isMySQL(_connection)) {
        structure->sqlSecurity = "DEFINER";
    }

    body = body.mid(characteristicsLength);
    structure->comment = extractLiteral(body, "COMMENT");
    body = body.trimmed();

    structure->body = body;
}

QString RoutineStructureParser::extractLiteral(QString & SQL,
                                               const QString & prefix)
{
    QString result;
    QRegularExpression regexp;
    QString pattern;
    if (prefix.isEmpty()) {
        pattern = R"(^\s*')";
    } else {
        pattern = R"(^\s*)" + prefix + R"(\s+')";
    }

    regexp.setPattern(pattern);

    QRegularExpressionMatch match = regexp.match(SQL);

    if (match.hasMatch()) {
        int literalStart = match.capturedLength(0);
        bool inLiteral = true;
        int i = literalStart;
        for (; i < SQL.length(); ++i) {
            if (SQL[i] == QChar('\'')) {
                inLiteral = !inLiteral;
            } else if (!inLiteral) {
                break;
            }
        }
        result = SQL.mid(literalStart, i-literalStart-1);
        result = _connection->unescapeString(result);
        SQL = SQL.mid(i);
    }

    return result;
}

} // namespace db
} // namespace meow
