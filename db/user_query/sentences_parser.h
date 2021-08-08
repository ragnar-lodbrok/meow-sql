#ifndef DB_USER_QUERY_SENTENCES_PARSER_H
#define DB_USER_QUERY_SENTENCES_PARSER_H

#include <QStringList>
#include <memory>

namespace meow {
namespace db {
namespace user_query {

enum class SentenceTokenType {
    None = 0,
    Text, // any not matching below
    SingleLineComment, // # or --
    MultipleLineComment, // /* comment */
    QuotedString, // 'str'
    DoubleQuotedString, // "str"
    QuotedIdentifier // `user_id`
};

struct SentenceToken {
    int startIndex = -1;
    int len = 0;
    SentenceTokenType type = SentenceTokenType::Text;
    bool leftOpen = false; // starts in prev sentense, e.g. /*
    bool rightOpen = false; // ends in next sentence, e.g closing */

    SentenceToken(int _startIndex = -1,
                  SentenceTokenType _type = SentenceTokenType::Text)
        :startIndex(_startIndex),
         type(_type) { }

    void setEndIndex(int _endIndex) {
        len = _endIndex - startIndex;
    }
};

using SentenceTokenPtr = std::shared_ptr<SentenceToken>;

struct Sentence
{
    QString text;
    int position = 0;
};

class SentencesParser
{
public:    
    QList<Sentence> parseByDelimiter(
        const QString & SQL,
        const QString & delim = QString(";")) const;

    QList<SentenceTokenPtr> parseToTokens(
        const QString & sentence,
        SentenceTokenType leftOpenType = SentenceTokenType::None) const;
};

} // namespace user_query
} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_SENTENCES_PARSER_H
