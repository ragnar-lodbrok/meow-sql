#include "geometry_helpers.h"
#include <QApplication>
#include <QScreen>

namespace meow {
namespace ui {
namespace helpers {

static QRect screenRectForRect(const QRect & rect)
{
    auto screens = QApplication::screens();

    for (const auto & screen : screens) {
        auto screenRect = screen->virtualGeometry();
        if (screenRect.intersects(rect)) {
            return screen->availableGeometry();
        }
    }
    return QRect();
}

static void fitRectToRect(QRect & rectSmall, const QRect & rectBig)
{
    if (rectSmall.width() > rectBig.width() ||
        rectSmall.height() > rectBig.height() ) {
        QSize boundedSize = rectSmall.size().boundedTo(rectBig.size());
        rectSmall.setSize(boundedSize);
    }

    if (rectSmall.bottomRight().x() > rectBig.bottomRight().x() ||
        rectSmall.bottomRight().y() > rectBig.bottomRight().y() ) {

        rectSmall.translate(
            rectBig.bottomRight() - rectBig.united(rectSmall).bottomRight()
        );
    } else if (rectSmall.topLeft().x() < rectBig.topLeft().x() ||
               rectSmall.topLeft().y() < rectBig.topLeft().y()) {
        rectSmall.translate(
            rectBig.topLeft() - rectBig.united(rectSmall).topLeft()
        );
    }
}

void fitRectToScreen(QRect & rect)
{
    const QRect screenRect = screenRectForRect(rect);
    if (screenRect.contains(rect) == false) {
        fitRectToRect(rect, screenRect);
    }
}

} // namespace helpers
} // namespace ui
} // namespace meow
