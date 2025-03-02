#include "mapwidget.h"

#include <QMouseEvent>
#include <QWheelEvent>

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
    : QWidget(parent),
      maxZoom(osmscout::Magnification{osmscout::Magnification::magHouse}),
      minZoom(osmscout::Magnification{osmscout::Magnification::magContinent})
{
    if (!currentProjection.Set(
        {0, 0}, 0.0,
        osmscout::Magnification{osmscout::Magnification::magContinent},
        logicalDpiX(),
        static_cast<size_t>(400), static_cast<size_t>(400)
    ))
    {
        qDebug() << "Something went wrong on projection setup";
    }
    renderer = osmscout::OSMScoutQt::GetInstance().MakeMapRenderer(osmscout::RenderingType::PlaneRendering);
    connect(renderer, &osmscout::MapRenderer::Redraw, this, [this] { update(); });
}

MapWidget::~MapWidget()
{

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
                    static_cast<double>(logicalDpiX()),
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
    if (!currentProjection.Move(-x_delta, y_delta))
    {
        qDebug() << "Something went wrong during projection move";
    }
    lastMousePosition = ::pos2(event);
    update();
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    uint32_t magnificationDelta;
    if (event->angleDelta().y() > 0)
    {
        if (currentProjection.GetMagnification() >= maxZoom)
        {
            return;
        }
        magnificationDelta = 1;
        auto x_delta = (width() / 2. - ::pos(event).x()) * 0.75;
        auto y_delta = (height() / 2. - ::pos(event).y()) * 0.75;
        if (!currentProjection.Move(-x_delta, y_delta))
        {
            qDebug() << "Something went wrong during projection move";
        }
    }
    else
    {
        if (currentProjection.GetMagnification() <= minZoom)
        {
            return;
        }
        magnificationDelta = -1;
        auto x_delta = (width() / 2. - ::pos(event).x()) * 0.75;
        auto y_delta = (height() / 2. - ::pos(event).y()) * 0.75;
        if (!currentProjection.Move(x_delta, -y_delta))
        {
            qDebug() << "Something went wrong during projection move";
        }
    }

    if (!currentProjection.Set(
        currentProjection.GetCenter(),
        osmscout::Magnification{osmscout::MagnificationLevel{currentProjection.GetMagnification().GetLevel() + magnificationDelta}},
        static_cast<size_t>(width()), static_cast<size_t>(height())
    ))
    {
        qDebug() << "Something went wrong on magnification level update";
    }
    update();
}
