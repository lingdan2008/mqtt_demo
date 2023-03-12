#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("MQTT Demo"));

    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Publish->setEnabled(false);
    ui->pushButton_Subscribe->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_Connect_clicked()
{
    if(tr("Connect") == ui->pushButton_Connect->text().trimmed()) {
        ui->pushButton_Publish->setEnabled(true);
        ui->pushButton_Subscribe->setEnabled(true);

        ui->pushButton_Connect->setText(tr("Disconnect"));

        QString sHost = ui->lineEdit_Host->text().trimmed();
        int nPort = ui->lineEdit_Port->text().trimmed().toInt();

        // Create a QMQTT client
        if(nullptr == m_pMQTT) {
            m_pMQTT = new QMQTT::Client(QHostAddress(sHost), nPort);
            Q_ASSERT(m_pMQTT);
        }

        // Signal-slot initialization
        std::vector<bool> vecConnect;

        vecConnect.push_back(QObject::connect(m_pMQTT, SIGNAL(connected()), this, SLOT(slotConnectedMQTT())));
        vecConnect.push_back(QObject::connect(m_pMQTT, SIGNAL(disconnected()), this, SLOT(slotDisconnectedMQTT())));
        vecConnect.push_back(QObject::connect(m_pMQTT, SIGNAL(subscribed(QString, quint8)), this, SLOT(slotSubscribedMQTT(QString, quint8))));
        vecConnect.push_back(QObject::connect(m_pMQTT, SIGNAL(received(QMQTT::Message)), this, SLOT(slotReceivedMessageByMQTT(QMQTT::Message))));

        for(int i=0; i<(int)vecConnect.size(); i++) {
            Q_ASSERT(vecConnect.at(i));
        }

        // Connect to host
        m_pMQTT->setCleanSession(true);
        m_pMQTT->connectToHost();
    }
    else {
        ui->pushButton_Publish->setEnabled(false);
        ui->pushButton_Subscribe->setEnabled(false);

        ui->pushButton_Connect->setText(tr("Connect"));
        m_pMQTT->disconnectFromHost();

        delete m_pMQTT;
        m_pMQTT = nullptr;
    }



}

void Widget::on_pushButton_Subscribe_clicked()
{
    // Subscribe a topic
    QString sTopic = ui->lineEdit_SubscribeTopic->text().trimmed();
    m_pMQTT->subscribe(sTopic, 1);
}

void Widget::on_pushButton_Publish_clicked()
{
    // Publish a topic
    QMQTT::Message sMsg;
    QString sTopic = ui->lineEdit_PublishTopic->text().trimmed();
    QString sContent = ui->lineEdit_PublishContent->text().trimmed();

    sMsg.setTopic(sTopic);
    sMsg.setPayload(sContent.toLocal8Bit());

    m_pMQTT->publish(sMsg);
}

void Widget::slotConnectedMQTT()
{
    ui->textBrowser_ReceivedMessage->append(tr("Connection succeeded"));
}

void Widget::slotDisconnectedMQTT()
{
    ui->textBrowser_ReceivedMessage->append(tr("Connection disconnected"));
}

void Widget::slotSubscribedMQTT(QString sTopic,quint8 qos)
{
    Q_UNUSED(sTopic);
    Q_UNUSED(qos);

    QString sMsg = "Subscribe Topic ";
    sMsg += ui->lineEdit_SubscribeTopic->text();
    sMsg += " Succeeded";
    ui->textBrowser_ReceivedMessage->append(sMsg);
}

void Widget::slotReceivedMessageByMQTT(QMQTT::Message sMsg)
{
    QString sPayload = sMsg.payload();
    QString sMsgTotal = "Topic:";

    sMsgTotal += sMsg.topic();
    sMsgTotal += "    Payload:";
    sMsgTotal += sPayload;

    ui->textBrowser_ReceivedMessage->append(sMsgTotal);
}
