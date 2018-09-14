#ifndef XLABEL_H
#define XLABEL_H

#include <QLabel>
#include <QLayout>
#include <QSpacerItem>
#include "defineAll.h"

class XLabel : public QLabel
{
    Q_OBJECT
public:

    explicit XLabel(QWidget *parent=0, Qt::WindowFlags f=0);

    explicit XLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);

protected:
    /*!由Qt自己调用的鼠标事件处理函数,在此发射clicked信号*/
    virtual void mouseReleaseEvent(QMouseEvent * ev);


signals:
    void clicked(void);

public:
    void showOne(int spellVale);
    void showOne(int spellVale,int spellVale1);
    void showOne(int spellVale,int spellVale1,int spellVale2);
    void fadeOne(int spellVale);
    void hideAll();

private:
    QLabel *tmp[6];     //对应可出现的最大combo 三组label{(0,3),(1,4),(2,5),}
    int tmp_vale[3];      //为了记录tmp上放的是哪个invoke
    QHBoxLayout *layout;
    QSpacerItem *spacer1;
    QSpacerItem *spacer2;
};

#endif // XLABEL_H
