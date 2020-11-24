//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "NetworkReply.h"
#include <QDebug>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply::NetworkReply(QObject* parent) :
    QObject(parent),
    m_Reply(nullptr)
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply::NetworkReply(QNetworkReply* reply, QObject* parent) :
    QObject(parent),
    m_Reply(reply)
{
    if (m_Reply)
    {
        connect(m_Reply, &QNetworkReply::finished, this, &NetworkReply::onFinished);
    }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkReply::~NetworkReply()
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

int NetworkReply::statusCode() const
{
    if (!m_Reply)
    {
        return 0;
    }

    return m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QVariant NetworkReply::headers() const
{
    if (!m_Reply)
    {
        return QVariant();
    }

    QVariantMap result;
    foreach (QNetworkReply::RawHeaderPair pair, m_Reply->rawHeaderPairs())
    {
        QString key = QString::fromUtf8(pair.first);
        QString value = QString::fromUtf8(pair.second);
        result[key] = value;
    }

    return result;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------


QByteArray NetworkReply::readAll()
{
    if (!m_Reply)
    {
        return QByteArray();
    }

    return m_Reply->readAll();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkReply::onFinished()
{
    emit finished();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
