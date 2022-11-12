#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatLatex : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "latex";
    }

    virtual QString name() const override {
        return QObject::tr("LaTeX");
    }

    virtual QString fileExtension() const override {
        return "LaTeX";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

