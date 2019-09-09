/**************************
**
** #include <QSerialPort>
** 需要在.pro文件中加上一句:
**        QT += serialport
**
***************************/

#include "slavethread.h"

#include <QTime>
#include <QDebug>
#include <complex>
#include <math.h>

SlaveThread::SlaveThread(QObject *parent) :
    QThread(parent)
{
}

SlaveThread::~SlaveThread()
{
}

// 开始关闭函数
void SlaveThread::startSlave()
{
    m_serial.setPortName(m_portName);
    m_serial.setBaudRate(QSerialPort::Baud9600);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setParity(QSerialPort::NoParity);

    if(!m_serial.open(QIODevice::ReadWrite)){
        qDebug()<<tr("serial open failed %1").arg(m_portName);
        return;
    }
    connect(&m_serial, &QSerialPort::readyRead, this, &SlaveThread::handleReadyRead);
    connect(&m_serial, &QSerialPort::bytesWritten, this, &SlaveThread::handleBytesWritten);
    connect(&m_serial,
            static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SlaveThread::handleError);
}

void SlaveThread::closeSlave()
{
    m_serial.close();
}

// property函数
QString SlaveThread::portName()
{
    return m_portName;
}

QByteArray SlaveThread::response()
{
    return m_response;
}

QString SlaveThread::recvMsg()
{
    return m_recvMsg;
}

void SlaveThread::setportName(const QString &portName)
{
    if(portName == m_portName)
        return;
    m_portName = portName;
    emit portNameChanged();
}

// 发送窗口数据修改
void SlaveThread::setresponse(const QByteArray &response)
{
//    if(m_response == response)
//        return;
    if (m_serial.portName() == "COM3") {
        StringToHex(response,m_response);//将str字符串转换为16进制的形式
    }
    else if(m_serial.portName() == "COM4"){
        m_response = response;
    }


    emit responseChanged(m_response);
}

// 转十六进制
void SlaveThread::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char SlaveThread::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}

// 获取时间
QByteArray SlaveThread::getCurrentTime()
{
     QDateTime ct = QDateTime::currentDateTime();
     QByteArray currentData = ct.toString("hh.mm.ss").toLatin1();
     return  currentData;
}

// 读取数据
void SlaveThread::handleReadyRead()
{
    QByteArray recvMsg = m_serial.readAll();

    if(m_serial.portName() == "COM3"){
        recvMsg = recvMsg.toHex();
        recvMsg = recvMsg.toUpper();

        handleParseRecvDate(recvMsg);
    }else if (m_serial.portName() == "COM4") {
        bottomParseRecvDate(recvMsg);
    }
}

// “COM3”  遥控数据 处理
void SlaveThread::handleParseRecvDate(QByteArray recvMsg)
{
    QByteArray Terminate = handleRecvFlow(recvMsg);
    if(!Terminate.isEmpty()){
        m_recvMsg = handleExtractRecvDate(Terminate);
        emit recvMsgChanged(m_recvMsg);
    }
}

// 遥控数据流 判断头和长度
QByteArray SlaveThread::handleRecvFlow(QByteArray recvMsg)
{
    QByteArray Terminate;
    m_storeNow += recvMsg;
    int pHead = m_storeNow.indexOf("FF");
    if(m_storeNow.size() - pHead > 18){
        Terminate = m_storeNow.mid(pHead,18);
        QByteArray tmp = m_storeNow.right(m_storeNow.size() - pHead -18);
        m_storeNow.clear();
        m_storeNow = tmp;
    }
    else if (m_storeNow.size() - pHead == 18) {
        Terminate = m_storeNow;
        m_storeNow.clear();
    }

    return Terminate;
}

// 打包遥控数据 显示在串口界面  截取发送给下位机
QByteArray SlaveThread::handleExtractRecvDate(QByteArray Terminate)
{
    QByteArray sendToBottom;
    sendToBottom = sendToDriver(Terminate);

    //区分遥控or自主     截取发送给下位机
    QByteArray organizeSendToBottom;
    organizeSendToBottom = organizeSendToDriver(sendToBottom);
    QByteArray currentTime = getCurrentTime();

    return "From Handle: " + Terminate + "\r\n" + "To Bottom: " + organizeSendToBottom + "\r\n" + "Now Time: " + currentTime+ "\r\n";
}

