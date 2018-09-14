#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "defineAll.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


signals:
    void classic_complete(void);
    void survival_complete(void);

private slots:
    void on_pushButton_6_endless_clicked();
    void on_pushButton_main_menu_clicked();
    void on_pushButton_restart_game_clicked();
    void on_label_instruction_2_clicked();
    void on_pushButton_5_random_clicked();
    void on_pushButton_1_playagain_clicked();
    void on_pushButton_2_mainmenu_clicked();
    void on_pushButton_3_combo_clicked();
    void on_pushButton_4_hattrick_clicked();
    void on_pushButton_2_classic_clicked();
    void on_pushButton_1_survival_clicked();
    void on_checkBox_stateChanged(int arg1);



protected:
    void keyPressEvent(QKeyEvent *);


private:
    Ui::Widget *ui;

    void init();                      //初始化
    void substituteCir();       //向前取代
    void invoke();                 //计算出cir_vale[0]
    void showSpells(int inv);  //随机出现祈祷
    void useSpells(int inv);    //传入一个spellvale,使用祈祷
    void changeStats();         //改变状态栏的数值
    void changeLabelTip();    //根据NeedToUse来确定是否改变tiplabel
    void changeRecord();      //改变最佳纪录
    void shrinkProgress();     //缩小进度条
    void resetLevel();            //生存模式：提升游戏难度,并重置进度条
    void over();                    //游戏结束计算成果

    int cir_vale[4];              //对应圈上的值,0代表和(spell1上的值)
    int old_vale;                 //spell2上的值,
    int GameMode;            //表示在游戏的模式状态,0代表在主页
    bool InGame_Status;     //代表是否按下开始游戏
    int randX;                      //随机生成spell
    int randY;
    int randZ;
    int Random_Counts;    //随机生成spell的个数
    bool NeedtoUse;        //确认是否需要使用,于XYZ全部作用
    int x_RGB;
    int y_RGB;
    int currentVale_RGB;
    int survival_Level;
    bool Use_Legacy_Hotkeys;

    int KeyPressed_Counts;              //按键计数
    int SpellsInvoked_Counts;           //祈祷计数
    int SpellsUsed_Counts;              //祈祷使用计数
    int Combinations_Counts;            //组合计数
    int Stage_Counts;                       //阶段计数,越高越难

    QPixmap invoke_Pixmap[10];
    QTimer *general_Timer;
    QTimer *control_Timer;
    int spellVale[10];
    QTime *t;
    float survival_Record;      //存放记录
    float classic_Record;
    int combo_Record;
    int hattrick_Record;
    int random_Record;


};

#endif // WIDGET_H
