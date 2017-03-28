#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QDir>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getUrl(QString url);
    void findPointId(QByteArray);
    void findPointId(QJsonArray);
    void findPointId(QJsonObject);
    int addPointId(int );
    void setmeasId(QByteArray);
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * networkManager;
    QNetworkReply * networkReply;
    QString queryString;
    QNetworkRequest networkRequest;
    quint64 totalData;
    bool pointIdDownloaded; //poin_id下载完成后为true
    bool measIdDownloadButton;//点击下载measIdbutton
    bool measIdDownloaded;//measId下载完成
    bool waveDownloadButton;//点击下载wave按钮
    QFile *file;
public slots:
    void finished();
    void httpError(QNetworkReply::NetworkError);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void metaDataChanged();

private slots:
    void on_pointIdCheck_clicked();
    void on_dateCheck_clicked();
    void on_measIdCheck_clicked();
};

#endif // MAINWINDOW_H
