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

signals:
    void centerChanged(const osmscout::GeoCoord &newCenter);

public slots:
    void zoomIn();
    void zoomOut();

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

    bool zoomInImplementation();
    bool zoomOutImplementation();
};

#endif // MAPWIDGET_H
