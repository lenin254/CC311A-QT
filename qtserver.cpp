#include "qtserver.h"
#include "ui_qtserver.h"

QtServer::QtServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtServer)
{
    ui->setupUi(this);
    initServer();                           //inicializar el servidor
    //es IMPORTANTE inicializar el server antes de hacer as conecciones
    //nosotros programamos los slots, ya que las señales ya estan programadas
    // cierra todo de una forma mas eficiente
    connect(ui->btn_close,                  // el conect conecta dos objetos, en este caso btn_close de .UI
            &QAbstractButton::clicked,      //clicked cuando clickeamos XD
            this,                           //segundo objeto (en esta caso esta clase)
            &QWidget::close);               //como esta clase es heredad de Qwidget tmb hereda la funcionalidad de QWidget::close asi que la cierra

    connect(serverSocket,                   //el primer objeto
            &QTcpServer::newConnection,     //Señal: nueva coneccion
            this,                           // segundo objeto
            &QtServer::sendRandomMessage);  //Slot: enviamos el random mensaje
    for (int i= 1;i<=50 ;i++ )              //fill the vector
    {
        randomMessages << "Mensaje aleatorio" +QString::number(i);
    }
    //qInfo() << randomMessage;             //verificar en la consola los mensajes aleatorios
}

QtServer::~QtServer()
{
    delete ui;
}

void QtServer::sendRandomMessage() //este es el slot que va enviar el mensaje al cliente
{
    qInfo() << "sendRandomMessage()...";    //iniciamos la funcion
    //estas lineas me facilita la comunicacion
    //Stream and channel#########################################################
    QByteArray qByteArray;
    QDataStream qDataStream(&qByteArray,    //El objeto
                            QIODevice::WriteOnly //escribimos
                            );
    //random message, tomando un mensanje aletorio de la lista########################
    //bounded es acotar (en este caso lo acotamos con el tamaño de la lista messages
    QString randomMessage = randomMessages[QRandomGenerator::global()->bounded(randomMessages.size())];
    qDataStream <<randomMessage; //el mensaje lo escribimos en el canal
    //Write to label#############################################
    ui->lbl_message->setText(randomMessage);
    //write to stream #################################################
    qDataStream << randomMessage;
    //send it (enviamos el mensaje) ######################################
    //a travez del socket, el mismo server socket genera un client socket
    QTcpSocket *clientSocket = serverSocket->nextPendingConnection();
    //le delega al client socket y el tiene que responder y gestionar la comunicacion
    //ahora se debe asociar, solo se ha creado, con el conocido connet, tambien debemos cerrar la comunicacion
    //porque si no va haber memoria en el aire
    connect(clientSocket,
            &QAbstractSocket::disconnected, //disconnected (esta desconectado)
            clientSocket,
            &QObject::deleteLater);         //libera la memoria
    //ahora enviamos
    clientSocket->write(qByteArray);        //(ESCRIBIMOS EN EL STRING) no en el canal(Bytes o BITS)
    clientSocket->disconnectFromHost();     //lo desconectamos del host / disconect (Lo desconecto)
}

void QtServer::initServer()
{
    qDebug() << "initserver()";             //iniciamos la funcion
    serverSocket = new QTcpServer(this);    //inicializando el socket
    if (!serverSocket->listen())            //Si el server socket se pone modo listen nos da verdadero, en este caso entramos cuando es falso
    {
        qDebug() << "Server error:"+        //mostramos una señal que el servidor no esta en modo listening
                    serverSocket->errorString();
        return;                             //retornamos con del error
    }
    QString port = QString::number(serverSocket->serverPort()); // guardamos el numero de puerto del servidor en un string port
    QString ipAdress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses(); //escribimos todas la direcciones de la maquina, en esta lista
    //qInfo() <<ipAddressList;                                             //mostramos las IPs y detectamos la nuestra
    for (int  i = 0; i< ipAddressList.size() ;i++ ) {                      //Recorremos la lista de IPs
        if (ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()) // verificamos que sea distina al localHost y que sea IpV4
        {
            ipAdress = ipAddressList.at(i).toString();          //Guarda en ipAdress la variables distinta al localHost y las ipV4
            break;                                              //para no tomar mas Ips y solo tomar la primera que cumpla
        }
    }
    QString info = "IP: " +ipAdress+" port:"+port; //imprimimos la IP y el puerto en la consola para comprobar
    qInfo() << info;
    ui->lbl_IP_Port->setText(info);                 //Agregamos el string en la interfase grafica
}


//void QtServer::on_btn_close_clicked()
//{
//    qDebug() << "close";

//}


