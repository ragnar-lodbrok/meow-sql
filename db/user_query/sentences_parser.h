#ifndef DB_USER_QUERY_SENTENCES_PARSER_H
#define DB_USER_QUERY_SENTENCES_PARSER_H

#include <QStringList>

namespace meow {
namespace db {
namespace user_query {

class SentencesParser
{
public:    
    QStringList parseByDelimiter(
        const QString & SQL,
        const QString & delim = QString(";")) const;
};

} // namespace user_query
} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_SENTENCES_PARSER_H