// 转化为驱动版参数
QByteArray SlaveThread::sendToDriver(QByteArray Terminate)
{
#define pi 3.1415926;   //定义一个π
#define tan5 0.0873
#define tan20 0.0060924
#define sig20 0.349066
#define sig10 0.1745329  //10°弧度
#define sig5 0.0873
#define tan10 0.00304618
    QByteArray SendToDriver = "";

    int sum;
    char stop = Terminate.at(15);
    if(stop == '2'){
        sum = 500500;
    }
    else {
        int highhexdata1,highhexdata2,lowhexdata1,lowhexdata2;
        char lstr21 = Terminate.at(3),hstr11 = 32,hstr21 = Terminate.at(5),lstr11 = 32;
        QString lstr2,hstr1,hstr2,lstr1;
        lstr2 = QString(lstr21);
        hstr1 = QString(hstr11);
        hstr2 = QString(hstr21);
        lstr1 = QString(lstr11);
        bool ok;
        lowhexdata1 = lstr1.toInt(&ok,16);
        lowhexdata2 = lstr2.toInt(&ok,16);
        highhexdata1 = hstr1.toInt(&ok,16);
        highhexdata2 = hstr2.toInt(&ok,16);
        highhexdata2 = lowhexdata1*4096+lowhexdata2*256+highhexdata1*16+highhexdata2;
        // 算y
        int highhexdata3,highhexdata4,lowhexdata3,lowhexdata4;
        char lstr31 = 32,hstr31 = 32,hstr41 = Terminate.at(9),lstr41 = Terminate.at(7);
        QString lstr3,hstr3,hstr4,lstr4;
        lstr3 = QString(lstr31);
        hstr3 = QString(hstr31);
        hstr4 = QString(hstr41);
        lstr4 = QString(lstr41);
        lowhexdata3 = lstr3.toInt(&ok,16);
        lowhexdata4 = lstr4.toInt(&ok,16);
        highhexdata3 = hstr3.toInt(&ok,16);
        highhexdata4 = hstr4.toInt(&ok,16);
        highhexdata4 = lowhexdata3*4096+lowhexdata4*256+highhexdata3*16+highhexdata4;

        // 转换，进行推理分配
        float Xten,Yten;
        int max_PWM;
        max_PWM = 60;
        Xten=(float(highhexdata2)-32)*2*max_PWM/960-max_PWM;
        Yten=(float(highhexdata4)-32)*2*max_PWM/960-max_PWM;
        int ThrustMode;    //推力象限

        double sita;
        if(Yten>0)  //正半轴
            sita=atan(Xten/Yten);         //!!!这个正切算出来是不是弧度制要确认
        else if(Yten==0&&Xten>0) //y轴
            sita=PI/2;
        else if(Yten<0&&Xten>0)			//三象限
            //	sita=PI+atan(Xten/Yten);
            sita=0.5*PI-atan(Yten/Xten);
        else if(Yten<0&&Xten<0)  //四象限
            sita=atan(Xten/Yten)-PI;
        else if(Yten==0&&Xten<0)  //-y轴
            sita=-PI/2;
        else if(Yten<0&&Xten==0)   //在船尾
            sita=PI;
        else if(Yten==0&&Xten==0)
            sita=0;
        //////////
        if(Xten>=0&&Yten>=0)
            ThrustMode=1;
        else if(Xten>=0&&Yten<0)
            ThrustMode=2;
        else if(Xten<0&&Yten<=0)
            ThrustMode=3;
        else
            ThrustMode=4;
        //****分界线**** 判断是否处于第5678小象限
        float xy,yx; //xy=Xten/Yten
        if(Xten<Yten&&Xten>-Yten)
        {
            xy=Xten/Yten;
            if(fabs(xy)<=tan5)
                ThrustMode=5;
        }
        if(Xten>Yten&&Xten>-Yten)
        {
            yx=Yten/Xten;
            if(fabs(yx)<=tan5)
                ThrustMode=6;
        }
        if(Xten>Yten&&Xten<-Yten)
        {
            xy=Xten/Yten;
            if(fabs(xy)<=tan5)
                ThrustMode=7;
        }
        if(Xten<Yten&&Xten<-Yten)
        {
            yx=Yten/Xten;
            if(fabs(yx)<=tan5)
                ThrustMode=8;
        }

        //***分界线***
        float PowerLeft,PowerRight;
        double temp;
        int SqrtSignal;
        temp=Xten*Xten+Yten*Yten;
        SqrtSignal=int(sqrt(temp));
        switch(ThrustMode)
        {
        case 1:
            PowerLeft=500+SqrtSignal;
            PowerRight=500+SqrtSignal*cos(2*(0.5*PI-sita));
            break;
        case 2:
            PowerLeft=500+SqrtSignal*cos(2*(sita-PI/2));
            PowerRight=500+SqrtSignal;
            break;
        case 3:
            PowerLeft=500-SqrtSignal*cos(-2*(sita+0.5*PI));
            PowerRight=500-SqrtSignal;
            break;
        case 4:
            PowerLeft=500-SqrtSignal;
            PowerRight=500+SqrtSignal*cos(2*(sita+0.5*PI));
            break;
        case 5:
            PowerLeft=500+(SqrtSignal/sig5)*atan(Xten/Yten);
            PowerRight=500-(SqrtSignal/sig5)*atan(Xten/Yten);
            break;
        case 6:
            PowerLeft=500+SqrtSignal;
            PowerRight=500+SqrtSignal;
            break;
        case 7:
            PowerLeft=500+(SqrtSignal/sig5)*atan(Xten/Yten);
            PowerRight=500-(SqrtSignal/sig5)*atan(Xten/Yten);
            break;
        case 8:
            PowerLeft=500-SqrtSignal;
            PowerRight=500-SqrtSignal;
            break;
        }
        int d_PowerLeft,d_PowerRight;
        d_PowerLeft=int(PowerLeft);
        d_PowerRight=int(PowerRight);  //满足各式 强制转换

        sum=d_PowerLeft*1000+d_PowerRight;
    }
    QByteArray s = "z";
    QString string;
    string = QString::number(sum);
    QByteArray byte;
    byte = string.toUtf8();

    s += byte;

    SendToDriver = s;

    return SendToDriver;
}

