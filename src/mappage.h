#ifndef MAPPAGE_H
#define MAPPAGE_H

#include <QWidget>

namespace Ui {
class MapPage;
}

class MapPage : public QWidget
{
    Q_OBJECT

public:
    explicit MapPage(QWidget *parent = nullptr);
    ~MapPage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget *map;
    Ui::MapPage *ui;
};

#endif // MAPPAGE_H
