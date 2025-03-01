#include "mapwidget.h"

#include <QMouseEvent>
#include <QWheelEvent>

const auto MAX_ZOOM = 20;
const auto MIN_ZOOM = 0;
const auto MAP_DPI = 96;

// TODO: add new features to this code
// from QtWidgetsDemoApp libosmscout example example.

namespace
{
// used with QWheelEvent
template <typename EventType>
auto pos(EventType *event)
{
    return event->position().toPoint();
}

// used with QMouseEvent
template <typename EventType>
auto pos2(EventType *event)
{
    return event->position().toPoint();
}
}

MapWidget::MapWidget(QWidget *parent)
    : QWidget{parent}
{
    currentProjection.Set(
                {0, 0}, 0.0,
                osmscout::Magnification{osmscout::Magnification::magWorld},
                MAP_DPI,
                width(), height()
    );
    renderer = osmscout::OSMScoutQt::GetInstance().MakeMapRenderer(osmscout::RenderingType::TiledRendering);
    connect(renderer, &osmscout::MapRenderer::Redraw, this, [this] { update(); });
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    renderer->RenderMap(
                painter,
                osmscout::MapViewStruct
                {
                    currentProjection.GetCenter(),
                    osmscout::Bearing{},
                    currentProjection.GetMagnification(),
                    static_cast<size_t>(width()),
                    static_cast<size_t>(height()),
                    MAP_DPI,
                }
    );
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = ::pos2(event);
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    auto x_delta = ::pos2(event).x() - lastMousePosition.x();
    auto y_delta = ::pos2(event).y() - lastMousePosition.y();
    currentProjection.Move(-x_delta, y_delta);
    lastMousePosition = ::pos2(event);
    update();
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    auto magnification = currentProjection.GetMagnification().GetLevel();
    if (event->angleDelta().y() > 0)
    {
        if (magnification >= MAX_ZOOM)
        {
            return;
        }
        magnification++;
        auto x_delta = (width() / 2. - ::pos(event).x()) * 0.75;
        auto y_delta = (height() / 2. - ::pos(event).y()) * 0.75;
        currentProjection.Move(-x_delta, y_delta);
    }
    else
    {
        if (magnification <= MIN_ZOOM)
        {
            return;
        }
        magnification--;
        auto x_delta = (width() / 2. - ::pos(event).x()) * 0.75;
        auto y_delta = (height() / 2. - ::pos(event).y()) * 0.75;
        currentProjection.Move(x_delta, -y_delta);
    }
    currentProjection.Set(
                currentProjection.GetCenter(),
                osmscout::Magnification{osmscout::MagnificationLevel{magnification}},
                width(), height()
    );
    update();
}
