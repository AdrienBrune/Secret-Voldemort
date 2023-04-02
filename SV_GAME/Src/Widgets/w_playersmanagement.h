#ifndef W_PLAYERSMANAGEMENT_H
#define W_PLAYERSMANAGEMENT_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>

#include "../../Shared/Components/c_player.h"

class W_PlayersManagement : public QScrollArea
{
    Q_OBJECT

public:
    explicit W_PlayersManagement(QWidget *parent = nullptr);
    ~W_PlayersManagement();

signals:
    void sig_playerClicked(C_Player*);

public slots:
    void onUpdateList();

protected:
    void paintEvent(QPaintEvent*)override;
    bool eventFilter(QObject* object, QEvent* event)override;

private:
    QList<C_Player*> *mPlayersList;
    QVBoxLayout *mLayout;
    QWidget *mScrollAreaWidgetContent;
    QPoint mLastMouseDragLocation;
};

#endif // W_PLAYERSMANAGEMENT_H
