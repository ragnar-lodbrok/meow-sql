#include "format_factory.h"

#include "format_csv.h"
#include "format_delimited_text.h"
#include "format_html_table.h"
#include "format_xml.h"
#include "format_sql_inserts.h"
#include "format_sql_replaces.h"
#include "format_sql_deletes_replaces.h"
#include "format_latex.h"
#include "format_wiki.h"
#include "format_php_array.h"
#include "format_markdown.h"
#include "format_json.h"

namespace meow {
namespace utils {
namespace exporting {

std::vector<QueryDataExportFormatPtr>
QueryDataExportFormatFactory::createFormats() const
{
    std::vector<QueryDataExportFormatPtr> formats;

    formats.push_back(std::make_shared<QueryDataExportFormatCSV>());
    formats.push_back(std::make_shared<QueryDataExportFormatDelimitedText>());
    formats.push_back(std::make_shared<QueryDataExportFormatHTMLTable>());
    formats.push_back(std::make_shared<QueryDataExportFormatXML>());
    formats.push_back(std::make_shared<QueryDataExportFormatSQLInserts>());
    formats.push_back(std::make_shared<QueryDataExportFormatSQLReplaces>());
    formats.push_back(std::make_shared<QueryDataExportFormatSQLDeletesReplaces>());
    formats.push_back(std::make_shared<QueryDataExportFormatLatex>());
    formats.push_back(std::make_shared<QueryDataExportFormatWiki>());
    formats.push_back(std::make_shared<QueryDataExportFormatPHPArray>());
    formats.push_back(std::make_shared<QueryDataExportFormatMarkdown>());
    formats.push_back(std::make_shared<QueryDataExportFormatJSON>());

    return formats;
}



} // namespace exporting
} // namespace utils
} // namespace meow
