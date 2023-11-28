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

    db = new Db;
    dressWindow = new DressWin();
    setWindow = new SetWin();
    systray = new systemtray();
    calendar = new Calendar;

    bodyImage = new QLabel(this);//身体图片指针
    eyesImage = new QLabel(this);//眼部图片指针
    stripeImage = new QLabel(this);//屏幕遮盖条纹图片指针
    earsImage = new QLabel(this);//耳朵图片指针

    timer = new QTimer;

    // 读取数据库
    db->connect();

    initBtn(); //初始化按钮
    loadConfigData();


    HaroUiUpdate(bodyImage, Body::getBody((Body::BodyName)bodyDressIdx), true);
    HaroUiUpdate(earsImage, Ear::getEar((Ear::EarName)earsDressIdx), true);
    HaroUiUpdate(eyesImage, Eye::getEye(Eye::Eye), true);
    HaroUiUpdate(stripeImage, Stripe::getStripe(Stripe::Stripe), true);

    if (windowSize > strideThreshold) {
        stripeImage->setVisible(false);
    } else {
        stripeImage->setVisible(true);
    }
    renderBtn();
    this->move(coordX, coordY);

    bindTimerSlots();
    bindSlots();
    timer->start(40);//动画速度
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


void Haro::bindTimerSlots()
{
    connect(this->timer, &QTimer::timeout, this, &Haro::earsMovement);
    connect(this->timer, &QTimer::timeout, this, &Haro::eyesMovement);
}

void Haro::bindSlots()
{
    connect(systray, SIGNAL(exitSignal()), this, SLOT(exit()));
    connect(systray, SIGNAL(restorePositionSignal()), this, SLOT(restorePosition()));
    connect(systray, SIGNAL(displaySignal()), this, SLOT(restoreWindows()));
    connect(dressWindow, SIGNAL(dressSignal(QString, int)), this, SLOT(updateDressing(QString, int)));
    connect(setWindow, SIGNAL(sizeSignal(int)), this, SLOT(updateSize(int)));
}

void Haro::earsMovement()
{
    if (earMoveIdx++ > EAR_SWITCH_MAX_COUNT) {
        if (earMoveIdx < EAR_SWITCH_MAX_COUNT+10)
        {
            HaroUiUpdate(earsImage, Ear::getEar2((enum Ear::EarName2)earsDressIdx));
        }
        else
        {
            HaroUiUpdate(earsImage, Ear::getEar((enum Ear::EarName)earsDressIdx));
            earMoveIdx = 0;
        }
    }
}

void Haro::eyesMovement()
{
    if (eyeMoveKind >= 0) {
        eyeMoveMax = Movement::getMovementNum((Movement::MovementKind)eyeMoveKind);
        eyeMoveIdx ++;

        // 拖动时起飞逻辑
        if (eyeMoveKind == Movement::MovementKind::Fly) {
            eyeMoveMax += 2;
            if(eyeMoveIdx < 3) {
                HaroUiUpdate(eyesImage, QString(":/assets/movement/%1/%2.png"
                                                ).arg(Movement::getMovementName(Movement::MovementKind::Blink)).arg(this->eyeMoveIdx*4));
            } else {
                HaroUiUpdate(eyesImage, QString(":/assets/movement/%1/%2.png"
                                                ).arg(Movement::getMovementName((Movement::MovementKind)eyeMoveKind)).arg(this->eyeMoveIdx-2));
            }
            if (eyeMoveIdx == 36) {
                eyeMoveIdx = 11;
            } else if (eyeMoveIdx == eyeMoveMax) {
                eyeMoveIdx = 8;
                eyeMoveKind = Movement::MovementKind::Blink;
            }
            return;
        }
        HaroUiUpdate(eyesImage, QString(":/assets/movement/%1/%2.png"
                                        ).arg(Movement::getMovementName((Movement::MovementKind)eyeMoveKind)).arg(this->eyeMoveIdx));
        if (eyeMoveIdx == eyeMoveMax) {
            eyeMoveIdx = 0;
            eyeMoveKind = -1;
        }
    } else if(eyeMoveKind == -2) {
        // TODO：专注模式
    } else {
        if ((rand() % 100) == 1) {
            eyeMoveKind = Movement::Blink;
        } else {
            HaroUiUpdate(eyesImage, Eye::getEye(Eye::Eye));
        }
    }
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
    int btnSize = windowSize/3;
    int btnX = this->frameGeometry().width()/2-btnSize/2;
    int btnY = this->frameGeometry().height()/2+windowSize/2;
    int btnX_relative[5] = {0, int(-btnSize*1.2), int(+btnSize*1.2), int(-btnSize*2.2), int(+btnSize*2.2)};
    int btnY_relative[5] = {0, int(-btnSize*0.2), int(-btnSize*0.2), int(-btnSize*0.8), int(-btnSize*0.8)};
    QSize iconsize(btnSize/2,btnSize/2);

    // 关闭按钮
    btns[0]->setStyleSheet(stableStyleClose.arg(btnSize/10).arg(btnSize/2));
    btns[0]->setGeometry(this->frameGeometry().width()/2+windowSize/2,this->frameGeometry().height()/2-windowSize/1.7,btnSize,btnSize);
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

void Haro::HaroUiUpdate(QLabel *image, QString imageUrl, bool isInit)
{
    if (isInit) {
        image->setPixmap(QPixmap(imageUrl).scaled(windowSize, windowSize));
        image->setScaledContents(true);
        image->resize(windowSize,windowSize);
    } else {
        image->setPixmap(QPixmap(imageUrl).scaled(image->size()));
    }
    image->move(this->frameGeometry().width()/2 - windowSize/2,
                this->frameGeometry().height()/2 - windowSize/2);
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
    } else if (btn_name == "m202") {
        this->TimeBoardBtnPush();
    } else {
        systray->sendNotification(btn->toolTip(), "功能开发中");
    }
}

