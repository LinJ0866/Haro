#include "setwin.h"
#include "ui_setwin.h"

SetWin::SetWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWin)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    setStyleSheet("background-color:white;");
    Qt::WindowFlags m_flags = windowFlags();//保持窗口置顶1
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint);//保持窗口置顶2

    this->setWindowIcon(QIcon(":/assets/icons/setting.png")); //设置窗口图标

}

SetWin::~SetWin()
{
    delete ui;
}

void SetWin::setSize(int size)
{
    haroSize = size;

    QFont ft;
    ft.setPointSize(12);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    sizeNum = new QLabel(this);
    sizeNum->move(120,100);
    sizeNum->resize(120,20);
    sizeNum->setFont(ft);
    sizeNum->setPalette(pa);
    sizeNum->setNum(haroSize);
    sizeNum->show();

    ui->sizeSlider->setValue(haroSize);
}

int SetWin::getSize()
{
    return haroSize;
}

void SetWin::on_sizeSlider_valueChanged(int value)
{
    haroSize = value;
    sizeNum->setNum(haroSize);
    emit sizeSignal(haroSize);
}
