#include "xlabel.h"

#include <QMouseEvent>
#include <QDebug>


#define cout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"

XLabel::XLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent,f)
{
    int index;
    for( index =0 ;index <6; index++){
    tmp[index] = new QLabel(this);
    tmp[index] ->hide();
    }

    for( index =0 ;index <3; index++){
    tmp[index]->setStyleSheet("border :0px"  );
    tmp[index+3]->setStyleSheet("border :0px;"
                                    "font-weight:600;"
                                    "font: 75 10pt Microsoft YaHei UI");
    }

    layout = new QHBoxLayout(this);
    layout->addStretch();
    layout->addStretch();
    layout->setSpacing(1);
    spacer1 = new QSpacerItem(25,25);
    spacer2 = new QSpacerItem(25,25);


}

XLabel::XLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text,parent,f)
{

}

void XLabel::mouseReleaseEvent(QMouseEvent * ev)
{
    Q_UNUSED(ev)
    emit clicked();
}

void XLabel::showOne(int spellVale){

    tmp_vale[0]=spellVale;

    QString iconPath,fontText;
    switch (spellVale) {
    case COLD_SNAP:
        iconPath=":/icon/cold_snap.png";
        fontText="<b><font color=#63b8ff>ColdSnap</color></b>";
        break;
     case GHOST_WALK:
        iconPath=":/icon/ghost_walk.png";
        fontText="<b><font color=#63b8ff>GhostWalk</color></b>";
        break;
     case ICE_WALL:
        iconPath=":/icon/ice_wall.png";
        fontText="<b><font color=#63b8ff>IceWall</color></b>";
        break;
     case EMP:
        iconPath=":/icon/emp.png";
        fontText="<b><font color=#EE82EE>EMP</color></b>";
        break;
     case TORNADO:
        iconPath=":/icon/tornado.png";
        fontText="<b><font color=#EE82EE>Tornado</color></b>";
        break;
     case ALACRITY:
        iconPath=":/icon/alacrity.png";
        fontText="<b><font color=#EE82EE>Alacrity</color></b>";
        break;
     case SUN_STRIKE:
        iconPath=":/icon/sun_strike.png";
        fontText="<b><font color=#FFB90F>SunStrike</color></b>";
        break;
     case FORGE_SPIRIT:
        iconPath=":/icon/forge_spirit.png";
        fontText="<b><font color=#FFB90F>ForgeSpirit</color></b>";
        break;
     case CHAOS_METEOR:
        iconPath=":/icon/chaos_meteor.png";
        fontText="<b><font color=#FFB90F>ChaosMeteor</color></b>";
        break;
     case DEAFENING_BLAST:
        iconPath=":/icon/deafening_blast.png";
        fontText="<b><font color=#FFB90F>DeafeningBlast</color></b>";
        break;
    default:
        break;
    }
    tmp[0] ->show();
    tmp[3] ->show();
    tmp[0]->setPixmap(QPixmap(iconPath));
    tmp[3]->setText(fontText);
    tmp[0]->adjustSize();
    tmp[3]->adjustSize();

    layout->insertWidget(1,tmp[0]);
    layout->insertWidget(2,tmp[3]);

    tmp[0]->move(tmp[3]->pos()-QPoint(21,0));

}

void XLabel::showOne(int spellVale,int spellVale1){

    tmp_vale[0]=spellVale;
    tmp_vale[1]=spellVale1;

    QString iconPath,fontText;
    for(int index=0;  index<2;  index++){
        switch (tmp_vale[index]) {
        case COLD_SNAP:
            iconPath=":/icon/cold_snap.png";
            fontText="<b><font color=#63b8ff>ColdSnap</color></b>";
            break;
         case GHOST_WALK:
            iconPath=":/icon/ghost_walk.png";
            fontText="<b><font color=#63b8ff>GhostWalk</color></b>";
            break;
         case ICE_WALL:
            iconPath=":/icon/ice_wall.png";
            fontText="<b><font color=#63b8ff>IceWall</color></b>";
            break;
         case EMP:
            iconPath=":/icon/emp.png";
            fontText="<b><font color=#EE82EE>EMP</color></b>";
            break;
         case TORNADO:
            iconPath=":/icon/tornado.png";
            fontText="<b><font color=#EE82EE>Tornado</color></b>";
            break;
         case ALACRITY:
            iconPath=":/icon/alacrity.png";
            fontText="<b><font color=#EE82EE>Alacrity</color></b>";
            break;
         case SUN_STRIKE:
            iconPath=":/icon/sun_strike.png";
            fontText="<b><font color=#FFB90F>SunStrike</color></b>";
            break;
         case FORGE_SPIRIT:
            iconPath=":/icon/forge_spirit.png";
            fontText="<b><font color=#FFB90F>ForgeSpirit</color></b>";
            break;
         case CHAOS_METEOR:
            iconPath=":/icon/chaos_meteor.png";
            fontText="<b><font color=#FFB90F>ChaosMeteor</color></b>";
            break;
         case DEAFENING_BLAST:
            iconPath=":/icon/deafening_blast.png";
            fontText="<b><font color=#FFB90F>DeafeningBlast</color></b>";
            break;
        default:
            break;
        }

    tmp[index] ->show();
    tmp[index+3] ->show();
    tmp[index]->setPixmap(QPixmap(iconPath));
    tmp[index+3]->setText(fontText);
    tmp[index]->adjustSize();
    tmp[index+3]->adjustSize();

    }//for

    spacer1->changeSize(25,25);

    layout->insertWidget(1,tmp[0]);
    layout->insertWidget(2,tmp[3]);
    layout->insertSpacerItem(3,spacer1);
    layout->insertWidget(4,tmp[1]);
    layout->insertWidget(5,tmp[4]);







}

