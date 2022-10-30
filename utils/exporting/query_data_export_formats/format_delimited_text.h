#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

// TODO: rm
class QueryDataExportFormatDelimitedText : public IQueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "delimited_text";
    }

    virtual QString name() const override {
        return QObject::tr("Delimited text");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H
