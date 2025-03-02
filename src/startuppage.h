#ifndef STARTUPPAGE_H
#define STARTUPPAGE_H

#include <QWidget>

namespace Ui {
class StartupPage;
}

class StartupPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartupPage(QWidget *parent = nullptr);
    ~StartupPage();

signals:
    void mapConfigured(const QString &mapsDirectory,
                       const QString &styleSheetFile);

private:
    QString selectedMapsFolderPath;
    QString selectedStyleSheetFilePath;
    Ui::StartupPage *ui;
};

#endif // STARTUPPAGE_H
