#ifndef DYNAMICENTRYMODEL_H
#define DYNAMICENTRYMODEL_H

#include <QtCore>
#include <QtGui>

typedef struct ST_TextInfo
{
    bool visible;
    int posx;
    int posy;
    QString text;
    QString color;
    QColor qcolor;
    ST_TextInfo();
}ST_TextInfo;

class DynamicEntryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum RoleNames {
        VisibleRole = Qt::UserRole,
        PosXRole = Qt::UserRole+2,
        PosYRole = Qt::UserRole+3,
        TextRole = Qt::UserRole+4,
        ColorRole = Qt::UserRole+5
    };

    explicit DynamicEntryModel(QObject *parent = 0);
    ~DynamicEntryModel();

    Q_INVOKABLE void insert(int index, const QString& textInfo);
    Q_INVOKABLE void append(const QString& textInfo);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void clear();
    ////Q_INVOKABLE QColor get(int index);
    Q_INVOKABLE void modify(int index, const QString& textInfo);
public: // interface QAbstractListModel
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    int count() const;

signals:
    void countChanged(int arg);

protected: // interface QAbstractListModel
    virtual QHash<int, QByteArray> roleNames() const;
private:
    //QList<QColor> m_data;
    QList<ST_TextInfo> m_data;
    QHash<int, QByteArray> m_roleNames;
    int m_count;
};

#endif // DYNAMICENTRYMODEL_H
