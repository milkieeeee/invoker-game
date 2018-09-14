#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QEvent>
#include <QTime>
#include <QtGlobal>
#include <QTimer>
#include <QTime>

#define cout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();

}

Widget::~Widget()
{
    delete ui;
    this->setWindowIcon(QIcon(":/image/td.ico"));
}

void Widget::init()
{
     /*初始化游戏模式*/
    this->GameMode = NOTINGAME;
    this->InGame_Status = false;

    /*初始化圈值*/
    for(int i=0;i<4;i++)
        cir_vale[i]=0;

    /*加载图片到数组*/
    invoke_Pixmap[0].load(":/image/cold_snap.png");
    invoke_Pixmap[1].load(":/image/ghost_walk.png");
    invoke_Pixmap[2].load(":/image/ice_wall.png");
    invoke_Pixmap[3].load(":/image/emp.png");
    invoke_Pixmap[4].load(":/image/tornado.png");
    invoke_Pixmap[5].load(":/image/alacrity.png");
    invoke_Pixmap[6].load(":/image/sun_strike.png");
    invoke_Pixmap[7].load(":/image/forge_spirit.png");
    invoke_Pixmap[8].load(":/image/chaos_meteor.png");
    invoke_Pixmap[9].load(":/image/deafening_blast.png");

    /*加载对应祈祷值*/
    spellVale[0]= COLD_SNAP;
    spellVale[1]= GHOST_WALK;
    spellVale[2]= ICE_WALL;
    spellVale[3]= EMP;
    spellVale[4]= TORNADO;
    spellVale[5]= ALACRITY;
    spellVale[6]= SUN_STRIKE;
    spellVale[7]= FORGE_SPIRIT;
    spellVale[8]= CHAOS_METEOR;
    spellVale[9]= DEAFENING_BLAST;
    /*初始化label_instruction*/
    ui->label_instruction->setText("Instructions");
    ui->label_instruction_2->setText("START GAME (Press Enter)");
    ui->label_instruction_2->setCursor(Qt::PointingHandCursor);

    /*设置页面*/
    ui->stackedWidget->setCurrentIndex(0);

    /*初始化种子*/
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    /*链接信号和槽*/
    general_Timer = new QTimer(this);
    control_Timer = new QTimer(this);
    t = new QTime;
    connect(general_Timer, &QTimer::timeout, this, &Widget::over);
    connect(control_Timer, &QTimer::timeout, this, &Widget::shrinkProgress);
    connect(this, &Widget::classic_complete, this, &Widget::over);
    connect(this, &Widget::survival_complete, this, &Widget::over);

    /*初始化状态值*/
    KeyPressed_Counts=0;
    SpellsInvoked_Counts=0;
    SpellsUsed_Counts=0;
    Combinations_Counts=0;
    Stage_Counts=0;
    changeStats();

    /*清空randX/Y/Z的值*/
    randX=0;
    randY=0;
    randZ=0;

    /*进度条不显示*/
    ui->progressBar->setVisible(false);
    survival_Level = 0;

    /*使用非传统键*/
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->label_11->setVisible(false);
    ui->label_12->setVisible(false);
    ui->label_13->setVisible(false);
    ui->label_14->setVisible(false);
    ui->label_15->setVisible(false);
    ui->label_16->setVisible(false);
    ui->label_17->setVisible(false);
    ui->label_5_Spell1->setVisible(true);
    ui->label_6_Spell2->setVisible(true);
    ui->checkBox->move(8,160);
    ui->CONTROLS_frame->setFixedSize(209,191);
    ui->STATS_frame->move(41,260);
    ui->label_D->setText("D");
    ui->label_F->setText("F");
    Use_Legacy_Hotkeys=false;

    /*从文件中读取最佳纪录,假设全为0*/

    survival_Record=0;
    classic_Record=0;
    combo_Record=0;
    hattrick_Record=0;
    random_Record=0;


    changeRecord();

}

