#include "drupalmodel.h"
#include <QtQml>

DrupalModel::DrupalModel(QObject *parent)
    : QAbstractListModel(parent)
{
   //QNetworkAccessManager networkManager;
  // QNetworkRequest request(QUrl("http://104.251.215.93:5050/projects-list/all/all?_format=json"));
    _request.setUrl(QUrl("http://104.251.215.93:5050/projects-list/all/all?_format=json&items_per_page=10"));
  //  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    _networkManager =  new QNetworkAccessManager(this);
  //  QNetworkReply *reply = _networkManager->get(_request);  // GET


   // connect(reply, SIGNAL(finished()), this, SLOT(onResult(QNetworkReply*)));


    connect(_networkManager, SIGNAL(finished(QNetworkReply*)),
              this, SLOT(onResult(QNetworkReply*)));

//    QEventLoop loop;
//    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec();


    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);


}

int DrupalModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return _map.count();
}

QVariant DrupalModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant::Invalid;

    // FIXME: Implement me!
       return _map.value(index.row()).value(_roles.value(role));
}

QHash<int, QByteArray> DrupalModel::roleNames() const
{
   return _roles;
}

void DrupalModel::onResult(QNetworkReply *reply)
{


    if (reply->error() != QNetworkReply::NoError){
        qDebug() << reply->error();
        return;
    }

     QByteArray data =  reply->readAll();
    // qDebug() << "DATA RAW:" + data;

     QJsonDocument obj = QJsonDocument::fromJson(data);

   //  qDebug() << obj.array();

     // create roles from the data
     int roleValue = Qt::UserRole + 1;
     QJsonValue firstObj = QJsonValue(obj.array().at(0));
     for (int i =0; i<firstObj.toObject().keys().count(); i++) {
         QString key = firstObj.toObject().keys().at(i);
         _roles.insert(roleValue++, key.toUtf8());
     }


     beginInsertRows(QModelIndex(),0, obj.array().count());
     for (int i =0; i < obj.array().count(); i++) {
         QJsonValue ele = obj.array().at(i);
         QHash<QByteArray, QVariant> bulk;
         for (int j=0; j< ele.toObject().keys().count(); j++) {
             QString key = ele.toObject().keys().at(j);
             qDebug() << ele.toObject().keys().at(j);
             QJsonValue value = ele.toObject().value(key);
             qDebug() << value.toArray().at(0).toObject();
             bulk.insert(key.toUtf8(), value.toArray().at(0).toObject());
            // qDebug() << key.toUtf8() << value.toString().toUtf8() ;
         }
        _map[i] = bulk;
     }
     endInsertRows();

    emit countChanged();

}

int getRole() {

}


void registerDrupalModelTypes() {
   // qmlRegisterType<QObject>(); // This is not needed. Check Later.
    qmlRegisterType<DrupalModel>("DrupalModel", 0, 1, "DrupalModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerDrupalModelTypes);
