#include "mapwidget.h"

#include <QMouseEvent>
#include <QWheelEvent>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent),
      maxZoom(osmscout::Magnification{osmscout::Magnification::magHouse}),
      minZoom(osmscout::Magnification{osmscout::Magnification::magContinent})
{
    setFocusPolicy(Qt::StrongFocus);

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
    osmscout::OSMScoutQt::FreeInstance();
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
    bool updatedModel = false;
    switch (event->key())
    {
        // Navigation
        case Qt::Key::Key_Left:
        {
            updatedModel = currentProjection.Move(-width() / 100., 0);
            break;
        }
        case Qt::Key::Key_Up:
        {
            updatedModel = currentProjection.Move(0, height() / 100.);
            break;
        }
        case Qt::Key::Key_Right:
        {
            updatedModel = currentProjection.Move(width() / 100., 0);
            break;
        }
        case Qt::Key::Key_Down:
        {
            updatedModel = currentProjection.Move(0, -height() / 100.);
            break;
        }
        // Zoom
        case Qt::Key::Key_Minus:
        {
            if (currentProjection.GetMagnification() > minZoom)
            {
                updatedModel = currentProjection.Set(
                    currentProjection.GetCenter(),
                    osmscout::Magnification{osmscout::MagnificationLevel{currentProjection.GetMagnification().GetLevel() - 1}},
                    static_cast<size_t>(width()), static_cast<size_t>(height())
                );
            }
            break;
        }
        case Qt::Key::Key_Plus:
        {
            if (currentProjection.GetMagnification() < maxZoom)
            {
                updatedModel = currentProjection.Set(
                    currentProjection.GetCenter(),
                    osmscout::Magnification{osmscout::MagnificationLevel{currentProjection.GetMagnification().GetLevel() + 1}},
                    static_cast<size_t>(width()), static_cast<size_t>(height())
                );
            }
            break;
        }
        default:
        {
            QWidget::keyPressEvent(event);
        }
    }

    if (updatedModel)
    {
        update();
    }
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->position().toPoint();
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    auto eventPoint = event->position().toPoint();
    auto x_delta = eventPoint.x() - lastMousePosition.x();
    auto y_delta = eventPoint.y() - lastMousePosition.y();
    if (currentProjection.Move(-x_delta, y_delta))
    {
        update();
    }
    else
    {
        qDebug() << "Something went wrong during projection move";
    }
    lastMousePosition = eventPoint;
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

void MapWidget::wheelEvent(QWheelEvent *event)
{
    uint32_t magnificationDelta;
    bool updatedModel = false;
    auto eventPoint = event->position().toPoint();
    if (event->angleDelta().y() > 0)
    {
        if (currentProjection.GetMagnification() >= maxZoom)
        {
            return;
        }
        magnificationDelta = 1;
        auto x_delta = (width() / 2. - eventPoint.x()) * 0.75;
        auto y_delta = (height() / 2. - eventPoint.y()) * 0.75;
        if (currentProjection.Move(-x_delta, y_delta))
        {
            updatedModel = true;
        }
        else
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
        auto x_delta = (width() / 2. - eventPoint.x()) * 0.75;
        auto y_delta = (height() / 2. - eventPoint.y()) * 0.75;
        if (currentProjection.Move(x_delta, -y_delta))
        {
            updatedModel = true;
        }
        else
        {
            qDebug() << "Something went wrong during projection move";
        }
    }

    if (currentProjection.Set(
        currentProjection.GetCenter(),
        osmscout::Magnification{osmscout::MagnificationLevel{currentProjection.GetMagnification().GetLevel() + magnificationDelta}},
        static_cast<size_t>(width()), static_cast<size_t>(height())
    ))
    {
        updatedModel = true;
    }
    else
    {
        qDebug() << "Something went wrong on magnification level update";
    }

    if (updatedModel)
    {
        update();
    }
}
