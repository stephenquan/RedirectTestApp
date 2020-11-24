#ifndef __NetworkReply__
#define __NetworkReply__

#include <QObject>
#include <QUrl>
#include <QNetworkReply>

class NetworkReply : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int error READ error NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished)
    Q_PROPERTY(int statusCode READ statusCode NOTIFY finished)
    Q_PROPERTY(QVariant headers READ headers NOTIFY finished)

public:
    NetworkReply(QObject* parent = nullptr);
    NetworkReply(QNetworkReply* reply, QObject* parent = nullptr);
    ~NetworkReply();

    Q_INVOKABLE QByteArray readAll();

signals:
    void finished();

protected slots:
    void onFinished();

protected:
    QNetworkReply* m_Reply;

    int error() const { return m_Reply ? m_Reply->error() : 0; }
    QString errorString() const { return m_Reply ? m_Reply->errorString() : ""; }
    int statusCode() const;
    QString statusText() const;
    QVariant headers() const;

};

#endif
