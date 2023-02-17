#ifndef C_TEXTDRAWINGHELPER_H
#define C_TEXTDRAWINGHELPER_H

#include <QObject>
#include <QFontMetrics>
#include <QDebug>

class C_AutoScaleFont;

class C_TextDrawingHelper
{
public:
    static bool optimizeFontSize(QFont *font, QString string, QRect boundingArea, int fontSize = 40)
    {
        QFontMetrics metric(*font);

        if(metric.boundingRect(string).width() < boundingArea.width() && metric.boundingRect(string).height() < boundingArea.height())
        {
            return true;
        }
        else
        {
            font->setPointSize(--fontSize);
            return optimizeFontSize(font, string, boundingArea, fontSize);
        }
    }
};

class C_AutoScaleFont : public QFont
{
public:
    C_AutoScaleFont(QString familyName, QString string, QRect boundingArea, int fontSize = 30)
        : QFont(familyName)
    {
        setPointSize(fontSize);
        C_TextDrawingHelper::optimizeFontSize(this, string, boundingArea, fontSize);
    }
    ~C_AutoScaleFont(){}
};

#endif // C_TEXTDRAWINGHELPER_H
