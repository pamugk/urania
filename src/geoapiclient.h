#ifndef GEOAPICLIENT_H
#define GEOAPICLIENT_H

#include <QObject>
#include <QQmlEngine>

class GeoApiClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit GeoApiClient(QObject *parent = nullptr);

    Q_INVOKABLE void searchByName(const QString &query);

Q_SIGNALS:
    void searchByNameCompleted(const QVector<QString> &results);
    void searchByNameStarted();
    void searchByNameReset();

private:
    QNetworkAccessManager *httpClient;

};

#endif // GEOAPICLIENT_H