void Widget::on_pushButton_6_endless_clicked()
{

    this->GameMode = ENDLESS;
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButton_5_random_clicked()
{

    this->GameMode = RANDOM;
    ui->stackedWidget->setCurrentIndex(1);
    general_Timer->setInterval(LIMITED_TIME *1000);
    general_Timer->setSingleShot(true);
}

void Widget::on_pushButton_3_combo_clicked()
{
    this->GameMode = COMBO;
    ui->stackedWidget->setCurrentIndex(1);
    general_Timer->setInterval(LIMITED_TIME *1000);
    general_Timer->setSingleShot(true);
}

void Widget::on_pushButton_4_hattrick_clicked()
{
    this->GameMode = HATTRICK;
    ui->stackedWidget->setCurrentIndex(1);
    general_Timer->setInterval(LIMITED_TIME *1000);
    general_Timer->setSingleShot(true);
}

void Widget::on_pushButton_2_classic_clicked()
{
    this->GameMode = CLASSIC;
    ui->stackedWidget->setCurrentIndex(1);
    general_Timer->setInterval(VERYLONGTIME);
    general_Timer->setSingleShot(true);
}

void Widget::on_pushButton_1_survival_clicked()
{
    this->GameMode = SURVIVAL;
    ui->stackedWidget->setCurrentIndex(1);
    general_Timer->setInterval(VERYLONGTIME);
    general_Timer->setSingleShot(true);
    control_Timer->setInterval(INIT_INTERVAL);
    x_RGB=INIT_RED;y_RGB=INIT_GREEN;
    currentVale_RGB= MAXPROGRESSVALE;
}

void Widget::on_pushButton_main_menu_clicked()
{
    //TODO:非常多的清除：label 图片 ,stats里的所有
    this->InGame_Status =false;
    this->GameMode = NOTINGAME;
    for(int i =0;i<4;i++)
        cir_vale[i]=0;
    ui->stackedWidget->setCurrentIndex(0);

    /*圈圈里没有东西*/
    ui->label_Cir_1->setStyleSheet("border-radius: 50px");
    ui->label_Cir_2->setStyleSheet("border-radius: 50px");
    ui->label_Cir_3->setStyleSheet("border-radius: 50px");

    /*初始化label_instruction*/
    ui->label_instruction->setText("Instructions");
    ui->label_instruction_2->setText("START GAME (Press Enter)");
    ui->label_instruction_2->setCursor(Qt::PointingHandCursor);
    ui->label_instruction_2->hideAll();

    /*初始化label_spell*/
    ui->label_spell1->setPixmap(QPixmap(":/image/skills2.png"));
    ui->label_spell2->setPixmap(QPixmap(":/image/skills2.png"));

    /*初始化状态值*/
    KeyPressed_Counts=0;
    SpellsInvoked_Counts=0;
    SpellsUsed_Counts=0;
    Combinations_Counts=0;
    Stage_Counts=0;
    changeStats();

    /*清空randX/Y/Z的值*/
    randX=0;
    randY=0;
    randZ=0;

    x_RGB=INIT_RED;y_RGB=INIT_GREEN;
    currentVale_RGB= MAXPROGRESSVALE;
    /*以防在游戏中按*/
    ui->progressBar->setVisible(false);
    control_Timer->stop();
    general_Timer->stop();
    survival_Level = 0;

    /*重置label_D label_F的文字*/
    if(Use_Legacy_Hotkeys == true){
        ui->label_D->setText(" ");
        ui->label_F->setText(" ");
    }
    else{
        ui->label_D->setText("D");
        ui->label_F->setText("F");
    }

}

void Widget::on_pushButton_restart_game_clicked()
{
    this->InGame_Status =false;
    for(int i =0;i<4;i++)
        cir_vale[i]=0;
    /*圈圈里没有东西*/
    ui->label_Cir_1->setStyleSheet("border-radius: 50px");
    ui->label_Cir_2->setStyleSheet("border-radius: 50px");
    ui->label_Cir_3->setStyleSheet("border-radius: 50px");

    /*初始化label_instruction*/
    ui->label_instruction->setText("Instructions");
    ui->label_instruction_2->setText("START GAME (Press Enter)");
    ui->label_instruction_2->setCursor(Qt::PointingHandCursor);
    ui->label_instruction_2->hideAll();

    /*初始化label_spell*/
    ui->label_spell1->setPixmap(QPixmap(":/image/skills2.png"));
    ui->label_spell2->setPixmap(QPixmap(":/image/skills2.png"));

    /*初始化状态值*/
    KeyPressed_Counts=0;
    SpellsInvoked_Counts=0;
    SpellsUsed_Counts=0;
    Combinations_Counts=0;
    Stage_Counts=0;
    changeStats();

    /*清空randX/Y/Z的值*/
    randX=0;
    randY=0;
    randZ=0;
    /*实际上没意义*/
    NeedtoUse = false;

    x_RGB=INIT_RED;y_RGB=INIT_GREEN;
    currentVale_RGB= MAXPROGRESSVALE;
    /*以防在游戏中按*/
    ui->progressBar->setVisible(false);
    control_Timer->stop();
    general_Timer->stop();
    survival_Level = 0;

    /*重置label_D label_F的文字*/
    if(Use_Legacy_Hotkeys == true){
        ui->label_D->setText(" ");
        ui->label_F->setText(" ");
    }
    else{
        ui->label_D->setText("D");
        ui->label_F->setText("F");
    }

}

void Widget::on_label_instruction_2_clicked()
{
    /*InGame中 点击无效*/
    if(this->InGame_Status == true)
        return;

    this->InGame_Status = true;



    ui->label_instruction->setText("Invoke Spell(s)");
    ui->label_instruction_2->setText("");
    ui->label_instruction_2->setCursor(Qt::ArrowCursor);

    int sb; //都是跟随机有关的
    switch (this->GameMode) {
    case ENDLESS:
        NeedtoUse = false;
        randX = qrand() % 10;
        ui->label_instruction_2->showOne(spellVale[randX]);
        break;
    case RANDOM:
        general_Timer->start();     //开启计时器 总共30秒
        Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

        sb = qrand() % 10;           //确定是否需要使用（一起）
        NeedtoUse = (sb<7) ? false:true;
        changeLabelTip();
        switch (Random_Counts) {
        case 1:
            randX = qrand() % 10;
            randY=-1;
            randZ=-1;
            ui->label_instruction_2->showOne(spellVale[randX]);
            break;
        case 2:
            randX = qrand() % 10;
            randY = qrand() % 10;
            while(randY == randX)
                randY = qrand() % 10;
            randZ=-1;
            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
            break;
        case 3:
            randX = qrand() % 10;
            randY = qrand() % 10;
            while(randY == randX)
                randY = qrand() % 10;
            randZ = qrand() % 10;
            while(randZ ==randX || randZ==randY)
                randZ = qrand() % 10;
            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
            break;

        default:
            break;
        }//switch counts
        break;
    case HATTRICK:
        general_Timer->start();     //开启计时器 总共30秒
        NeedtoUse = true;
        Random_Counts = 3;
        changeLabelTip();

        randX = qrand() % 10;
        randY = qrand() % 10;
        while(randY == randX)
            randY = qrand() % 10;
        randZ = qrand() % 10;
        while(randZ ==randX || randZ==randY)
            randZ = qrand() % 10;
        ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);

        break;
    case COMBO:
        general_Timer->start();     //开启计时器 总共30秒
        NeedtoUse = true;
        Random_Counts = 2;
        changeLabelTip();
        randX = qrand() % 10;
        randY = qrand() % 10;
        while(randY == randX)
            randY = qrand() % 10;
        ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
        break;
    case CLASSIC:
        general_Timer->start();     //开启计时器
        t->restart();
        NeedtoUse = false;
        randX = qrand() % 10;
        ui->label_instruction_2->showOne(spellVale[randX]);
        break;
    case SURVIVAL:
        general_Timer->start();     //开启计时器
        control_Timer->start();
        t->restart();
        ui->progressBar->setVisible(true);
        Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个
        sb = qrand() % 10;           //确定是否需要使用（一起）
        NeedtoUse = (sb<7) ? false:true;
        changeLabelTip();
        switch (Random_Counts) {
        case 1:
            randX = qrand() % 10;
            randY=-1;
            randZ=-1;
            ui->label_instruction_2->showOne(spellVale[randX]);
            break;
        case 2:
            randX = qrand() % 10;
            randY = qrand() % 10;
            while(randY == randX)
                randY = qrand() % 10;
            randZ=-1;
            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
            break;
        case 3:
            randX = qrand() % 10;
            randY = qrand() % 10;
            while(randY == randX)
                randY = qrand() % 10;
            randZ = qrand() % 10;
            while(randZ ==randX || randZ==randY)
                randZ = qrand() % 10;
            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
            break;

        default:
            break;
        }//switch counts
        break;
    default:
        break;
    }




}

