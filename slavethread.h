#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

#include <QThread>
#include <QSerialPort>
#include <math.h>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

class SlaveThread : public QThread
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setportName NOTIFY portNameChanged)
    Q_PROPERTY(QByteArray response READ response WRITE setresponse NOTIFY responseChanged)
    Q_PROPERTY(QString recvMsg READ recvMsg NOTIFY recvMsgChanged)

public:
    explicit SlaveThread(QObject *parent = nullptr);
    ~SlaveThread() override;

    Q_INVOKABLE void startSlave();
    Q_INVOKABLE void closeSlave();
    Q_INVOKABLE void sendResponse();

    Q_INVOKABLE void remoteToAutonomous(QByteArray num);

    Q_INVOKABLE void transTask(QByteArray task);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleBytesWritten(qint64 bytes);

private:
    QSerialPort m_serial;
    QString m_portName;
    QByteArray m_response;
    QString m_recvMsg;
    qint64 m_bytesWritten = 0;
signals:
    // 属性信号
    void portNameChanged();
    void responseChanged(QString sendMsg);
    void recvMsgChanged(QString recvMsg);

    // 坐标转换信号
    void coordinateChanged(double shipX, double shipY, double targetX, double targetY, double pointX, double pointY, QString yaw);
public:
    // 属性函数
    QString portName();
    void setportName(const QString &portName);
    QByteArray response();
    void setresponse(const QByteArray &response);
    QString recvMsg();

#define PI 3.1415926

    QByteArray getCurrentTime();// 获取当前时间

    //转换驱动板参数
    QByteArray m_storeNow = "";
    QByteArray m_storeNext = "";

    // 遥控数据处理
    void handleParseRecvDate(QByteArray recvMsg);
    QByteArray handleRecvFlow(QByteArray recvMsg);
    QByteArray handleExtractRecvDate(QByteArray Terminate);
    QByteArray sendToDriver(QByteArray Terminate);
    QByteArray organizeSendToDriver(QByteArray Terminate);

    // 接收下位机数据
    void bottomParseRecvDate(QByteArray recvMsg);
    QByteArray bottomRecvFlow(QByteArray recvMsg);
    QByteArray bottomExtractRecvDate(QByteArray Terminate);

    // 提取经纬度，转化为 大地坐标系
    QByteArray coordinateTrans(QByteArray recvMsg);

    //经纬度转换后的大地坐标值
    double shipxD;
    double shipyD;
    double yawD;
    double targetXD;
    double targetYD;
    double pointXD;
    double pointYD;

    // 遥控OR自主 切换 信号
    QByteArray rORa = "";

    // 切换任务模式
    QByteArray strTask = "";

    // 字符串转16进制
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
};

#endif // SLAVETHREAD_H
