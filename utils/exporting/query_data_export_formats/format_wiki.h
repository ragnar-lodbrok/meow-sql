#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatWiki : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "wiki";
    }

    virtual QString name() const override {
        return QObject::tr("Wiki markup");
    }

    virtual QString fileExtension() const override {
        return "wiki";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