void Widget::on_pushButton_1_playagain_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    on_pushButton_restart_game_clicked();
}

void Widget::on_pushButton_2_mainmenu_clicked()
{
    on_pushButton_main_menu_clicked();
}

void Widget::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2){//使用传统键
        ui->label_8->setVisible(true);
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->label_11->setVisible(true);
        ui->label_12->setVisible(true);
        ui->label_13->setVisible(true);
        ui->label_14->setVisible(true);
        ui->label_15->setVisible(true);
        ui->label_16->setVisible(true);
        ui->label_17->setVisible(true);
        ui->label_5_Spell1->setVisible(false);
        ui->label_6_Spell2->setVisible(false);
        ui->checkBox->move(8,320);
        ui->CONTROLS_frame->setFixedSize(209,361);
        ui->STATS_frame->move(41,420);
        ui->label_D->setText(" ");
        ui->label_F->setText(" ");
        Use_Legacy_Hotkeys=true;
    }
    else{
        ui->label_8->setVisible(false);
        ui->label_9->setVisible(false);
        ui->label_10->setVisible(false);
        ui->label_11->setVisible(false);
        ui->label_12->setVisible(false);
        ui->label_13->setVisible(false);
        ui->label_14->setVisible(false);
        ui->label_15->setVisible(false);
        ui->label_16->setVisible(false);
        ui->label_17->setVisible(false);
        ui->label_5_Spell1->setVisible(true);
        ui->label_6_Spell2->setVisible(true);
        ui->checkBox->move(8,160);
        ui->CONTROLS_frame->setFixedSize(209,191);
        ui->STATS_frame->move(41,260);
        ui->label_D->setText("D");
        ui->label_F->setText("F");
        Use_Legacy_Hotkeys=false;
    }


}

void Widget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        on_label_instruction_2_clicked();

    if(!this->InGame_Status)//如果不在游戏状态
        return;

    this->KeyPressed_Counts++;
    switch (e->key()) {
    case Qt::Key_Q:
        //计算对应圈上的值,切换图片,
        cir_vale[3]=cir_vale[2];
        cir_vale[2]=cir_vale[1];
        cir_vale[1]=QUAS;
        substituteCir();
        break;
    case Qt::Key_W:
        cir_vale[3]=cir_vale[2];
        cir_vale[2]=cir_vale[1];
        cir_vale[1]=WEX;
        substituteCir();
        break;
    case Qt::Key_E:
        cir_vale[3]=cir_vale[2];
        cir_vale[2]=cir_vale[1];
        cir_vale[1]=EXORT;
        substituteCir();
        break;
    case Qt::Key_R:
        this->SpellsInvoked_Counts++;
        invoke();
        showSpells(cir_vale[0]);
        break;

        break;
    default:
        break;
    }

    if(Use_Legacy_Hotkeys == true)
        switch (e->key()) {
        case Qt::Key_Y:
            this->SpellsUsed_Counts++;
            useSpells(COLD_SNAP);
            break;
        case Qt::Key_V:
            this->SpellsUsed_Counts++;
            useSpells(GHOST_WALK);
        case Qt::Key_G:
            this->SpellsUsed_Counts++;
            useSpells(ICE_WALL);
        case Qt::Key_C:
            this->SpellsUsed_Counts++;
            useSpells(EMP);
            break;
        case Qt::Key_X:
            this->SpellsUsed_Counts++;
            useSpells(TORNADO);
        case Qt::Key_Z:
            this->SpellsUsed_Counts++;
            useSpells(ALACRITY);
            break;
        case Qt::Key_T:
            this->SpellsUsed_Counts++;
            useSpells(SUN_STRIKE);
        case Qt::Key_F:
            this->SpellsUsed_Counts++;
            useSpells(FORGE_SPIRIT);
            break;
        case Qt::Key_D:
            this->SpellsUsed_Counts++;
            useSpells(CHAOS_METEOR);
        case Qt::Key_B:
            this->SpellsUsed_Counts++;
            useSpells(DEAFENING_BLAST);
            break;
        default:
            break;
        }
    else{
        switch (e->key()) {
        case Qt::Key_D:
            this->SpellsUsed_Counts++;
            useSpells(cir_vale[0]);
            break;
        case Qt::Key_F:
            this->SpellsUsed_Counts++;
            useSpells(old_vale);
            break;
        }
    }









    changeStats();
}

