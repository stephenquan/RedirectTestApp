#ifndef __NetworkReply__
#define __NetworkReply__

#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QByteArray>
#include "NetworkAccessManager.h"

class NetworkReply : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int error READ error NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished)
    Q_PROPERTY(int statusCode READ statusCode NOTIFY finished)
    Q_PROPERTY(QVariant headers READ headers NOTIFY finished)
    Q_PROPERTY(QByteArray response READ response NOTIFY finished)

public:
    NetworkReply(QObject* parent = nullptr);
    ~NetworkReply();

public:
    NetworkAccessManager::HttpMethod httpMethod() const { return m_HttpMethod; }
    void setHttpMethod(NetworkAccessManager::HttpMethod httpMethod);
    NetworkAccessManager::FollowRedirects followRedirects() const { return m_FollowRedirects; }
    void setFollowRedirects(NetworkAccessManager::FollowRedirects followRedirects);
    QNetworkReply* networkReply() const { return m_Reply; }
    void setNetworkReply(QNetworkReply* reply);
    void setBody(const QByteArray& body);
    QByteArray response() const { return m_Response; }

signals:
    void finished();

protected slots:
    void onFinished();

protected:
    QNetworkReply* m_Reply;
    NetworkAccessManager::HttpMethod m_HttpMethod;
    NetworkAccessManager::FollowRedirects m_FollowRedirects;
    QByteArray m_BodyByteArray;
    QByteArray m_Response;
    int m_Error;
    QString m_ErrorString;
    int m_StatusCode;
    QString m_StatusText;
    QVariant m_Headers;

    int error() const { return m_Error; }
    QString errorString() const { return m_ErrorString; }
    int statusCode() const { return m_StatusCode; }
    QVariant headers() const { return m_Headers; }
    void update();

};

#endif
