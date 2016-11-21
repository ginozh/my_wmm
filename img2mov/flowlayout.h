#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>
//! [0]
//class FlowLayout : public QHBoxLayout
class FlowLayout : public QLayout
//class FlowLayout : public QBoxLayout
//class FlowLayout : public QGridLayout
{
public:
    //explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    //explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit FlowLayout(QWidget *parent, int margin = 0, int hSpacing = 0, int vSpacing = 0);
    explicit FlowLayout(int margin = 0, int hSpacing = 0, int vSpacing = 0);
    ~FlowLayout();

    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;
    int heightForWidth(int) const Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;
    void insertWidget(int index, QWidget *widget);

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};
//! [0]

#endif // FLOWLAYOUT_H
