#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatPHPArray : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "php_array";
    }

    virtual QString name() const override {
        return QObject::tr("PHP Array");
    }

    virtual QString fileExtension() const override {
        return "php";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