// 区分遥控or自主     截取发送给下位机
QByteArray SlaveThread::organizeSendToDriver(QByteArray Terminate)
{
    if(rORa == "1"){
        // 遥控
        QByteArray currentTime = getCurrentTime();
        QByteArray organizeSendToDriver = "$TB,RCM," + Terminate + "," + currentTime + ",*";

        int len = organizeSendToDriver.size();
        QString len1;
        len1 = QString::number(len);
        QByteArray len2;
        len2 = len1.toUtf8();
        organizeSendToDriver ="$TB," +  len2 + "," + "RCM," + Terminate + "," + currentTime + ",*";


        int len4 = organizeSendToDriver.size();
        QString len5;
        len5 = QString::number(len4);
        QByteArray len6;
        len6 = len5.toUtf8();
        organizeSendToDriver ="$TB," +  len6 + "," + "RCM," + Terminate + "," + currentTime + ",*";

        return organizeSendToDriver;
    }
    else if(rORa == "2"){
        // 自主
        QByteArray currentTime = getCurrentTime();
        QByteArray organizeSendToDriver = "$TB,AUM," + strTask + Terminate + "," + currentTime + ",*";

        int len = organizeSendToDriver.size() + 5;
        QString len1;
        len1 = QString::number(len);
        QByteArray len2;
        len2 = len1.toUtf8();
        organizeSendToDriver = "$TB," + len2 + "," + "AUM," + strTask + Terminate + "," + currentTime + ",*";

        int len4 = organizeSendToDriver.size();
        QString len5;
        len5 = QString::number(len4);
        QByteArray len6;
        len6 = len5.toUtf8();
        organizeSendToDriver = "$TB," + len6 + "," + "AUM," + strTask + Terminate + "," + currentTime + ",*";

        return organizeSendToDriver;
    }
        return "请选择模式";
}

