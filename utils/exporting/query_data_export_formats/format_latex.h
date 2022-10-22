#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatLatex : public IQueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "latex";
    }

    virtual QString name() const override {
        return QObject::tr("LaTeX");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