void Widget::substituteCir()
{
    switch (cir_vale[1]) {
    case QUAS:
        ui->label_Cir_1->setStyleSheet("border-radius: 50px;border-image: url(:/image/quas.png)-1");
        break;
    case WEX:
        ui->label_Cir_1->setStyleSheet("border-radius: 50px;border-image: url(:/image/wex.png)-1");
        break;
    case EXORT:
        ui->label_Cir_1->setStyleSheet("border-radius: 50px;border-image: url(:/image/exort.png)-1");
        break;
    default:
        break;
    }
    switch (cir_vale[2]) {
    case QUAS:
        ui->label_Cir_2->setStyleSheet("border-radius: 50px;border-image: url(:/image/quas.png)-1");
        break;
    case WEX:
        ui->label_Cir_2->setStyleSheet("border-radius: 50px;border-image: url(:/image/wex.png)-1");
        break;
    case EXORT:
        ui->label_Cir_2->setStyleSheet("border-radius: 50px;border-image: url(:/image/exort.png)-1");
        break;
    default:
        break;
    }
    switch (cir_vale[3]) {
    case QUAS:
        ui->label_Cir_3->setStyleSheet("border-radius: 50px;border-image: url(:/image/quas.png)-1");
        break;
    case WEX:
        ui->label_Cir_3->setStyleSheet("border-radius: 50px;border-image: url(:/image/wex.png)-1");
        break;
    case EXORT:
        ui->label_Cir_3->setStyleSheet("border-radius: 50px;border-image: url(:/image/exort.png)-1");
        break;
    default:
        break;
    }

}

