#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

// add necessary includes here
#include <qmqtt.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_Connect_clicked();

    void on_pushButton_Subscribe_clicked();

    void on_pushButton_Publish_clicked();


    void slotConnectedMQTT();

    void slotDisconnectedMQTT();

    void slotSubscribedMQTT(QString sTopic, quint8 qos = 0);

    void slotReceivedMessageByMQTT(QMQTT::Message sMsg);

private:
    Ui::Widget *ui;

    // the mqtt client pointer
    QMQTT::Client *m_pMQTT = nullptr;
};
#endif // WIDGET_H
