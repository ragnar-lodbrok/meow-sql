#ifndef MEOW_SETTINGS_QUERY_DATA_EXPORT_STORAGE_H
#define MEOW_SETTINGS_QUERY_DATA_EXPORT_STORAGE_H

namespace meow {

namespace utils {
namespace exporting {
    class QueryDataExporter;
}
}

namespace settings {

class QueryDataExportStorage
{
public:
    void saveFrom(utils::exporting::QueryDataExporter * exporter);
    void loadTo(utils::exporting::QueryDataExporter * exporter);
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_QUERY_DATA_EXPORT_STORAGE_H
