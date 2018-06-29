#ifndef DRUPALMODEL_H
#define DRUPALMODEL_H

#include <QAbstractListModel>
#include <QtNetwork>
#include <QJsonDocument>
#include <leveldb/db.h>
#include <leveldb/options.h>



class DrupalModel : public QAbstractListModel
{
    Q_OBJECT

    QNetworkAccessManager *_networkManager;

    QNetworkRequest _request;

    QNetworkReply *_reply;

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit DrupalModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;




private slots:

   void onResult(QNetworkReply* reply);

private:





   QHash<int, QHash<QByteArray, QVariant>> _map;

   QHash<int, QByteArray> _roles;

signals:

   void countChanged();
};

#endif // DRUPALMODEL_H
