#include "haro.h"
#include "ui_haro.h"

Haro::Haro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Haro)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);//去掉窗口标题
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    Qt::WindowFlags m_flags = windowFlags();//保持窗口置顶1
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint);//保持窗口置顶2

    initBtn();//初始化按钮

    // 读取数据库
    db = new Db;
    db->connect();

    this->loadConfigData();

    move(coordX,coordY);
    this->renderBtn(); // 渲染按钮

    timer = new QTimer;
    timer->start(40);//动画速度
    connect(timer,&QTimer::timeout,this,&Haro::eyesMovement);//关联眼部动作


    imageLoad();//载入部位图片
    eyesMovementLoad();//载入表情图片
    flyMovementLoad();
    specialMovementLoad();//载入特殊动作图片

    bodyImage = new QLabel(this);//身体图片指针
    eyesImage = new QLabel(this);//眼部图片指针
    stripeImage = new QLabel(this);//屏幕遮盖条纹图片指针
    earsImage = new QLabel(this);//耳朵图片指针

    // 加载外观
    imageSet(bodyImage,body[bodyNum]);
    imageSet(eyesImage,eyes);
    if(size>140){
        imageSet(stripeImage,stripe);
        stripeImage->show();
    }
    else
        stripeImage->hide();
    imageSet(earsImage,ears1[earsNum]);


    initSystemTray();//初始化系统托盘

    dressWindow = new DressWin();
    setWindow = new SetWin();
    connect(dressWindow, SIGNAL(dressSignal(QString, int)), this, SLOT(updateDressing(QString, int)));
    connect(setWindow, SIGNAL(sizeSignal(int)), this, SLOT(updateSize(int)));
}

Haro::~Haro()
{
    //清理各类指针申请的空间
    delete ui;

    delete earsImage;
    delete bodyImage;
    delete eyesImage;
    delete stripeImage;
}

void Haro::imageSet(QLabel *image,QPixmap map)
{
    //根据size设定各图片大小和坐标
    image->setPixmap(map.scaled(size,size));//使用scaled修改图片大小，能避免图片因缩放模糊
    image->setScaledContents(true);
    image->resize(size,size);
    image->move(this->frameGeometry().width()/2 - size/2,
                this->frameGeometry().height()/2 - size/2);
}

void Haro::imageLoad()
{
    //载入装扮图片
    body.push_back(QPixmap(QString(":/images/appearance/body/def_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/blue_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/pink_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/icefire_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/cat_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/Gundam_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/drill_body.png")));
    body.push_back(QPixmap(QString(":/images/appearance/body/angel_body.png")));

    ears1.push_back(QPixmap(QString(":/images/appearance/ears/def_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/blue_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/pink_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/icefire_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/cat_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/Gundam_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/drill_ears1.png")));
    ears1.push_back(QPixmap(QString(":/images/appearance/ears/angel_ears1.png")));

    ears2.push_back(QPixmap(QString(":/images/appearance/ears/def_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/blue_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/pink_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/icefire_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/cat_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/Gundam_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/drill_ears2.png")));
    ears2.push_back(QPixmap(QString(":/images/appearance/ears/angel_ears2.png")));

    eyes.load(":/images/appearance/eyes/def_eyes.png");
    stripe.load(":/images/appearance/stripe.png");
}

