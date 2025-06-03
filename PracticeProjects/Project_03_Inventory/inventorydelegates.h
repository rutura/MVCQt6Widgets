#ifndef INVENTORYDELEGATES_H
#define INVENTORYDELEGATES_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QFileDialog>
#include <QEvent>
#include <QMouseEvent>

// Image Delegate
class ImageDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ImageDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                    const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

// Rating Delegate - Improved star visualization
class RatingDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit RatingDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                    const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    void drawStar(QPainter *painter, const QRect &rect, bool filled) const;
    int starAtPosition(const QPoint &pos, const QStyleOptionViewItem &option) const;
    static constexpr int MaxStars = 5;
    static constexpr int StarSize = 20;
};

#endif // INVENTORYDELEGATES_H
