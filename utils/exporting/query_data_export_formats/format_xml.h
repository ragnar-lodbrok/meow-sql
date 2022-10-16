#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatXML : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "XML";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

