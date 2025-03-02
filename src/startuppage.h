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
    void mapConfigured(const QString &mapDirectory,
                       const QString &styleSheetFile);

private:
    QString selectedMapFolderPath;
    QString selectedStyleSheetFilePath;
    Ui::StartupPage *ui;
};

#endif // STARTUPPAGE_H