void Haro::initBtn()
{
    // 创建按钮对象
    for (int i = 0; i < 6; i++) {
        btns<<new QPushButton(this);
    }

    // 固定按钮加载
    btns[0]->setIcon(QIcon(":/assets/icons/close.png"));
    btns[0]->setObjectName("m001");
    btns[0]->setToolTip("退出");
    btns[5]->setIcon(QIcon(":/assets/icons/more.png"));
    btns[5]->setObjectName("m002");
    btns[5]->setToolTip("更多（开发中）");
    connect(btns[0], &QPushButton::clicked, this, &Haro::onBtnsClick);
    connect(btns[5], &QPushButton::clicked, this, &Haro::onBtnsClick);
}
void Haro::renderBtn() {
    QString stableStyle = {
        "QPushButton{border:%1px solid rgb(47, 82, 143);background-color:rgb(200,210,255);border-radius: %2px;}"
        "QPushButton::hover{background-color:rgb(170,200,255);}"
        "QPushButton:pressed{background-color:rgb(60,70,200);}"
    };
    QString stableStyleClose = {
        "QPushButton{border:%1px solid rgb(47, 82, 143);background-color:rgb(93,151,255);border-radius: %2px;}"
        "QPushButton::hover{background-color:rgb(170,200,255);}"
        "QPushButton:pressed{background-color:rgb(60,70,200);}"
    };
    btnSize = size<510?size/3:170;
    int btnX = this->frameGeometry().width()/2-btnSize/2;
    int btnY = this->frameGeometry().height()/2+size/2;
    int btnX_relative[5] = {0, int(-btnSize*1.2), int(+btnSize*1.2), int(-btnSize*2.2), int(+btnSize*2.2)};
    int btnY_relative[5] = {0, int(-btnSize*0.2), int(-btnSize*0.2), int(-btnSize*0.8), int(-btnSize*0.8)};
    QSize iconsize(btnSize/2,btnSize/2);

    // 关闭按钮
    btns[0]->setStyleSheet(stableStyleClose.arg(btnSize/10).arg(btnSize/2));
    btns[0]->setGeometry(this->frameGeometry().width()/2+size/2,this->frameGeometry().height()/2-size/1.7,btnSize,btnSize);
    btns[0]->setIconSize(iconsize);
    btns[0]->setVisible(btnStatus%2);

    // 主菜单
    if (btnStatus < 2) {
        for (int i = 1; i < 6; i++) {
            btns[i]->setStyleSheet(stableStyle.arg(btnSize/10).arg(btnSize/2));
            btns[i]->setGeometry(btnX+btnX_relative[i-1], btnY+btnY_relative[i-1], btnSize, btnSize);
            btns[i]->setIconSize(iconsize);
            btns[i]->setVisible(btnStatus%2);
        }
    }
}

void Haro::hideMenuBtns() {
    if (btnStatus % 2) { // 若为奇数，则当前为显示状态，需-1隐藏
        this->btnStatus -= 1;
        this->renderBtn();
    }
}

void Haro::hideMenuBtns(int status) {
    if (status != 1) return;
    if (btnStatus % 2) { // 若为奇数，则当前为显示状态，需-1隐藏
        this->btnStatus -= 1;
    } else {
        this->btnStatus += 1;
    }
    this->renderBtn();
}



// systemTray：托盘、通知
void Haro::initSystemTray()
{

    pSystemTray = new QSystemTrayIcon(this);
    pSystemTray->setIcon(QIcon(":/assets/icons/haro_icon.ico"));
    pSystemTray->setToolTip("你好，我是学术妲己Haro~");

    // 托盘菜单
    QAction *menuDisplay = new QAction("显示Haro", this);
    QAction *menuRestore = new QAction("重置位置（找不到Haro时点击这个）", this);
    QAction *menuExit = new QAction("退出", this);
    connect(menuDisplay, &QAction::triggered, this, &Haro::restoreWindows);
    connect(menuRestore, &QAction::triggered, this, &Haro::restorePosition);
    connect(menuExit, &QAction::triggered, this, &Haro::closeBtnPush);

    QMenu *menu = new QMenu(this);
    menu->addAction(menuDisplay);
    menu->addAction(menuRestore);
    menu->addAction(menuExit);
    pSystemTray->setContextMenu(menu);

    pSystemTray->show();
    connect(pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayPush(QSystemTrayIcon::ActivationReason)));
}

// 从最小化窗口恢复
void Haro::restoreWindows() {
    if (this->isHidden()) {
        this->show();
    }
}

// 动态按钮响应
void Haro::onBtnsClick() {
    QPushButton* btn = static_cast<QPushButton*>(QObject::sender());
    QString btn_name = btn->objectName();

    if (btn_name == "m001") {
        this->closeBtnPush();
    } else if(btn_name == "m101") {
        this->dressBtnPush();
    } else if (btn_name == "m102") {
        this->setBtnPush();
    } else if (btn_name == "m103") {
        this->minBtnPush();
    } else {
        pSystemTray->showMessage(btn->toolTip(), "功能开发中", QSystemTrayIcon::NoIcon, 100);
    }
}

// 关闭
void Haro::closeBtnPush()
{
    QApplication::exit(0);
}

// 最小化
void Haro::minBtnPush()
{
    this->hide();

    pSystemTray->showMessage("Haro", "已最小化在托盘显示，双击任务栏图标恢复显示", QSystemTrayIcon::NoIcon, 1000);

    this->hideMenuBtns();
}

void Haro::systemTrayPush(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        this->restoreWindows();
    }
}

void Haro::restorePosition() {
    move(0, 0);
    updateConfigData("coordinate_x", 0);
    updateConfigData("coordinate_y", 0);

    pSystemTray->showMessage("Haro", "已恢复至默认位置", QSystemTrayIcon::NoIcon, 1000);

    this->restoreWindows();
}

