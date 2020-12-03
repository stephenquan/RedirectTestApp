//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "NetworkAccessManager.h"
#include "NetworkReply.h"
#include <QQmlEngine>
#include <QHttpMultiPart>
#include <QNetworkProxy>
#include <QUrl>
#include <QDebug>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkAccessManager::NetworkAccessManager(QObject* parent) :
    QObject(parent),
    m_NetworkAccessManager(nullptr),
    m_MultiPart(nullptr)
{
    qDebug() << Q_FUNC_INFO << this;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkAccessManager::~NetworkAccessManager()
{
    if (m_NetworkAccessManager)
    {
        clearNetworkAccessManager();
    }

    clearNetworkAccessManager();

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkAccessManager::createNetworkAccessManager()
{
    if (m_NetworkAccessManager)
    {
        return;
    }

    m_NetworkAccessManager = new QNetworkAccessManager();

    connectSignals();

    qDebug() << Q_FUNC_INFO << "Network.AccessManager.m_NetworkAccessManager" << m_NetworkAccessManager;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkAccessManager::clearNetworkAccessManager()
{
    if (!m_NetworkAccessManager)
    {
        return;
    }

    disconnectSignals();

    m_NetworkAccessManager->deleteLater();
    m_NetworkAccessManager = nullptr;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkAccessManager::connectSignals()
{
    connect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this, &NetworkAccessManager::onFinished);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkAccessManager::disconnectSignals()
{
    disconnect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this, &NetworkAccessManager::onFinished);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply* NetworkAccessManager::get(const QUrl& url)
{
    qDebug() << Q_FUNC_INFO << "url: " << url;

    if (!m_NetworkAccessManager)
    {
        createNetworkAccessManager();
    }

    if (!m_NetworkAccessManager)
    {
        return nullptr;
    }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));

    QNetworkReply* reply = m_NetworkAccessManager->get(request);
    qDebug() << Q_FUNC_INFO << reply;
    NetworkReply* networkReply = new NetworkReply(reply);
    QQmlEngine::setObjectOwnership(networkReply, QQmlEngine::JavaScriptOwnership);
    return networkReply;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply* NetworkAccessManager::post(const QUrl& url, const QVariant& params)
{
    qDebug() << Q_FUNC_INFO << "url: " << url;
    qDebug() << Q_FUNC_INFO << "params: " << params;

    if (!m_NetworkAccessManager)
    {
        createNetworkAccessManager();
    }

    if (!m_NetworkAccessManager)
    {
        return nullptr;
    }

    if (m_MultiPart)
    {
        delete m_MultiPart;
        m_MultiPart = nullptr;
    }

    QVariantMap paramsMap = params.toMap();
    bool hasParams = paramsMap.keys().count() > 0;
    bool userHttpMultiPart = false;

    QNetworkReply* reply = nullptr;
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    if (!hasParams)
    {
        reply = m_NetworkAccessManager->post(request, QByteArray());
    }
    else if (userHttpMultiPart)
    {
        m_MultiPart = encodeHttpMultiPart(paramsMap);
        reply = m_NetworkAccessManager->post(request, m_MultiPart);
    }
    else
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
        QByteArray body = encodeByteArray(paramsMap);
        reply = m_NetworkAccessManager->post(request, body);
    }

    NetworkReply* networkReply = new NetworkReply(reply);
    QQmlEngine::setObjectOwnership(networkReply, QQmlEngine::JavaScriptOwnership);
    return networkReply;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkAccessManager::onFinished(QNetworkReply* reply)
{
    qDebug() << Q_FUNC_INFO << reply;
    NetworkReply* networkReply = new NetworkReply(reply);
    QQmlEngine::setObjectOwnership(networkReply, QQmlEngine::JavaScriptOwnership);
    emit finished(networkReply);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QHttpMultiPart* NetworkAccessManager::encodeHttpMultiPart(const QVariantMap& values)
{
    if (!values.keys().length())
    {
        return nullptr;
    }

    QHttpMultiPart* multiPart = new QHttpMultiPart();
    foreach (QString key, values.keys())
    {
        QString value = values[key].toString();
        QHttpPart textPart;
        QString header = "form-data; name=\"" + key + "\"";
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, header);
        textPart.setBody(value.toUtf8());
        multiPart->append(textPart);
    }
    return multiPart;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QByteArray NetworkAccessManager::encodeByteArray(const QVariantMap &values)
{
    if (!values.keys().length())
    {
        return QByteArray();
    }

    QString body;
    foreach (QString key, values.keys())
    {
        QString value = values[key].toString();
        QString keyValue = key + "=" + QUrl::toPercentEncoding(value);
        if (body.isEmpty())
        {
            body = keyValue;
        }
        else
        {
            body = body + "&" + keyValue;
        }
    }
    return body.toUtf8();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
