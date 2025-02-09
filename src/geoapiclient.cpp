#include "geoapiclient.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

GeoApiClient::GeoApiClient(QObject *parent)
    : QObject { parent },
      httpClient(new QNetworkAccessManager(this))
{}

void GeoApiClient::searchByName(const QString &query)
{
    if (query.isEmpty())
    {
        Q_EMIT searchByNameReset();
        return;
    }

    QUrl requestUrl(QStringLiteral("https://nominatim.openstreetmap.org/search"));
    requestUrl.setQuery(QUrlQuery({
                                      std::pair(QStringLiteral("q"), query),
                                      std::pair(QStringLiteral("format"), QStringLiteral("jsonv2")),
                                      std::pair(QStringLiteral("limit"), QStringLiteral("10")),
                                      std::pair(QStringLiteral("accept-language"), QStringLiteral("ru-RU")),
                                      std::pair(QStringLiteral("dedupe"), QStringLiteral("1"))
                                  }));
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QStringLiteral("Urania/0.1"));

    Q_EMIT searchByNameStarted();

    QNetworkReply *reply = httpClient->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QVector<QString> results;
            QJsonArray resultJsonArray = QJsonDocument::fromJson(reply->readAll()).array();
            results.resize(resultJsonArray.count());
            for (const QJsonValue &resultItem : std::as_const(resultJsonArray))
            {
                results << resultItem[QStringLiteral("display_name")].toString();
            }
            Q_EMIT searchByNameCompleted(results);
        }
        else if (reply->error() != QNetworkReply::OperationCanceledError)
        {
            qDebug() << reply->error()
                     << reply->errorString()
                     << reply->readAll();
        }

        reply->deleteLater();
    });
    connect(this, &QObject::destroyed, reply, &QNetworkReply::abort);
    connect(this, &GeoApiClient::searchByNameReset, reply, &QNetworkReply::abort);
    connect(this, &GeoApiClient::searchByNameStarted, reply, &QNetworkReply::abort);
}
