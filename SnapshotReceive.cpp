#include "snapshotreceive.h"
#include "mainwindow.h"
SnapshotReceive::SnapshotReceive()
{
    context=new zmq::context_t(1);
    sock=new zmq::socket_t(*context,ZMQ_SUB);
    sock->setsockopt(ZMQ_SUBSCRIBE,0,0);
//    m_userSettings = new QSettings("zy1205", "TradingMonitor", this);

//    QString ip =m_userSettings->value("ip").toString();
//    int SnapshotPort=m_userSettings->value("SnapshotPort").toInt();

    m_ip=MainWindow::instance()->ip;
    m_port=MainWindow::instance()->SnapshotPort;
    //qDebug()<<"add"<<m_ip<<":"<<m_port;
    connect(m_ip,m_port);

}
void SnapshotReceive::run()
{

while(true)
{
 QMutexLocker locker(&m_mutex);

    char buffer[1024];
    long long  tv_sec;
    long long tv_usec;
//        if(!sock->connected())
//        {
//            break;
//        }
    sock->recv(buffer,sizeof(buffer));

    MarketSnapshot *shot=reinterpret_cast<MarketSnapshot*>(buffer);

//        if(tv_sec!=shot->time.tv_sec)
//        {
        tv_sec=shot->time.tv_sec;
        tv_usec=shot->time.tv_usec;
//qDebug()<<"tv_sec"<<tv_sec<<"tv_usec"<<tv_usec;
        emit receivemessage(shot->ticker,QDateTime::fromTime_t(shot->time.tv_sec),shot->open_interest,shot->bids[0],shot->bids[1],shot->asks[0],shot->asks[1],shot->last_trade,shot->bids[2],shot->asks[2]);
//
}
//        m_mutex.lock();




//   m_mutex.unlock();

        //                   shot->Show(stdout);



}
//void SnapshotReceive::close(){


////    context->close();
////    sock->close();
////    delete context;
////    delete sock;
////    this->deleteLater();
//}
void SnapshotReceive::pause(){
    stopped=true;
//    m_mutex.lock();
}
void SnapshotReceive::resume(){
    stopped=false;
//    m_mutex.unlock();
}
void SnapshotReceive::connect(QString ip,int port)
{
//    sock=zmq::socket_t(*context,ZMQ_SUB);
//    sock->setsockopt(ZMQ_SUBSCRIBE,0,0);
    m_ip=ip;
    m_port=port;
//qDebug()<<"add"<<ip<<":"<<port;
    QString add="tcp://"+m_ip+":"+QString::number(m_port);
        //qDebug()<<"add"<<add;
    sock->connect(add.toStdString());
}
void SnapshotReceive::closereceive(){

    QString add="tcp://"+m_ip+":"+QString::number(m_port);
// qDebug()<<sock->ptr;
context->close();
sock->close();
delete context;
//delete sock;
//    sock->disconnect(add.toStdString());

}
