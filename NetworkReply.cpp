//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "NetworkReply.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply::NetworkReply(QObject* parent) :
    QObject(parent),
    m_Reply(nullptr),
    m_HttpMethod(NetworkAccessManager::HttpMethodGET),
    m_FollowRedirects(NetworkAccessManager::FollowRedirectsDisabled),
    m_Error(0),
    m_StatusCode(0)
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply::~NetworkReply()
{
    if (m_Reply)
    {
        setNetworkReply(nullptr);
    }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::update()
{
    qDebug() << Q_FUNC_INFO;

    m_Error = 0;
    m_ErrorString.clear();
    m_StatusCode = 0;
    m_StatusText.clear();
    m_Headers.clear();

    if (!m_Reply)
    {
        return;
    }

    m_Error = m_Reply->error();
    m_ErrorString = m_Reply->errorString();
    m_StatusCode = m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_StatusText.clear();

    qDebug() << Q_FUNC_INFO << "statusCode: " << m_StatusCode;

    QVariantMap headersMap;
    foreach (QNetworkReply::RawHeaderPair pair, m_Reply->rawHeaderPairs())
    {
        QString key = QString::fromUtf8(pair.first);
        QString value = QString::fromUtf8(pair.second);
        headersMap[key] = value;
    }
    m_Headers = headersMap;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::onFinished()
{
    m_Response.clear();

    if (m_Reply)
    {
        m_Response = m_Reply->readAll();
        update();

        bool isRedirect = (m_StatusCode == 301
                           || m_StatusCode == 302
                           || m_StatusCode == 303
                           || m_StatusCode == 305
                           || m_StatusCode == 307
                           || m_StatusCode == 308);

        if (isRedirect)
        {
            QUrl targetUrl = m_Reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            QNetworkRequest targetRequest(targetUrl);

            QNetworkAccessManager* manager = m_Reply->manager();
            QNetworkReply* _networkReply;

            switch (m_HttpMethod)
            {
            case NetworkAccessManager::HttpMethodGET:
                m_Reply->deleteLater();
                _networkReply = manager->get(targetRequest);
                setNetworkReply(_networkReply);
                return;

            case NetworkAccessManager::HttpMethodPOST:
                _networkReply = manager->post(targetRequest, m_BodyByteArray);
                setNetworkReply(_networkReply);
                return;
            }
        }

        m_Reply->deleteLater();
        setNetworkReply(nullptr);
    }

    emit finished();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::setHttpMethod(NetworkAccessManager::HttpMethod httpMethod)
{
    m_HttpMethod = httpMethod;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::setFollowRedirects(NetworkAccessManager::FollowRedirects followRedirects)
{
    m_FollowRedirects = followRedirects;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::setNetworkReply(QNetworkReply* reply)
{
    if (m_Reply == reply)
    {
        return;
    }

    if (m_Reply)
    {
        m_Reply->deleteLater();
        m_Reply = nullptr;
    }

    if (!reply)
    {
        return;
    }

    m_Reply = reply;
    connect(m_Reply, &QNetworkReply::finished, this, &NetworkReply::onFinished);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::setBody(const QByteArray& body)
{
    m_BodyByteArray = body;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