void Haro::dressBtnPush()
{
    this->hideMenuBtns();
    dressWindow->accept(body,ears1,bodyNum,earsNum);
    dressWindow->show();
}

void Haro::setBtnPush()
{
    this->hideMenuBtns();
    setWindow->setSize(size);//为设置窗口传入size参数
    setWindow->show();
}

//void Haro::gameBtnPush()
//{
//    //隐藏所有窗口
//    this->hide();
//    setWindow->hide();
//    calenWindow->hide();
//    musicWindow->hide();
//    QDir dir( "./game/Sky_island/Release/Sky_island.exe.lnk");//获取相对路径
//    QString temDir = dir.absolutePath();//通过相对路径获取绝对路径
//    system(temDir.toLatin1());

//    //↑通过cmd启动游戏，toLatin1()将QString类型转为char*类型
//    //阻塞式启动，关闭后游戏窗口后才运行下面语段↓
//    //隐藏按钮↓
//    btnSwitch_1=0;
//    btnSwitch_2=0;
//    btnSwitchRole();

//    this->show();
//}

void Haro::mouseMoveEvent(QMouseEvent *event)
{
    if (draggingCount > 0) {
        draggingCount += 1;
        // 处理阈值
        if (draggingCount > 10) {
            this->move(event->globalPos()-moveLeftTop);

            // 拖动时收起按钮
            this->hideMenuBtns();
        }
     }
}

void Haro::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().y() > this->frameGeometry().height()/2 - size/2 &&
            event->pos().y() < this->frameGeometry().height()/2 + size/2 &&
            event->pos().x() > this->frameGeometry().width()/2 - size/2 &&
            event->pos().x() < this->frameGeometry().width()/2 + size/2) {
        moveLeftTop = event->globalPos() - this->pos();
        draggingCount = 1;
    }
}

void Haro::mouseReleaseEvent(QMouseEvent *event)
{
    // 更新位置存储
    if (draggingCount > 10) {
        this->updateConfigData("coordinate_x", x());
        this->updateConfigData("coordinate_y", y());
        draggingCount = 0;
        flyMove = 0;
        face = 0;
        return;
    }
    draggingCount = 0;

    static int flag = 0;//触发特殊动作的计数变量
    //鼠标左键事件,切换表情
    if(event->button() == Qt::LeftButton) {
        if(face<0&&spMove<0){//随机播放表情
            face = qrand()%(faceSum-1)+1;
            flag++;
            if(flag==10){//触发蓝屏
                flag = 0;
                spMove = 0;
                face = -1;
            }
        }
    } else if (event->button() == Qt::RightButton){ //鼠标右键事件
        this->hideMenuBtns(1);
    }
}

void Haro::eyesMovementLoad()
{
    faceNum.push_back(9);//帧数-例：9代表9帧
    faceNum.push_back(0);//起始位置-例：0代表该表情第一张图片下标
    for(int i = 1; i<=faceNum[0]; i++)//表情1-眨眼
        movement.push_back(QPixmap(QString(":/images/movement/blink/%1.png").arg(i)));
    faceNum.push_back(12);
    faceNum.push_back(9);
    for(int i = 1; i<=faceNum[2]; i++)//表情2-心动
        movement.push_back(QPixmap(QString(":/images/movement/heart/%1.png").arg(i)));
    faceNum.push_back(16);
    faceNum.push_back(21);
    for(int i = 1; i<=faceNum[4]; i++)//表情3-疑惑
        movement.push_back(QPixmap(QString(":/images/movement/question/%1.png").arg(i)));
    faceNum.push_back(15);
    faceNum.push_back(37);
    for(int i = 1; i<=faceNum[6]; i++)//表情4-闭眼
        movement.push_back(QPixmap(QString(":/images/movement/closeEyes/%1.png").arg(i)));
    faceNum.push_back(9);
    faceNum.push_back(52);
    for(int i = 1; i<=faceNum[8]; i++)//表情5-单眨眼
        movement.push_back(QPixmap(QString(":/images/movement/wink/%1.png").arg(i)));

    face = -1;//表情序号初始化为-1，不生效
    faceSum = 5;//表情数量
}

