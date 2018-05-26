#include "foreign_key.h"
#include <QMap>

namespace meow {
namespace db {


ForeignKey::ForeignKey()
    :_id(0),
     _onUpdate(ReferenceOption::Restrict),
     _onDelete(ReferenceOption::Restrict)
{
    // For an ON DELETE or ON UPDATE that is not specified,
    // the default action is always RESTRICT.
}

ForeignKey::operator QString() const
{
    QString str =
    QString("CONSTRAINT `%1` FOREIGN KEY (%2) REFERENCES `%3` (%4)")
            .arg(_name)
            .arg(_columns.join(','))
            .arg(_referenceTable)
            .arg(_referenceColumns.join(','));

    if (_onDelete != ReferenceOption::Restrict) {
        str += " ON DELETE " + referenceOptionToStr(_onDelete);
    }
    if (_onUpdate != ReferenceOption::Restrict) {
        str += " ON UPDATE " + referenceOptionToStr(_onUpdate);
    }

    return str;
}

void ForeignKey::setOnUpdate(const QString & opt) {
    _onUpdate = strToReferenceOption(opt);
}

void ForeignKey::setOnDelete(const QString & opt) {
    _onDelete = strToReferenceOption(opt);
}

ForeignKey::ReferenceOption strToReferenceOption(
    const QString & str)
{

    static QMap<QString, ForeignKey::ReferenceOption> map = {
        { "NO ACTION",   ForeignKey::ReferenceOption::NoAction },
        { "RESTRICT",    ForeignKey::ReferenceOption::Restrict },
        { "CASCADE",     ForeignKey::ReferenceOption::Cascade },
        { "SET NULL",    ForeignKey::ReferenceOption::SetNull },
        { "SET DEFAULT", ForeignKey::ReferenceOption::SetDefault }
    };

    QString strUp = str.toUpper();
    return map.value(strUp, ForeignKey::ReferenceOption::Restrict);
}

QString referenceOptionToStr(ForeignKey::ReferenceOption opt)
{
    switch (opt) {
    case ForeignKey::ReferenceOption::Restrict:
        return "RESTRICT";
    case ForeignKey::ReferenceOption::Cascade:
        return "CASCADE";
    case ForeignKey::ReferenceOption::SetNull:
        return "SET NULL";
    case ForeignKey::ReferenceOption::SetDefault:
        return "SET DEFAULT";
    default:
        return "NO ACTION";
    }
}

} // namespace db
} // namespace meow
