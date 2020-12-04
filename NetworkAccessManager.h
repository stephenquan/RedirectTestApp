#ifndef __AppFrameworkNetworkAccessManager__
#define __AppFrameworkNetworkAccessManager__

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>

class NetworkReply;

class NetworkAccessManager : public QObject
{
    Q_OBJECT

public:
    enum FollowRedirects
    {
        FollowRedirectsDisabled,
        FollowRedirectsQt,
        FollowRedirectsSoftware
    };
    Q_ENUM(FollowRedirects)

    enum PostBodyMode
    {
        PostBodyMultiPart,
        PostBodyByteArray
    };
    Q_ENUM(PostBodyMode)

    enum HttpMethod
    {
        HttpMethodGET,
        HttpMethodPOST
    };
    Q_ENUM(HttpMethod)

public:
    NetworkAccessManager(QObject* parent = nullptr);
    ~NetworkAccessManager();

    Q_INVOKABLE NetworkReply* get(const QUrl& url);
    Q_INVOKABLE NetworkReply* post(const QUrl& url, const QVariant& body = QVariant());

signals:
    void finished(NetworkReply* reply);

protected:
    QNetworkAccessManager* m_NetworkAccessManager;
    QHttpMultiPart* m_MultiPart;
    FollowRedirects m_FollowRedirects;
    PostBodyMode m_PostBodyMode;

    void connectSignals();
    void disconnectSignals();
    void createNetworkAccessManager();
    void clearNetworkAccessManager();

protected:
    QHttpMultiPart* encodeHttpMultiPart(const QVariantMap& values);
    QByteArray encodeByteArray(const QVariantMap& values);

protected slots:
    void onFinished(QNetworkReply* reply);

};

#endif
