#ifndef QTSERVER_H
#define QTSERVER_H

#include <QWidget>
#include <QDebug>
#include <QTcpServer>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class QtServer; }
QT_END_NAMESPACE

class QtServer : public QWidget //server se hereda de QWidget
{
    Q_OBJECT

public:
    QtServer(QWidget *parent = nullptr);
    ~QtServer();

private slots:
    void sendRandomMessage(); //para enviar el slot desde el elservidor para el cliente
    //void on_btn_close_clicked();

private:
    Ui::QtServer *ui;
    QTcpServer *serverSocket = nullptr; //Esta variable va a manejar la funcionalidad
    QVector<QString> randomMessages; //vector para enviar el mensaje

    void initServer(); //Para inicar el servidor

};
#endif // QTSERVER_H