// 关闭
void Haro::exit()
{
    QApplication::exit(0);
}

void Haro::closeBtnPush()
{
    exit();
}

// 最小化
void Haro::minBtnPush()
{
    this->hide();
    systray->sendNotification("Haro", "已最小化在托盘显示，双击任务栏图标恢复显示");

    this->hideMenuBtns();
}


void Haro::restorePosition() {
    move(0, 0);
    updateConfigData("coordinate_x", 0);
    updateConfigData("coordinate_y", 0);

    systray->sendNotification("Haro", "已恢复至默认位置");

    this->restoreWindows();
}

void Haro::dressBtnPush()
{
    this->hideMenuBtns();
    dressWindow->accept(bodyDressIdx,earsDressIdx);
    dressWindow->show();
}

void Haro::setBtnPush()
{
    this->hideMenuBtns();
    setWindow->setSize(windowSize);//为设置窗口传入size参数
    setWindow->show();
}

void Haro::TimeBoardBtnPush() {
    this->hideMenuBtns();
    calendar->updateTime();
    calendar->render();
    calendar->show();
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
            if (draggingCount == 11) {
                eyeMoveIdx = 0;
                eyeMoveKind = Movement::MovementKind::Fly;
                // 拖动时收起按钮
                this->hideMenuBtns();
            }
            this->move(event->globalPos()-moveLeftTop);
        }
     }
}

void Haro::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().y() > this->frameGeometry().height()/2 - windowSize/2 &&
            event->pos().y() < this->frameGeometry().height()/2 + windowSize/2 &&
            event->pos().x() > this->frameGeometry().width()/2 - windowSize/2 &&
            event->pos().x() < this->frameGeometry().width()/2 + windowSize/2) {
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
        eyeMoveIdx = 38;
        return;
    }
    draggingCount = 0;

    //鼠标左键事件,切换表情
    if(event->button() == Qt::LeftButton) {
        if (eyeMoveKind < 0){//随机播放表情
            eyeMoveKind = qrand()%(Movement::movementNum);
            eyeErrorIdx++;
            if(eyeErrorIdx==10){//触发蓝屏
                eyeErrorIdx = 0;
                eyeMoveKind = 11;
            }
        }
    } else if (event->button() == Qt::RightButton){ //鼠标右键事件
        this->hideMenuBtns(1);
    }
}


void Haro::loadConfigData() {
    QHash<QString,QString> configData;
    QHash<QString,QString> btnData;
    db->getData("config", configData);
    // TODO：统一管理初始化
    if (configData.empty()) {
        configData["window_size"] = "100";
        configData["dress_body"] = "0";
        configData["dress_ears"] = "0";
        configData["coordinate_x"] = "0";
        configData["coordinate_y"] = "0";

        db->addData("config", configData);
    }

    windowSize = configData["window_size"].toInt();
    bodyDressIdx = configData["dress_body"].toInt();
    earsDressIdx = configData["dress_ears"].toInt();
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
    if (key == "body") {
        bodyDressIdx = value;
        HaroUiUpdate(bodyImage, Body::getBody((Body::BodyName)bodyDressIdx), true);
        this->updateConfigData("dress_body", value);
    } else if (key == "ears") {
        earsDressIdx = value;
        HaroUiUpdate(earsImage, Ear::getEar((Ear::EarName)earsDressIdx), true);
        this->updateConfigData("dress_ears", value);
    }
}

void Haro::updateSize(int value) {
    windowSize = value;
    HaroUiUpdate(bodyImage, Body::getBody((Body::BodyName)bodyDressIdx), true);
    HaroUiUpdate(earsImage, Ear::getEar((Ear::EarName)earsDressIdx), true);
    HaroUiUpdate(eyesImage, Eye::getEye(Eye::Eye), true);
    HaroUiUpdate(stripeImage, Stripe::getStripe(Stripe::Stripe), true);
    if (windowSize > strideThreshold) {
        stripeImage->setVisible(false);
    } else {
        stripeImage->setVisible(true);
    }

    updateConfigData("window_size", value);

    renderBtn();
}
