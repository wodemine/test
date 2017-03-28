#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumWidth(399);
    this->setMaximumHeight(402);
    this->setMinimumWidth(399);
    this->setMinimumHeight(402);
    ui->progressBar->setValue(0);
    //初始化变量
    totalData=0;
    pointIdDownloaded=false;
    measIdDownloaded=false;
    measIdDownloadButton=false;
    waveDownloadButton=false;

    ui->measIdCheck->setEnabled(false);
    ui->measIdComboBox->setEnabled(false);

    ui->dateCheck->setEnabled(false);
    ui->startDateTime->setEnabled(false);
    ui->endDateTime->setEnabled(false);
    ui->startDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->endDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->startDateTime->setMaximumDateTime(QDateTime::currentDateTime());
    ui->endDateTime->setMaximumDateTime(QDateTime::currentDateTime());
    ui->startDateTime->setDateTime(QDateTime::fromString("2016-12-25 10:19:35","yyyy-MM-dd hh:mm:ss"));
    ui->endDateTime->setDateTime(QDateTime::fromString("2016-12-25 10:19:45","yyyy-MM-dd hh:mm:ss"));

    //开始下载point_id
    networkManager=new QNetworkAccessManager(this);
    networkRequest.setUrl(QUrl("http://220.180.238.16:8080/api/Structure/GetStructure"));
    networkReply=networkManager->get(networkRequest);
    connect(networkReply,SIGNAL(finished()),this,SLOT(finished()));
    connect(networkReply,SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(downloadProgress(qint64,qint64)));

    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
               SLOT(httpError(QNetworkReply::NetworkError)));

    connect(networkReply,SIGNAL(metaDataChanged()),this,SLOT(metaDataChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getUrl(QString url)
{
    qDebug()<<url;

}

void MainWindow::findPointId(QByteArray receivedByte)
{

    //qDebug()<<receivedByte;
    ui->pointIdComboBox->clear();
    QJsonDocument jsonDocument=QJsonDocument::fromJson(receivedByte);
    QJsonObject jsonObject=jsonDocument.object();
    QStringList strList = jsonObject.keys();
    for( int i =0; i<jsonObject.size(); i++ )
    {
         QJsonValue value=jsonObject.value(strList.at(i));
         if(value.isObject())findPointId(value.toObject());
         else if(value.isArray()) findPointId(value.toArray());
         else if(((QString)strList.at(i)=="point_id"))
            addPointId(value.toInt());
    }
}

void MainWindow::findPointId(QJsonArray jsonArray)
{

    for( int i =0; i<jsonArray.size(); i++ )
    {
         QJsonValue value=jsonArray.at(i);
         if(value.isObject())findPointId(value.toObject());
         else if(value.isArray()) findPointId(value.toArray());

    }
}

void MainWindow::findPointId(QJsonObject jsonObject)
{

    QStringList strList = jsonObject.keys();
    for( int i =0; i<jsonObject.size(); i++ )
    {
         QJsonValue value=jsonObject.value(strList.at(i));
         if(value.isObject())findPointId(value.toObject());
         else if(value.isArray()) findPointId(value.toArray());
         else if(((QString)strList.at(i)=="point_id"))
          addPointId(value.toInt());

    }
}

int MainWindow::addPointId(int add)
{
    for(int i=0;i<ui->pointIdComboBox->count();i++)
    {
        QString id=ui->pointIdComboBox->itemText(i);
        if(id==QString::number(add)) return 1;
    }
    ui->pointIdComboBox->addItem(QString::number(add));
}

void MainWindow::setmeasId(QByteArray receivedByte )
{
    ui->measIdComboBox->clear();
    QJsonDocument jsonDocument=QJsonDocument::fromJson(receivedByte);
    QJsonArray jsonArray=jsonDocument.array();
    for(int i=0;i<jsonArray.size();i++){
        QJsonObject jsonObject=jsonArray.at(i).toObject();
        QString insert=QString::number(jsonObject["meas_id"].toInt());
        ui->measIdComboBox->addItem(insert);
    }
    if(ui->measIdComboBox->count()>0){
        ui->measIdComboBox->setEnabled(true);
        ui->measIdCheck->setEnabled(true);
        ui->statusBar->setText("请选择meas_id");
    }else{
        ui->measIdComboBox->setEnabled(false);
        ui->measIdCheck->setEnabled(false);
        ui->statusBar->setText("此时间段没有meas_id");
    }
}

void MainWindow::finished()
{
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    totalData=0;
    //pointId 下载完成
    if(pointIdDownloaded==false){


        if (networkReply->error() == QNetworkReply::NoError)
        {
            findPointId(networkReply->readAll());
            ui->statusBar->setText("请选择point_id");
            pointIdDownloaded=true;
            on_pointIdCheck_clicked();

        }else
        {
            qDebug() << " CODE:" << networkReply->error();
            QMessageBox::warning(this,"无法连接到网络","请打开网络再试");
            this->close();
        }
    }
    //下载measID
    if( pointIdDownloaded==true && measIdDownloadButton==true)
    {
        setmeasId(networkReply->readAll());
        measIdDownloadButton=false;
        measIdDownloaded=true;


    }
    //下载wave文件
    if(pointIdDownloaded ==true &&measIdDownloaded==true && waveDownloadButton==true){
        file->close();
        waveDownloadButton=false;
        ui->statusBar->setText("已保存到"+ui->pointIdComboBox->currentText()+"_"+ui->measIdComboBox->currentText()+".json");
        ui->dateCheck->setEnabled(true);
        ui->measIdCheck->setEnabled(true);
    }
}

void MainWindow::httpError(QNetworkReply::NetworkError replay )
{
    qDebug()<<"error"<<replay;
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
   ui->progressBar->setValue(bytesReceived);
   qDebug()<<bytesReceived;
   if(waveDownloadButton==true)
   {

       QTextStream out(file);
       out<<networkReply->readAll();
       out.flush();

   }
}

void MainWindow::metaDataChanged()
{
    totalData=networkReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    ui->progressBar->setMaximum(totalData);
    ui->progressBar->show();
    if(measIdDownloadButton==true)
        ui->statusBar->setText("正在下载measId");

}

void MainWindow::on_pointIdCheck_clicked()
{

    ui->startDateTime->setEnabled(true);
    ui->endDateTime->setEnabled(true);
    ui->dateCheck->setEnabled(true);
    ui->statusBar->setText("请选择时间");
}

void MainWindow::on_dateCheck_clicked()
{
    QString startTime;
    QString endTime;
    QString url;
    if(ui->endDateTime->dateTime()>ui->startDateTime->dateTime()){
        startTime=ui->startDateTime->date().toString("yyyy-MM-dd");
        startTime+=" "+ui->startDateTime->time().toString("hh:mm:ss");
        endTime=ui->endDateTime->date().toString("yyyy-MM-dd");;
        endTime+=" "+ui->endDateTime->time().toString("hh:mm:ss");
    //    qDebug()<<startTime;
    //    qDebug()<<endTime;
        url="http://220.180.238.16:8080/api/History/GetPointWaveData?point_id="+ui->pointIdComboBox->currentText()+
                "&start_time="+startTime+"&end_time="+endTime;

        networkRequest.setUrl(QUrl::fromUserInput(url));
        networkReply=networkManager->get(networkRequest);
        qDebug()<<url;

        connect(networkReply,SIGNAL(finished()),this,SLOT(finished()));
        connect(networkReply,SIGNAL(downloadProgress(qint64,qint64)),
                    this,SLOT(downloadProgress(qint64,qint64)));

        connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
                   SLOT(httpError(QNetworkReply::NetworkError)));

        connect(networkReply,SIGNAL(metaDataChanged()),this,SLOT(metaDataChanged()));
        measIdDownloadButton=true;
    }else{
        ui->statusBar->setText("起始时间应小于结束时间");
    }
}

void MainWindow::on_measIdCheck_clicked()
{
    if(ui->measIdComboBox->currentText()!=NULL){
        //QString fileName=QFileDialog::getSaveFileName(this,"保存波形文件",QDir::currentPath()+"\\JSON","*.txt");

        QString fileName=ui->pointIdComboBox->currentText()+"_"+ui->measIdComboBox->currentText()+".json";
        file=new QFile("JSON\\"+fileName);
        if (!file->open(QIODevice::WriteOnly|QIODevice::Text)) {
            QMessageBox::critical(this, "波形文件", "无法写入波形文件");

        }
        QString pointId=ui->pointIdComboBox->currentText();
        QString measId=ui->measIdComboBox->currentText();
        QString url="http://220.180.238.16:8080/api/General/GetWaveDataByMeasID?point_id="+pointId+"&meas_id="+measId;
        networkRequest.setUrl(QUrl::fromUserInput(url));
        networkReply=networkManager->get(networkRequest);
        qDebug()<<url;

        connect(networkReply,SIGNAL(finished()),this,SLOT(finished()));
        connect(networkReply,SIGNAL(downloadProgress(qint64,qint64)),
                    this,SLOT(downloadProgress(qint64,qint64)));

        connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
                   SLOT(httpError(QNetworkReply::NetworkError)));

        connect(networkReply,SIGNAL(metaDataChanged()),this,SLOT(metaDataChanged()));


        ui->statusBar->setText("开始下载波形");
        ui->dateCheck->setEnabled(false);
        ui->measIdCheck->setEnabled(false);
        waveDownloadButton=true;


    }

}