void Haro::eyesMovement()
{
    if (draggingCount > 10) {
        flyMove++;
        if (flyMove > 10) {
            eyesImage->setPixmap(flyMovement[10+flyMove%12].scaled(size,size));
        } else {
            eyesImage->setPixmap(flyMovement[flyMove].scaled(size,size));
        }
        return;
    }
    //各种静态变量，用于计数、记录状态等↓
    static int flag = 0,second1 = 0,second2 = 0,earSwitch = 1;

    int valve = qrand()%200;
    if(face<0 && spMove<0){//控制眨眼动作
        second1++;
        if(second1>=valve && valve>100){
            face = 0;
            second1 = 0;
        }
    }

    second2++;//控制耳朵的动画
    if(second2>40 && earSwitch){
        earsImage->setPixmap(ears2[earsNum].scaled(size,size));
        earSwitch = 0;
        second2 = 0;
    }
    else if(second2>10 && !earSwitch){
        earsImage->setPixmap(ears1[earsNum].scaled(size,size));
        earSwitch = 1;
        second2 = 0;
    }

    if(face!=-1){//控制表情变化
    int num = faceNum[face*2],start = faceNum[face*2+1];
        flag++;
        if(flag<num)
            eyesImage->setPixmap(
                        movement[start+flag].scaled(size,size));
        else
            eyesImage->setPixmap(
                        movement[start-flag+(num-1)*2].scaled(size,size));

        if(flag>=(num-1)*2){
            flag = 0;
            face = -1;
            eyesImage->setPixmap(eyes.scaled(size,size));
        }
    }
    if(spMove>-1)//特殊动作
        specialMovement();
}

void Haro::flyMovementLoad() {
    for(int i = 1;i<=22;i++)
        flyMovement.push_back(QPixmap(QString(":/images/movement/fly/%1.png").arg(i)));
}
void Haro::specialMovementLoad()
{
    for(int i = 1;i<=11;i++)
        spMovement.push_back(QPixmap(QString(":/images/movement/error/%1.png").arg(i)));
    for(int i = 1;i<=22;i++)
        spMovement.push_back(QPixmap(QString(":/images/movement/fly/%1.png").arg(i)));

    spMove = -1;
}

void Haro::specialMovement(){
    static int flag = 0;

    if(spMove == 0){//动作-error
        if(flag%20==0 && flag<=200)
            eyesImage->setPixmap(spMovement[flag/20].scaled(size,size));
        else if(flag>300){
            eyesImage->setPixmap(eyes.scaled(size,size));
            flag=0;
            spMove=-1;
            return ;
        }
    }

    flag++;
}


void Haro::loadConfigData() {
    QHash<QString,QString> configData;
    QHash<QString,QString> btnData;
    db->getData("config", configData);
    // TODO：统一管理初始化
    if (configData.empty()) {
        configData["scale"] = "100";
        configData["dress_head"] = "0";
        configData["dress_clothes"] = "0";
        configData["coordinate_x"] = "0";
        configData["coordinate_y"] = "0";

        db->addData("config", configData);
    }

    size = configData["scale"].toInt();
    bodyNum = configData["dress_head"].toInt();
    earsNum = configData["dress_clothes"].toInt();
    coordX = configData["coordinate_x"].toInt();
    coordY = configData["coordinate_y"].toInt();

    // 菜单按钮动态加载
    for(int i=0; i<4; i++) {
        db->getData("buttons", btnData, "where b_id == "+configData["button"+QString::number(i+1)+"_id"]);

        btns[menuBtnIdxs[i]]->setObjectName("m"+btnData["function_id"]);
        btns[menuBtnIdxs[i]]->setIcon(QIcon(btnData["icon_url"]));
        btns[menuBtnIdxs[i]]->setToolTip(btnData["name"]);
        connect(btns[menuBtnIdxs[i]], &QPushButton::clicked, this, &Haro::onBtnsClick);
    }

    return;
}

// 更新config
void Haro::updateConfigData(QString key, int value) {
    QHash<QString,QString> configData;
    configData[key] = QString::number(value);
    db->updateData("config", configData);
}

void Haro::updateDressing(QString key, int value) {
    if (key == "head") {
        bodyNum = value;
        imageSet(bodyImage,body[bodyNum]);
        this->updateConfigData("dress_head", value);
    } else if (key == "clothes") {
        earsNum = value;
        imageSet(earsImage,ears1[earsNum]);
        this->updateConfigData("dress_clothes", value);
    }
}

void Haro::updateSize(int value) {
    size = value;
    imageSet(bodyImage,body[bodyNum]);
    imageSet(eyesImage,eyes);
    if(size>140){
        imageSet(stripeImage,stripe);
        stripeImage->show();
    }
    else
        stripeImage->hide();
    imageSet(earsImage,ears1[earsNum]);
    this->updateConfigData("scale", value);
    renderBtn();
}
