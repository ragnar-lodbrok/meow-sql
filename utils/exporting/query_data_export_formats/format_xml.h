#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatXML : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "xml";
    }

    virtual QString name() const override {
        return QObject::tr("XML");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