void XLabel::showOne(int spellVale,int spellVale1,int spellVale2){

    tmp_vale[0]=spellVale;
    tmp_vale[1]=spellVale1;
    tmp_vale[2]=spellVale2;

    QString iconPath,fontText;
    for(int index=0;  index<3;  index++){
        switch (tmp_vale[index]) {
        case COLD_SNAP:
            iconPath=":/icon/cold_snap.png";
            fontText="<b><font color=#63b8ff>ColdSnap</color></b>";
            break;
         case GHOST_WALK:
            iconPath=":/icon/ghost_walk.png";
            fontText="<b><font color=#63b8ff>GhostWalk</color></b>";
            break;
         case ICE_WALL:
            iconPath=":/icon/ice_wall.png";
            fontText="<b><font color=#63b8ff>IceWall</color></b>";
            break;
         case EMP:
            iconPath=":/icon/emp.png";
            fontText="<b><font color=#EE82EE>EMP</color></b>";
            break;
         case TORNADO:
            iconPath=":/icon/tornado.png";
            fontText="<b><font color=#EE82EE>Tornado</color></b>";
            break;
         case ALACRITY:
            iconPath=":/icon/alacrity.png";
            fontText="<b><font color=#EE82EE>Alacrity</color></b>";
            break;
         case SUN_STRIKE:
            iconPath=":/icon/sun_strike.png";
            fontText="<b><font color=#FFB90F>SunStrike</color></b>";
            break;
         case FORGE_SPIRIT:
            iconPath=":/icon/forge_spirit.png";
            fontText="<b><font color=#FFB90F>ForgeSpirit</color></b>";
            break;
         case CHAOS_METEOR:
            iconPath=":/icon/chaos_meteor.png";
            fontText="<b><font color=#FFB90F >ChaosMeteor</color></b>";
            break;
         case DEAFENING_BLAST:
            iconPath=":/icon/deafening_blast.png";
            fontText="<b><font color=#FFB90F >DeafeningBlast</color></b>";
            break;
        default:
            break;
        }

    tmp[index] ->show();
    tmp[index+3] ->show();
    tmp[index]->setPixmap(QPixmap(iconPath));
    tmp[index+3]->setText(fontText);
    tmp[index]->adjustSize();
    tmp[index+3]->adjustSize();

    }//for
    spacer1->changeSize(8,25);
    spacer2->changeSize(8,25);

    layout->insertWidget(1,tmp[0]);
    layout->insertWidget(2,tmp[3]);
    layout->insertSpacerItem(3,spacer1);
    layout->insertWidget(4,tmp[1]);
    layout->insertWidget(5,tmp[4]);
    layout->insertSpacerItem(6,spacer2);
    layout->insertWidget(7,tmp[2]);
    layout->insertWidget(8,tmp[5]);








}

void XLabel::fadeOne(int spellVale){

    int index;
    int i;
    /*找到是哪个标签需要fade*/
    for(i=0;i<4;i++)
        if(spellVale == tmp_vale[i]){
            index=i;
            break;
        }
    if(i == 4){
        cout<<"can't fadeOne :WROING spellVale"<<spellVale;
    }



    QString iconPath,fontText;
    switch (spellVale) {
    case COLD_SNAP:
        iconPath=":/icon_fade/cold_snap.png";
        fontText="<b><font color=#12222f>ColdSnap</color></b>";
        break;
     case GHOST_WALK:
        iconPath=":/icon_fade/ghost_walk.png";
        fontText="<b><font color=#12222f>GhostWalk</color></b>";
        break;
     case ICE_WALL:
        iconPath=":/icon_fade/ice_wall.png";
        fontText="<b><font color=#12222f>IceWall</color></b>";
        break;
     case EMP:
        iconPath=":/icon_fade/emp.png";
        fontText="<b><font color=#2f192f>EMP</color></b>";
        break;
     case TORNADO:
        iconPath=":/icon_fade/tornado.png";
        fontText="<b><font color=#2f192f>Tornado</color></b>";
        break;
     case ALACRITY:
        iconPath=":/icon_fade/alacrity.png";
        fontText="<b><font color=#2f192f>Alacrity</color></b>";
        break;
     case SUN_STRIKE:
        iconPath=":/icon_fade/sun_strike.png";
        fontText="<b><font color=#2f2102>SunStrike</color></b>";
        break;
     case FORGE_SPIRIT:
        iconPath=":/icon_fade/forge_spirit.png";
        fontText="<b><font color=#2f2102>ForgeSpirit</color></b>";
        break;
     case CHAOS_METEOR:
        iconPath=":/icon_fade/chaos_meteor.png";
        fontText="<b><font color=#2f2102>ChaosMeteor</color></b>";
        break;
     case DEAFENING_BLAST:
        iconPath=":/icon_fade/deafening_blast.png";
        fontText="<b><font color=#2f2102>DeafeningBlast</color></b>";
        break;
    default:
        break;
    }


    tmp[index]->setPixmap(QPixmap(iconPath));
    tmp[index+3]->setText(fontText);
    tmp[index]->adjustSize();
    tmp[index+3]->adjustSize();

}

void XLabel::hideAll(){
    for(int index =0;index<6;index++){
        tmp[index] ->hide();
        layout->removeWidget(tmp[index]);
    }


    layout->removeItem(spacer1);
    layout->removeItem(spacer2);

}