void Widget::invoke()
{
    old_vale=cir_vale[0];
    cir_vale[0]=cir_vale[1]+cir_vale[2]+cir_vale[3];
    if(old_vale == cir_vale[0])//如果没有改变的话，图片不进行替换
        return;

    QImage tmp = ui->label_spell1->pixmap()->toImage();
    ui->label_spell2->setPixmap(QPixmap::fromImage(tmp));

    if(Use_Legacy_Hotkeys == true){
        QString tmp2 = ui->label_D->text();
        ui->label_F->setText(tmp2);
    }
    //qDebug()<<cir_vale[1]<<"/"<<cir_vale[2]<<"/"<<cir_vale[3]<<"/"<<cir_vale[0];

    switch (cir_vale[0]) {
    case COLD_SNAP:
        ui->label_spell1->setPixmap(invoke_Pixmap[0]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("Y");
        break;
     case GHOST_WALK:
        ui->label_spell1->setPixmap(invoke_Pixmap[1]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("V");
        break;
     case ICE_WALL:
        ui->label_spell1->setPixmap(invoke_Pixmap[2]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("G");
        break;
     case EMP:
        ui->label_spell1->setPixmap(invoke_Pixmap[3]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("C");
        break;
     case TORNADO:
        ui->label_spell1->setPixmap(invoke_Pixmap[4]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("X");
        break;
     case ALACRITY:
        ui->label_spell1->setPixmap(invoke_Pixmap[5]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("Z");
        break;
     case SUN_STRIKE:
        ui->label_spell1->setPixmap(invoke_Pixmap[6]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("T");
        break;
     case FORGE_SPIRIT:
        ui->label_spell1->setPixmap(invoke_Pixmap[7]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("F");
        break;
     case CHAOS_METEOR:
        ui->label_spell1->setPixmap(invoke_Pixmap[8]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("D");
        break;
     case DEAFENING_BLAST:
        ui->label_spell1->setPixmap(invoke_Pixmap[9]);
        if(Use_Legacy_Hotkeys == true)
            ui->label_D->setText("B");
        break;
    default:
        break;
    }
}

void Widget::showSpells(int inv)
{
    //qDebug()<<randX<<cir_vale[0] << spellVale[randX];

    switch(this->GameMode){
        case ENDLESS:
            if(inv == spellVale[randX]){
                randX = qrand() % 10;
                ui->label_instruction_2->showOne(spellVale[randX]);
            }
            break;
        case RANDOM:
            if(inv==spellVale[randX]){
                 if(NeedtoUse == false){
                     ui->label_instruction_2->fadeOne(spellVale[randX]);
                     randX=-1;//-1代表变灰色了
                     /*判断是否所有都变灰了*/
                     if(randX==-1&&randY==-1&&randZ==-1){
                             ui->label_instruction_2->hideAll();
                             Stage_Counts++;
                             if(Random_Counts != 1)
                                 Combinations_Counts++;
                             Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                             int sb = qrand() % 10;            // //确定是否需要使用（一起）
                             NeedtoUse = (sb<7) ? false:true;
                             changeLabelTip();
                             switch (Random_Counts) {
                             case 1:
                                 randX = qrand() % 10;
                                 randY=-1;
                                 randZ=-1;
                                 ui->label_instruction_2->showOne(spellVale[randX]);
                                 break;
                             case 2:
                                 randX = qrand() % 10;
                                 randY = qrand() % 10;
                                 while(randY == randX)
                                     randY = qrand() % 10;
                                 randZ=-1;
                                 ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                                 break;
                             case 3:
                                 randX = qrand() % 10;
                                 randY = qrand() % 10;
                                 while(randY == randX)
                                     randY = qrand() % 10;
                                 randZ = qrand() % 10;
                                 while(randZ ==randX || randZ==randY)
                                     randZ = qrand() % 10;
                                 ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                                 break;

                             default:
                                 break;
                             }//switch Random_Counts
                     }// if(randX==-1&&randY==-1&&randZ==-1)
                     else{
                         //nothing
                     }
                }//if(NeedtoUse == false)
            }//if(inv==spellVale[randX])
            else if (inv==spellVale[randY]){
                if(NeedtoUse == false){
                    ui->label_instruction_2->fadeOne(spellVale[randY]);
                    randY=-1;//-1代表变灰色了
                    /*判断是否所有都变灰了*/
                    if(randX==-1&&randY==-1&&randZ==-1){
                            ui->label_instruction_2->hideAll();
                            Stage_Counts++;
                            if(Random_Counts != 1)
                                Combinations_Counts++;
                            Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                            int sb = qrand() % 10;            // //确定是否需要使用（一起）
                            NeedtoUse = (sb<7) ? false:true;
                            changeLabelTip();
                            switch (Random_Counts) {
                            case 1:
                                randX = qrand() % 10;
                                randY=-1;
                                randZ=-1;
                                ui->label_instruction_2->showOne(spellVale[randX]);
                                break;
                            case 2:
                                randX = qrand() % 10;
                                randY = qrand() % 10;
                                while(randY == randX)
                                    randY = qrand() % 10;
                                randZ=-1;
                                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                                break;
                            case 3:
                                randX = qrand() % 10;
                                randY = qrand() % 10;
                                while(randY == randX)
                                    randY = qrand() % 10;
                                randZ = qrand() % 10;
                                while(randZ ==randX || randZ==randY)
                                    randZ = qrand() % 10;
                                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                                break;

                            default:
                                break;
                            }//switch Random_Counts
                    }
                    else{
                        //nothing
                    }
                }
            }//if(inv==spellVale[randY])
            else if (inv==spellVale[randZ]){
                if(NeedtoUse == false){
                    ui->label_instruction_2->fadeOne(spellVale[randZ]);
                    randZ=-1;//-1代表变灰色了
                    /*判断是否所有都变灰了*/
                    if(randX==-1&&randY==-1&&randZ==-1){
                            ui->label_instruction_2->hideAll();
                            Stage_Counts++;
                            if(Random_Counts != 1)
                                Combinations_Counts++;
                            Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                            int sb = qrand() % 10;            // //确定是否需要使用（一起）
                            NeedtoUse = (sb<7) ? false:true;
                            changeLabelTip();
                            switch (Random_Counts) {
                            case 1:
                                randX = qrand() % 10;
                                randY=-1;
                                randZ=-1;

                                ui->label_instruction_2->showOne(spellVale[randX]);
                                break;
                            case 2:
                                randX = qrand() % 10;
                                randY = qrand() % 10;
                                while(randY == randX)
                                    randY = qrand() % 10;
                                randZ=-1;

                                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                                break;
                            case 3:
                                randX = qrand() % 10;
                                randY = qrand() % 10;
                                while(randY == randX)
                                    randY = qrand() % 10;
                                randZ = qrand() % 10;
                                while(randZ ==randX || randZ==randY)
                                    randZ = qrand() % 10;

                                sb = qrand() % 10;

                                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                                break;

                            default:
                                break;
                            }//switch Random_Counts
                    }
                    else{
                        //nothing
                    }
                }
            }//if(cir_vale[0]==spellVale[randZ])
            else{
                //nothing
            }
            break;
        case HATTRICK:
            //do nothing
            break;
        case COMBO:
            //do nothing
            break;
        case CLASSIC:
            if(inv == spellVale[randX]){
                randX = qrand() % 10;
                ui->label_instruction_2->showOne(spellVale[randX]);
                Stage_Counts++;
                if(Stage_Counts == NEEDTOCOMPLETE)
                    emit classic_complete();
            }
            break;
    case SURVIVAL:
        if(inv==spellVale[randX]){
             if(NeedtoUse == false){
                 ui->label_instruction_2->fadeOne(spellVale[randX]);
                 randX=-1;//-1代表变灰色了
                 /*判断是否所有都变灰了*/
                 if(randX==-1&&randY==-1&&randZ==-1){
                         ui->label_instruction_2->hideAll();
                         Stage_Counts++;
                         if(Random_Counts != 1)
                             Combinations_Counts++;
                         Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                         int sb = qrand() % 10;            // //确定是否需要使用（一起）
                         NeedtoUse = (sb<7) ? false:true;
                         changeLabelTip();
                         resetLevel();
                         switch (Random_Counts) {
                         case 1:
                             randX = qrand() % 10;
                             randY=-1;
                             randZ=-1;
                             ui->label_instruction_2->showOne(spellVale[randX]);
                             break;
                         case 2:
                             randX = qrand() % 10;
                             randY = qrand() % 10;
                             while(randY == randX)
                                 randY = qrand() % 10;
                             randZ=-1;
                             ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                             break;
                         case 3:
                             randX = qrand() % 10;
                             randY = qrand() % 10;
                             while(randY == randX)
                                 randY = qrand() % 10;
                             randZ = qrand() % 10;
                             while(randZ ==randX || randZ==randY)
                                 randZ = qrand() % 10;
                             ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                             break;

                         default:
                             break;
                         }//switch Random_Counts
                 }// if(randX==-1&&randY==-1&&randZ==-1)
                 else{
                     //nothing
                 }
            }//if(NeedtoUse == false)
        }//if(inv==spellVale[randX])
        else if (inv==spellVale[randY]){
            if(NeedtoUse == false){
                ui->label_instruction_2->fadeOne(spellVale[randY]);
                randY=-1;//-1代表变灰色了
                /*判断是否所有都变灰了*/
                if(randX==-1&&randY==-1&&randZ==-1){
                        ui->label_instruction_2->hideAll();
                        Stage_Counts++;
                        if(Random_Counts != 1)
                            Combinations_Counts++;
                        Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                        int sb = qrand() % 10;            // //确定是否需要使用（一起）
                        NeedtoUse = (sb<7) ? false:true;
                        changeLabelTip();
                        resetLevel();
                        switch (Random_Counts) {
                        case 1:
                            randX = qrand() % 10;
                            randY=-1;
                            randZ=-1;
                            ui->label_instruction_2->showOne(spellVale[randX]);
                            break;
                        case 2:
                            randX = qrand() % 10;
                            randY = qrand() % 10;
                            while(randY == randX)
                                randY = qrand() % 10;
                            randZ=-1;
                            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                            break;
                        case 3:
                            randX = qrand() % 10;
                            randY = qrand() % 10;
                            while(randY == randX)
                                randY = qrand() % 10;
                            randZ = qrand() % 10;
                            while(randZ ==randX || randZ==randY)
                                randZ = qrand() % 10;
                            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                            break;

                        default:
                            break;
                        }//switch Random_Counts
                }
                else{
                    //nothing
                }
            }
        }//if(inv==spellVale[randY])
        else if (inv==spellVale[randZ]){
            if(NeedtoUse == false){
                ui->label_instruction_2->fadeOne(spellVale[randZ]);
                randZ=-1;//-1代表变灰色了
                /*判断是否所有都变灰了*/
                if(randX==-1&&randY==-1&&randZ==-1){
                        ui->label_instruction_2->hideAll();
                        Stage_Counts++;
                        if(Random_Counts != 1)
                            Combinations_Counts++;
                        Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个

                        int sb = qrand() % 10;            // //确定是否需要使用（一起）
                        NeedtoUse = (sb<7) ? false:true;
                        changeLabelTip();
                        resetLevel();
                        switch (Random_Counts) {
                        case 1:
                            randX = qrand() % 10;
                            randY=-1;
                            randZ=-1;

                            ui->label_instruction_2->showOne(spellVale[randX]);
                            break;
                        case 2:
                            randX = qrand() % 10;
                            randY = qrand() % 10;
                            while(randY == randX)
                                randY = qrand() % 10;
                            randZ=-1;

                            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                            break;
                        case 3:
                            randX = qrand() % 10;
                            randY = qrand() % 10;
                            while(randY == randX)
                                randY = qrand() % 10;
                            randZ = qrand() % 10;
                            while(randZ ==randX || randZ==randY)
                                randZ = qrand() % 10;

                            sb = qrand() % 10;

                            ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                            break;

                        default:
                            break;
                        }//switch Random_Counts
                }
                else{
                    //nothing
                }
            }
        }//if(cir_vale[0]==spellVale[randZ])
        else{
            //nothing
        }
        break;
    default:
        break;
        }//switch(this->GameMode)

}

void Widget::useSpells(int inv)
{
    switch (this->GameMode) {
    case ENDLESS:
        //nothing
        break;
    case RANDOM:
        if(inv == spellVale[randX]){
            randX=-1;//-1代表变灰色了
        }
        else if(inv == spellVale[randY]){
            randY=-1;//-1代表变灰色了
        }
        else if(inv == spellVale[randZ]) {
            randZ=-1;//-1代表变灰色了
        }
        else{
            return;
        }

        ui->label_instruction_2->fadeOne(inv);
        /*判断是否所有都变灰了*/
        if(randX==-1&&randY==-1&&randZ==-1){
                ui->label_instruction_2->hideAll();
                Stage_Counts++;
                if(Random_Counts != 1)
                    Combinations_Counts++;
                Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个
                int sb=qrand() % 10;
                NeedtoUse = (sb<7) ? false:true;
                changeLabelTip();
                switch (Random_Counts) {
                case 1:
                    randX = qrand() % 10;
                    randY=-1;
                    randZ=-1;
                    ui->label_instruction_2->showOne(spellVale[randX]);
                    break;
                case 2:
                    randX = qrand() % 10;
                    randY = qrand() % 10;
                    while(randY == randX)
                        randY = qrand() % 10;
                    randZ=-1;
                    ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                    break;
                case 3:
                    randX = qrand() % 10;
                    randY = qrand() % 10;
                    while(randY == randX)
                        randY = qrand() % 10;
                    randZ = qrand() % 10;
                    while(randZ ==randX || randZ==randY)
                        randZ = qrand() % 10;
                    ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                    break;
                default:
                    break;
                }//switch Random_Counts
        }// if(randX==-1&&randY==-1&&randZ==-1)
        else{
            //nothing
        }
        break;
    case HATTRICK:
        if(inv==spellVale[randX]){
             ui->label_instruction_2->fadeOne(spellVale[randX]);
             randX=-1;//-1代表变灰色了
             /*判断是否所有都变灰了*/
             if(randX==-1&&randY==-1&&randZ==-1){
                 ui->label_instruction_2->hideAll();
                 Stage_Counts++;
                 Combinations_Counts++;
                 randX = qrand() % 10;
                 randY = qrand() % 10;
                 while(randY == randX)
                     randY = qrand() % 10;
                 randZ = qrand() % 10;
                 while(randZ ==randX || randZ==randY)
                     randZ = qrand() % 10;
                 ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
             }// if(randX==-1&&randY==-1&&randZ==-1)

        }//if(inv==spellVale[randX])
        else if (inv==spellVale[randY]){
            ui->label_instruction_2->fadeOne(spellVale[randY]);
            randY=-1;//-1代表变灰色了
            /*判断是否所有都变灰了*/
            if(randX==-1&&randY==-1&&randZ==-1){
                ui->label_instruction_2->hideAll();
                Stage_Counts++;
                Combinations_Counts++;
                randX = qrand() % 10;
                randY = qrand() % 10;
                while(randY == randX)
                    randY = qrand() % 10;
                randZ = qrand() % 10;
                while(randZ ==randX || randZ==randY)
                    randZ = qrand() % 10;
                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
            }// if(randX==-1&&randY==-1&&randZ==-1)

        }//if(inv==spellVale[randY])
        else if (inv==spellVale[randZ]){
            ui->label_instruction_2->fadeOne(spellVale[randZ]);
            randZ=-1;//-1代表变灰色了
            /*判断是否所有都变灰了*/
            if(randX==-1&&randY==-1&&randZ==-1){
                ui->label_instruction_2->hideAll();
                Stage_Counts++;
                Combinations_Counts++;
                randX = qrand() % 10;
                randY = qrand() % 10;
                while(randY == randX)
                    randY = qrand() % 10;
                randZ = qrand() % 10;
                while(randZ ==randX || randZ==randY)
                    randZ = qrand() % 10;
                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
            }// if(randX==-1&&randY==-1&&randZ==-1)

        }//if(cir_vale[0]==spellVale[randZ])
        else{
            //nothing
        }
        break;

    case COMBO:
        if(inv==spellVale[randX]){
             ui->label_instruction_2->fadeOne(spellVale[randX]);
             randX=-1;//-1代表变灰色了
             /*判断是否所有都变灰了*/
             if(randX==-1&&randY==-1){
                 ui->label_instruction_2->hideAll();
                 Stage_Counts++;
                 Combinations_Counts++;
                 randX = qrand() % 10;
                 randY = qrand() % 10;
                 while(randY == randX)
                     randY = qrand() % 10;

                 ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
             }// if(randX==-1&&randY==-1&&randZ==-1)

        }//if(inv==spellVale[randX])
        else if (inv==spellVale[randY]){
            ui->label_instruction_2->fadeOne(spellVale[randY]);
            randY=-1;//-1代表变灰色了
            /*判断是否所有都变灰了*/
            if(randX==-1&&randY==-1){
                ui->label_instruction_2->hideAll();
                Stage_Counts++;
                Combinations_Counts++;
                randX = qrand() % 10;
                randY = qrand() % 10;
                while(randY == randX)
                    randY = qrand() % 10;

                ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
            }// if(randX==-1&&randY==-1&&randZ==-1)

        }//if(inv==spellVale[randY])
        break;
    case CLASSIC:
        //do nothing
        break;
    case SURVIVAL:
        if(inv == spellVale[randX]){
            randX=-1;//-1代表变灰色了
        }
        else if(inv == spellVale[randY]){
            randY=-1;//-1代表变灰色了
        }
        else if(inv == spellVale[randZ]) {
            randZ=-1;//-1代表变灰色了
        }
        else{
            return;
        }

        ui->label_instruction_2->fadeOne(inv);
        /*判断是否所有都变灰了*/
        if(randX==-1&&randY==-1&&randZ==-1){
                ui->label_instruction_2->hideAll();
                Stage_Counts++;
                if(Random_Counts != 1)
                    Combinations_Counts++;
                Random_Counts = (qrand() % 3)+1;//确定出现1个2个或3个
                int sb=qrand() % 10;
                NeedtoUse = (sb<7) ? false:true;
                changeLabelTip();
                resetLevel();
                switch (Random_Counts) {
                case 1:
                    randX = qrand() % 10;
                    randY=-1;
                    randZ=-1;
                    ui->label_instruction_2->showOne(spellVale[randX]);
                    break;
                case 2:
                    randX = qrand() % 10;
                    randY = qrand() % 10;
                    while(randY == randX)
                        randY = qrand() % 10;
                    randZ=-1;
                    ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY]);
                    break;
                case 3:
                    randX = qrand() % 10;
                    randY = qrand() % 10;
                    while(randY == randX)
                        randY = qrand() % 10;
                    randZ = qrand() % 10;
                    while(randZ ==randX || randZ==randY)
                        randZ = qrand() % 10;
                    ui->label_instruction_2->showOne(spellVale[randX],spellVale[randY],spellVale[randZ]);
                    break;
                default:
                    break;
                }//switch Random_Counts
        }// if(randX==-1&&randY==-1&&randZ==-1)
        else{
            //nothing
        }
        break;
    default:
        break;
    }
}

void Widget::changeLabelTip()
{
    if(NeedtoUse == false){
        ui->label_instruction->setStyleSheet("background-color: rgb(107, 107, 107);"
                                             "border-style: solid;"
                                             "color: rgb(255, 255, 255);"
                                             "border-radius: 4px;");
        ui->label_instruction->setText("Invoke Spell(s)");

        ui->label_instruction_2->setStyleSheet("color: rgb(255, 255, 255);"
                                               "border-style: solid;"
                                               "border:4px solid rgb(107, 107, 107);"
                                               "border-radius: 25px;");
    }
    else{
        ui->label_instruction->setStyleSheet("background-color: rgb(60, 60, 60);"
                                             "border-style: solid;"
                                             "color: rgb(255, 255, 255);"
                                             "border-radius: 4px;");
        ui->label_instruction->setText("Invoke and Cast");

        ui->label_instruction_2->setStyleSheet("color: rgb(255, 255, 255);"
                                               "border-style: solid;"
                                               "border:4px solid rgb(60, 60, 60);"
                                               "border-radius: 25px;");
    }
}

void Widget::changeStats()
{
    ui->label_1_key_pressed->setText(QString("Keys Pressed:\t\t%1").arg(this->KeyPressed_Counts));
    ui->label_2_spells_invoke->setText(QString("Spells Invoked:\t\t%1").arg(this->SpellsInvoked_Counts));
    ui->label_3_spells_used->setText(QString("Spells Used:\t\t%1").arg(this->SpellsUsed_Counts));
    ui->label_4_combinations->setText(QString("Combinations:\t\t%1").arg(this->Combinations_Counts));
    ui->label_5_stage->setText(QString("Stage:\t\t\t%1").arg(this->Stage_Counts));
}

void Widget::changeRecord()
{
    if(survival_Record)
        ui->label_record_1->setText(QString("%1 sec").arg(QString::number(survival_Record, 'f', 1)));
    if(classic_Record)
        ui->label_record_2->setText(QString("%1 sec").arg(QString::number(classic_Record,'f',1)));
    if(combo_Record)
        ui->label_record_3->setText(QString("%1 combos").arg(combo_Record));
    if(hattrick_Record)
        ui->label_record_4->setText(QString("%1 combos").arg(hattrick_Record));
    if(hattrick_Record)
        ui->label_record_5->setText(QString("%1 stages").arg(random_Record));

}

void Widget::shrinkProgress()
{
    if(x_RGB != END_RED && y_RGB==INIT_GREEN)
        x_RGB++;
    else if(x_RGB == END_RED && y_RGB != END_GREEN )
        y_RGB--;
    ui->progressBar->setValue(currentVale_RGB--);
    ui->progressBar->setStyleSheet(QString("QProgressBar::chunk {"
                                  " background: rgb(%1, %2, 0);"
                                  " border-radius:6px;  }").arg(x_RGB).arg(y_RGB));
    if(currentVale_RGB==0){
        control_Timer->stop();
        emit survival_complete();
    }
}

void Widget::resetLevel()
{
    x_RGB=INIT_RED;y_RGB=INIT_GREEN;
    currentVale_RGB= MAXPROGRESSVALE;
    if(survival_Level != 7)
        survival_Level++;
    control_Timer->start(INIT_INTERVAL -survival_Level);
    ui->progressBar->setValue(MAXPROGRESSVALE);
}

void Widget::over()
{
    this->InGame_Status = false;
    ui->stackedWidget->setCurrentIndex(2);
    if(general_Timer->isActive())
        general_Timer->stop();
    switch (this->GameMode) {
    case RANDOM:
        if(random_Record >= Stage_Counts){
            ui->label_0_finish->setText("RANDOM GAME FINISHED!");
            ui->label_1_result->setText(QString("Stage %1").arg(Stage_Counts));
            ui->label_3_newrecord->setVisible(false);
        }
        else{
            ui->label_0_finish->setText("RANDOM GAME FINISHED!");
            random_Record=Stage_Counts;
            ui->label_1_result->setText(QString("Stage %1").arg(random_Record));
            ui->label_3_newrecord->setVisible(true);
        }
        break;
    case HATTRICK:
        if(hattrick_Record >= Combinations_Counts){
            ui->label_0_finish->setText("HATTRICK GAME FINISHED!");
            ui->label_1_result->setText(QString("Combo %1").arg(Combinations_Counts));
            ui->label_3_newrecord->setVisible(false);
        }
        else{
            ui->label_0_finish->setText("HATTRICK GAME FINISHED!");
            hattrick_Record = Combinations_Counts;
            ui->label_1_result->setText(QString("Combo %1").arg(Combinations_Counts));
            ui->label_3_newrecord->setVisible(true);
        }
        break;
    case COMBO:
        if(combo_Record >= Combinations_Counts){
            ui->label_0_finish->setText("COMBO GAME FINISHED!");
            ui->label_1_result->setText(QString("Combo %1").arg(Combinations_Counts));
            ui->label_3_newrecord->setVisible(false);
        }
        else{
            ui->label_0_finish->setText("COMBO GAME FINISHED!");
            combo_Record = Combinations_Counts;
            ui->label_1_result->setText(QString("Combo %1").arg(combo_Record));
            ui->label_3_newrecord->setVisible(true);
        }
        break;
    case CLASSIC:
        if(classic_Record!=0 && classic_Record <= (t->elapsed()*1.0/1000)){
            ui->label_0_finish->setText("CLASSIC GAME FINISHED!");
            ui->label_1_result->setText(QString("%1 Seconds").arg(QString::number(t->elapsed()*1.0/1000, 'f', 1)));//保留一位小数
            ui->label_3_newrecord->setVisible(false);
        }
        else{
            ui->label_0_finish->setText("CLASSIC GAME FINISHED!");
            classic_Record = t->elapsed()*1.0/1000;
            ui->label_1_result->setText(QString("%1 Seconds").arg(QString::number(classic_Record, 'f', 1)));//保留一位小数
            ui->label_3_newrecord->setVisible(true);
        }
        break;
    case SURVIVAL:
        if(survival_Record >= (t->elapsed()*1.0/1000) ){
            ui->label_0_finish->setText("SURVIVAL GAME FINISHED!");
            ui->label_1_result->setText(QString("%1 Seconds").arg(QString::number(t->elapsed()*1.0/1000, 'f', 1)));//保留一位小数
            ui->label_3_newrecord->setVisible(false);
        }
        else{
            ui->label_0_finish->setText("SURVIVAL GAME FINISHED!");
            survival_Record = t->elapsed()*1.0/1000;
            ui->label_1_result->setText(QString("%1 Seconds").arg(QString::number(survival_Record, 'f', 1)));//保留一位小数
            ui->label_3_newrecord->setVisible(true);
        }
        break;
    default:
        break;
    }
    changeRecord();
}























