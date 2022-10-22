#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_INTERFACE_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_INTERFACE_H

#include <QString>
#include <QObject>
#include <memory>

namespace meow {
namespace utils {
namespace exporting {

class IQueryDataExportFormat
{
public:
    virtual ~IQueryDataExportFormat() {}

    virtual QString id() const = 0;
    virtual QString name() const = 0;
};

using QueryDataExportFormatPtr = std::shared_ptr<IQueryDataExportFormat>;


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_INTERFACE_H
