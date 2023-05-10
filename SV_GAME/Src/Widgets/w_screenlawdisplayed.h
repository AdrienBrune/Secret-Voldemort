#ifndef W_SCREENLAWDISPLAYED_H
#define W_SCREENLAWDISPLAYED_H

#include <QPushButton>
#include <QPainter>

#include "Widgets/Components/w_lawcard.h"
#include "../../Shared/Components/c_stack.h"
#include "Handlers/c_soundhandler.h"

class W_ScreenLawDisplayed : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenLawDisplayed(QWidget *parent = nullptr, QList<C_LawCard> laws = QList<C_LawCard>(), bool enableInteraction = true);
    ~W_ScreenLawDisplayed();

signals:
    void sig_discard(W_LawCard*);
    void sig_drawPile();
    void sig_askVeto();
    void sig_timeout();

private slots:
    void onConfirmSelection();
    void onNewSelection();

protected:
    void paintEvent(QPaintEvent*)override;
    void resizeEvent(QResizeEvent*)override;

private:
    void selectAll(){ for(quint8 i = 0; i < wLaws.size(); i++) wLaws[i]->setSelection(W_LawCard::E_SELECTION::selected); }
    QRect getLawCardPosition(quint8 index);

private:
    QString mSentence;
    QList<W_LawCard*> wLaws;
    bool mEnableInteraction;
    QPushButton *mButtonVeto;
    QPushButton *mButtonConfirm;
};

#endif // W_SCREENLAWDISPLAYED_H
