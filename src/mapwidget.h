#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

#include <osmscoutclientqt/OSMScoutQt.h>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    const osmscout::Magnification maxZoom;
    const osmscout::Magnification minZoom;
    osmscout::MapRenderer* renderer;
    osmscout::MercatorProjection currentProjection;
    QPoint lastMousePosition;
};

#endif // MAPWIDGET_H