// “COM4”  下位机数据处理
void SlaveThread::bottomParseRecvDate(QByteArray recvMsg)
{
    QByteArray PacketRecvMsg = bottomRecvFlow(recvMsg);

    QByteArray CoordinateTrans = coordinateTrans(PacketRecvMsg);

    if(!PacketRecvMsg.isEmpty()){
        m_recvMsg = PacketRecvMsg;
    }
    emit recvMsgChanged(m_recvMsg);
}

// 判断头尾 数据流接受
QByteArray SlaveThread::bottomRecvFlow(QByteArray recvMsg)
{
    QByteArray Terminate = "";

    if( -1 != m_storeNow.indexOf("$BT")) //现在打的包有头了
    {
        if( -1 != recvMsg.indexOf("$BT") && -1 != recvMsg.indexOf("*") ) //含有$BT且含有*
        {
            int n = recvMsg.indexOf("$BT");
            int m = recvMsg.indexOf("*");

            if( n<m ) //*在$BT后面  本次内容即完整的一段
            {
                Terminate = recvMsg.mid( n , m-n+1 ); //打包完整，走你
                m_storeNow = ""; //因为此次传来的数据已经是完整的 所以这次打包结束 清空一下包包
            }

            else if( n>m ) //*在$BT前面 本次内容有正在打包的尾 和 下一个要打包的头
            {
                m_storeNow = m_storeNow + recvMsg.left(m+1); //加上尾OK 这次打包完整了
                m_storeNext = recvMsg.right(recvMsg.size()- n); //把$BT后面的内容放到下一个要打的包中
                Terminate = m_storeNow; //把包包放到流通盒中啦
                m_storeNow = ""; //清空一下
                m_storeNow = m_storeNext; //把下一个包的内容放到现在的包里继续等待快递给数据吧
            }
        }

        else if( -1 != recvMsg.indexOf("$BT") && -1 == recvMsg.indexOf("*") ) //仅仅含有$BT
        {
            //新的开始 把正在打的包给刷新一下
            m_storeNow = "";

            int n = recvMsg.indexOf("$BT"); //找到$BT位置
            recvMsg.remove(0,n); //删除之前错乱内容
            m_storeNow = recvMsg; //把整理好的内容放到要的包里
        }

        else if(-1 == recvMsg.indexOf("$BT") && -1 != recvMsg.indexOf("*") ) //仅仅含有*
        {
            //说明快递送来了最后的零件
            int m = recvMsg.indexOf("*");
            m_storeNow = m_storeNow + recvMsg.left(m+1); //内容补充完成
            Terminate = m_storeNow; //把包包放到流通盒里去
            m_storeNow = ""; //清空一下包包
        }

        else if(-1 == recvMsg.indexOf("$BT") && -1 == recvMsg.indexOf("*") ) //即不含有$BT也不含有*
        {
            m_storeNow = m_storeNow + recvMsg; //把这次快递过来的东西放到包包里去
        }

        recvMsg = ""; //清空一下快递口
    }

    else if( -1 == m_storeNow.indexOf("$BT") ) //现在的包包要么是空包，要么打包错误
    {
        m_storeNow = ""; //既然是空包那就再刷新一次，如果是错误包，那更要刷新一下啦

        if( -1 != recvMsg.indexOf("$BT") && -1 != recvMsg.indexOf("*") ) //含有$BT且含有*
        {
            int n = recvMsg.indexOf("$BT");
            int m = recvMsg.indexOf("*");
            if( n<m ) //*在$BT后面  本次内容即完整的一段
            {
                Terminate = recvMsg.mid( n , m-n+1 ); //打包完整，走你
            }

            else if( n>m ) //*在$BT前面 包包为空 所以只需要截取$BT后的内容就OK了
            {
                recvMsg.remove(0,n);   //--错误的,正确的应该Delete(0,n);
                m_storeNow = recvMsg; //把头后面的内容放到包包里面去
            }
        }

        else if( -1 != recvMsg.indexOf("$BT") && -1 == recvMsg.indexOf("*") ) //仅仅含有$BT
        {
            int n = recvMsg.indexOf("$BT"); //找到$BT位置
            recvMsg.remove(0,n); //删除之前错乱内容
            m_storeNow = recvMsg; //把整理好的内容放到要的包里
        }

        else if(-1 == recvMsg.indexOf("$BT") && -1 != recvMsg.indexOf("*") ) //仅仅含有*
        {
            //说明快递送来了最后的零件
            //但是正在打包的包包没有头 所以这次快递拒绝接受
        }

        else if(-1 == recvMsg.indexOf("$BT") && -1 == recvMsg.indexOf("*") ) //即不含有$BT也不含有*
        {
            //说明快递送来了最后的零件
            //但是正在打包的包包没有头 所以这次快递拒绝接受
        }
        recvMsg = "";
    }

    return Terminate;
}

