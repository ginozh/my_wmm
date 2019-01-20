#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include "dynamicentrymodel.h"

ST_TextInfo::ST_TextInfo()
{
    visible=true;
    posx=posy=0;
}

DynamicEntryModel::DynamicEntryModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[VisibleRole] = "visible";
    m_roleNames[PosXRole] = "posx";
    m_roleNames[PosYRole] = "posy";
    m_roleNames[TextRole] = "text";
    m_roleNames[ColorRole] = "color";
}

DynamicEntryModel::~DynamicEntryModel()
{
}

void DynamicEntryModel::insert(int index, const QString &textInfo)
{
    if(index < 0 || index > m_data.count()) {
        qInfo()<<"DynamicEntryModel::insert error index: "<<index<<" m_data.count: "<<m_data.count();
        return;
    }
    QJsonObject jsonObj;
    QJsonDocument doc = QJsonDocument::fromJson(textInfo.toUtf8());
    if(doc.isNull())
    {
        qInfo()<<"DynamicEntryModel::insert error. doc is null. data: "<<textInfo;
        return;
    }
    if(!doc.isObject())
    {
        qInfo()<<"DynamicEntryModel::insert error. doc is not object . data: "<<textInfo;
        return;
    }
    jsonObj = doc.object(); 
    if(!jsonObj.contains("posx") || !jsonObj["posx"].isDouble() 
            || !jsonObj.contains("posy") ||!jsonObj["posy"].isDouble()
            || !jsonObj.contains("text") ||!jsonObj["text"].isString() || jsonObj["text"].toString().isEmpty()
            || !jsonObj.contains("color") ||!jsonObj["color"].isString() || jsonObj["color"].toString().isEmpty()
      )
    {
        qInfo()<<"DynamicEntryModel::insert error no posx or posy or text or color. index: "<<index
            <<" textInfo: "<<textInfo;
        return;
    }
#define ASSIGN_PROPERTY(name, totype, type) do{ \
    if(jsonObj.contains(#name) && jsonObj[#name].is##totype()) \
    { \
        stTextInfo.name=(type)jsonObj[#name].to##totype(); \
    } \
}while(0);
    ST_TextInfo stTextInfo;
    ASSIGN_PROPERTY(posx, Double, int);
    ASSIGN_PROPERTY(posy, Double, int);
    ASSIGN_PROPERTY(text, String, QString);
    ASSIGN_PROPERTY(color, String, QString);

    QColor color(stTextInfo.color);
    if(!color.isValid()) {
        qInfo()<<"DynamicEntryModel::insert error. color is isvalid. stTextInfo.color: "<<stTextInfo.color
            <<" textInfo: "<<textInfo;
        return;
    }
    stTextInfo.qcolor=color;
    emit beginInsertRows(QModelIndex(), index, index);
    m_data.insert(index, stTextInfo);
    emit endInsertRows();
    emit countChanged(m_data.count());
}

void DynamicEntryModel::modify(int index, const QString &textInfo)
{
    if(index < 0 || index > m_data.count()) {
        qInfo()<<"DynamicEntryModel::modify error. index: "<<index<<" m_data.count: "<<m_data.count();
        return;
    }
    QJsonObject jsonObj;
    QJsonDocument doc = QJsonDocument::fromJson(textInfo.toUtf8());
    if(doc.isNull())
    {
        qInfo()<<"DynamicEntryModel::modify error. doc is null. data: "<<textInfo;
        return;
    }
    if(!doc.isObject())
    {
        qInfo()<<"DynamicEntryModel::modify error. doc is not object . data: "<<textInfo;
        return;
    }
    jsonObj = doc.object(); 
    ST_TextInfo stTextInfo=m_data[index];
    ASSIGN_PROPERTY(posx, Double, int);
    ASSIGN_PROPERTY(posy, Double, int);
    ASSIGN_PROPERTY(text, String, QString);
    ASSIGN_PROPERTY(color, String, QString);

    QColor color(stTextInfo.color);
    if(!color.isValid()) {
        qInfo()<<"DynamicEntryModel::modify error. color is isvalid. stTextInfo.color: "<<stTextInfo.color
            <<" textInfo: "<<textInfo;
        return;
    }
    stTextInfo.qcolor=color;

    m_data[index]=stTextInfo;
    qDebug()<<"DynamicEntryModel::modify index: "<<index<<" color: "<<color;
    QModelIndex topLeft = createIndex(index,index);
    emit QAbstractListModel::dataChanged(topLeft, topLeft);
}

void DynamicEntryModel::append(const QString &textInfo)
{
    insert(count(), textInfo);
}

void DynamicEntryModel::remove(int index)
{
    if(index < 0 || index >= m_data.count()) {
        return;
    }
    emit beginRemoveRows(QModelIndex(), index, index);
    m_data.removeAt(index);
    emit endRemoveRows();
    emit countChanged(m_data.count());
}

void DynamicEntryModel::clear()
{
    emit beginResetModel();
    m_data.clear();
    emit endResetModel();
}
#if 0
QColor DynamicEntryModel::get(int index)
{
    if(index < 0 || index >= m_data.count()) {
        return QColor();
    }
    return m_data.at(index);
}
#endif

int DynamicEntryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant DynamicEntryModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_data.count()) 
    {
        return QVariant();
    }
    const ST_TextInfo& stTextInfo = m_data.at(row);
    switch(role) {
    case VisibleRole:
        return stTextInfo.visible;
    case PosXRole:
        return stTextInfo.posx;
    case PosYRole:
        return stTextInfo.posy;
    case TextRole:
        return stTextInfo.text;
    case ColorRole:
        return stTextInfo.qcolor;
    }
    return QVariant();
}

int DynamicEntryModel::count() const
{
    return rowCount(QModelIndex());
}

QHash<int, QByteArray> DynamicEntryModel::roleNames() const
{
    return m_roleNames;
}
