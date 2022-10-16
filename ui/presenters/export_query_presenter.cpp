#include "export_query_presenter.h"
#include "utils/exporting/query_data_exporter.h"

namespace meow {
namespace ui {
namespace presenters {

ExportQueryPresenter::ExportQueryPresenter()
    : _exporter(new utils::exporting::QueryDataExporter())
{

}

void ExportQueryPresenter::setModeClipboard()
{
    _exporter->setMode(utils::exporting::QueryDataExporter::Mode::Clipboard);
}

bool ExportQueryPresenter::isModeClipboard() const
{
    return _exporter->mode()
            == utils::exporting::QueryDataExporter::Mode::Clipboard;
}

void ExportQueryPresenter::setModeFile()
{
    _exporter->setMode(utils::exporting::QueryDataExporter::Mode::File);
}

bool ExportQueryPresenter::isModeFile() const
{
    return _exporter->mode() == utils::exporting::QueryDataExporter::Mode::File;
}

QString ExportQueryPresenter::fileEncoding() const
{
    return _exporter->fileEncoding();
}

QStringList ExportQueryPresenter::supportedFileEncodings() const
{
    QStringList encodings = _exporter->supportedFileEncodings();
    encodings.sort();
    return encodings;
}

void ExportQueryPresenter::setFileEncoding(const QString & encoding)
{
    _exporter->setFileEncoding(encoding);
}

QStringList ExportQueryPresenter::formatNames() const
{
    return _exporter->formatNames();
}

} // namespace presenter
} // namespace ui
} // namespace meow