// 截取经纬度 转换坐标系
QByteArray SlaveThread::coordinateTrans(QByteArray recvMsg)
{
    QString recvMsgTrans = recvMsg;

    QString lat = recvMsg.split(',')[3];
    QString log = recvMsg.split(',')[4];
    QString yaw = recvMsg.split(',')[6];
    QString targetX = recvMsg.split(',')[14];
    QString targetY = recvMsg.split(',')[15];

#define ERADIUS 6378245.0
#define PI 3.1415926
#define TORAD PI/180
    double ship_lat, ship_log;
    ship_lat = lat.toDouble();
    ship_log = -1 * log.toDouble();
    shipxD = (ship_lat - 38.811345) * TORAD * ERADIUS;
    shipyD = (ship_log - (121.38491)) * TORAD * ERADIUS * cos(ship_lat * TORAD);

    targetXD = targetX.toDouble();
    targetYD = targetY.toDouble();

    double delta_E, delta_N;
    delta_E = targetYD;
    delta_N = targetXD;

    double ship_yaw;
    ship_yaw = yaw.toDouble() * TORAD;

    pointXD = delta_E * sin(ship_yaw) + delta_N * cos(ship_yaw);
    pointYD = delta_E * cos(ship_yaw) + delta_N * sin(ship_yaw);

    emit coordinateChanged(shipxD,shipyD,targetXD,targetYD,pointXD,pointYD,yaw);
    return  recvMsg;
}

void SlaveThread::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;

    if(m_bytesWritten == m_response.size()){
        m_bytesWritten = 0;
        qDebug()<<tr("SerialData Successfully sent to port %1")
                  .arg(m_serial.portName());
    }
}

// 发送响应
void SlaveThread::sendResponse()
{
    // write response
    const QByteArray responseData = m_response;
    const qint64 bytes = m_serial.write(responseData);

    if(bytes == -1){
        qDebug()<<tr("Failed to write the data to port %1, error %2")
                  .arg(m_serial.portName())
                  .arg(m_serial.errorString());
    }else if(bytes != m_response.size()){
        qDebug()<<tr("Failed to write the all data to port %1, error %2")
                  .arg(m_serial.portName())
                  .arg(m_serial.errorString());
    }
}

// 处理错误
void SlaveThread::handleError(QSerialPort::SerialPortError serialPortError)
{
    qDebug()<<serialPortError;
}

// 遥控   自主
void SlaveThread::remoteToAutonomous(QByteArray num)
{
    if(num == "1")
        rORa = "1";
    else if (num == "2")
        rORa = "2";
    else if (num == "0")
        rORa = "";
}

// 任务切换
void SlaveThread::transTask(QByteArray task)
{
    strTask = task;
    /*
    C：绕圆
    E：八字
    S：抵近侦查
    A：避障
    R：恢复 */
}
