#include "drupalmodel.h"
#include <QtQml>


DrupalModel::DrupalModel(QObject *parent)
    : QAbstractListModel(parent)
{

    _request.setUrl(QUrl("http://104.251.215.93:5050/projects-list/all/all?_format=json&items_per_page=10"));

    _networkManager =  new QNetworkAccessManager(this);
    QNetworkReply *reply = _networkManager->get(_request);  // GET

    connect(_networkManager, SIGNAL(finished(QNetworkReply*)),
              this, SLOT(onResult(QNetworkReply*)));



    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &_db);


//    _db->Put(leveldb::WriteOptions(), "key2", "SAmple inserted Value");

//    std::string value;
//    _db->Get(leveldb::ReadOptions(), "key2", &value);


    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
      qDebug() << QString::fromStdString(it->key().ToString()) +  ": "  +  QString::fromStdString(it->value().ToString());
    }


    delete it;

   // qDebug() << value;

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
         //   qDebug() << ele.toObject().keys().at(j);
             QJsonValue value = ele.toObject().value(key);
         //    qDebug() << value.toArray().at(0).toObject();

//             leveldb::Slice key_string((char*)&key, sizeof(QString));
//             leveldb::Slice value_string((char*)&value, sizeof(QJsonValue));

             //leveldb::Slice value_string((char*)&value.toArray().at(0).toObject(), sizeof(QJsonObject));
           //  value.toArray().at(0).toObject().value("value").toString().toStdString() // I used this to get the string in the drupal value key.

             _db->Put(leveldb::WriteOptions(), key.toStdString() + std::to_string(i) + std::to_string(j), value.toArray().at(0).toObject().value("value").toString().toStdString() );
             bulk.insert(key.toUtf8(), value.toArray().at(0).toObject());
            // qDebug() << key.toUtf8() << value.toString().toUtf8() ;
         }
        _map[i] = bulk;
        leveldb::Slice bulk_conv((char*)&bulk, sizeof(QHash<QByteArray, QVariant>));
        _db->Put(leveldb::WriteOptions(), std::to_string(i), bulk_conv);
     }
     endInsertRows();

     emit countChanged();

}


void populateModelfromDb() {

}

int getRole() {

}


void registerDrupalModelTypes() {
   // qmlRegisterType<QObject>(); // This is not needed. Check Later.
    qmlRegisterType<DrupalModel>("DrupalModel", 0, 1, "DrupalModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerDrupalModelTypes);
