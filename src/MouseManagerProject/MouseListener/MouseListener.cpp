#include "MouseListener.h"
#include "Exception/AllocMemoryException.h"
#include <new>

MouseListener::MouseListener()
{
    try
    {
        server = new QTcpServer(this);
        connect(server, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
        portNumber = 0;
        qDebug()<< "ML ctor";
    }
    catch (std::bad_alloc& exception)
    {
        throw new AllocMemoryException("Error memory alloc MouseListener::MouseListener()");
    }
}

MouseListener::~MouseListener()
{
    if (server)
    {
        delete server;
        server = NULL;
    }
}

///
/// \brief MouseListener::StartListen   Начинает прослушивать соединения по установленному адресу
///
void MouseListener::StartListen()
{
    if (!server->isListening())
    {
        server->listen(QHostAddress::Any, portNumber);
        qDebug()<< "Started";
    }
}

///
/// \brief MouseListener::SetPortNumber  Устанавливает номер порта
/// \param port Номер порта
///
void MouseListener::SetPortNumber(int port)
{
    portNumber = port;
}

///
/// \brief MouseListener::onNewConnection  Вызывается при появлении нового подключенного клиента
///
void MouseListener::onNewConnection()
{
    qDebug()<< "server: New connection";
    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if(socket->state() == QTcpSocket::ConnectedState)
    {
        qDebug()<<socket->peerPort();
    }
}

///
/// \brief MouseListener::onReadyRead   Вызывается, когда на сокете появляются данные дл чтения
///
void MouseListener::onReadyRead()
{
    char data[128];
    qDebug()<< "server: read";
    int readerd = socket->read(data, 128);
    data[readerd] = '\0';
    qDebug() << "Client sended to me:";
    qDebug() << data;
  //  qDebug()<< socket->readAll();
    emit MessageReceived(data);
}


///
/// \brief MouseListener::onDisconnected    Вызывается, когда клиент завершает соединение
///
void MouseListener::onDisconnected()
{
    qDebug()<< "disconnct";
    disconnect(socket, SIGNAL(disconnected()));
    disconnect(socket, SIGNAL(readyRead()));
    socket->deleteLater();
}
