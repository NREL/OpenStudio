#include "NetworkProxyDialog.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Application.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QInputDialog>
#include <QNetworkReply>
#include <QEventLoop>

#include "../utilities/time/Time.hpp"

#include <iostream>

namespace openstudio
{
  NetworkProxyDialog::NetworkProxyDialog(QSettings &t_settings, QWidget *t_parent = 0)
    : QDialog(t_parent), 
      m_settings(t_settings)
  {
    QGridLayout *layout = new QGridLayout(this);

    // NoProxy, DefaultProxy, Socks5Proxy, HttpProxy, HttpCachingProxy, FtpCachingProxy
    m_proxyType = new QComboBox();
    m_proxyType->addItem("No Proxy", QNetworkProxy::NoProxy);
    m_proxyType->addItem("Socks5 Proxy", QNetworkProxy::Socks5Proxy);
    m_proxyType->addItem("Http Proxy", QNetworkProxy::HttpProxy);

    m_hostName = new QLineEdit(this);
    m_port = new QLineEdit(this);
    m_port->setValidator(new QIntValidator(0, 65535));

    m_user = new QLineEdit(this);


    layout->addWidget(new QLabel("Proxy Type"), 0, 0);
    layout->addWidget(m_proxyType, 0, 1);
    layout->addWidget(new QLabel("Host Name"), 1, 0);
    layout->addWidget(m_hostName, 1, 1);
    layout->addWidget(new QLabel("Port"), 2, 0);
    layout->addWidget(m_port, 2, 1);
    layout->addWidget(new QLabel("User Name"), 3, 0);
    layout->addWidget(m_user, 3, 1);
    
    QHBoxLayout *buttonBox = new QHBoxLayout();

    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *testButton = new QPushButton("Test");
   
    buttonBox->addWidget(saveButton);
    buttonBox->addWidget(cancelButton);
    buttonBox->addWidget(testButton);

    layout->addLayout(buttonBox, 4,0,1,2);

    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(testButton, SIGNAL(clicked()), this, SLOT(testClicked()));

    QNetworkProxy proxy(createProxy(t_settings));

    int type = m_proxyType->findData(proxy.type());
    QString user = proxy.user();
    QString hostname = proxy.hostName();
    int port = proxy.port();

    m_proxyType->setCurrentIndex(type>-1?type:0);
    m_user->setText(user);
    m_hostName->setText(hostname);
    m_port->setText(QString::number(port));
  }

  void NetworkProxyDialog::saveClicked()
  {
    accept();
  }

  void NetworkProxyDialog::cancelClicked()
  {
    reject();
  }

  void NetworkProxyDialog::testClicked()
  {
    QSettings set;
    setSettings(set);

    QNetworkProxy proxy(createProxy(set));
    if (testProxy(proxy))
    {
      QMessageBox::information(this, "Test Succeeded", "Proxy configuration test passed.");
    } else {
      QMessageBox::critical(this, "Test Failed", "Proxy configuration test failed.");
    }
  }

  bool NetworkProxyDialog::testProxyConnection(const QNetworkProxy &t_proxy)
  {
    std::cout << "testProxyConnection" << std::endl;
    QNetworkAccessManager nam;
//    QNetworkProxy prox(t_proxy);
//    prox.setType(QNetworkProxy::NoProxy);
    nam.setProxy(t_proxy);
//    std::cout << "testProxyConnection " << toString(nam.proxy().host()) << " " << toString(nam.proxy().user()) << std::endl;
    QNetworkReply *head = nam.head(QNetworkRequest(QUrl("https://bcl.nrel.gov/")));
    std::cout << " errorString " << toString(head->errorString()) << std::endl;


    if (!head) return false;

    std::cout << " processing event loop for proxy connection" << std::endl;

    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    while ( !head->isFinished()
            && (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() < 10000)
    {
      Application::instance().processEvents();
    }

    head->deleteLater();

    if (!head->isFinished()) 
    {
      head->abort();
      return false;
    }

    std::cout << " errorString " << toString(head->errorString()) << " " << head->error() << std::endl;

    if (head->error() == QNetworkReply::NoError)
    {
      return true;
    } else {
      return false;
    }
  }

  bool NetworkProxyDialog::testProxy(QNetworkProxy &t_proxy, QWidget *t_parent)
  {

    t_parent->setEnabled(false);
    if (testProxyConnection(t_proxy)) { 
      t_parent->setEnabled(true);
      return true; 
    }

    while (!testProxyConnection(t_proxy) && t_proxy.type() != QNetworkProxy::NoProxy)
    {
      if (t_proxy.user() != "") {
        bool ok;
        QString password = QInputDialog::getText(t_parent, "Proxy Password", "Proxy connection failed, please enter network proxy password.", QLineEdit::Password, "", &ok);

        if (ok)
        {
          t_proxy.setPassword(password);
        } else {
          t_parent->setEnabled(true);
          return false;
        }
      } else {
        t_parent->setEnabled(true);
        return false;
      }
    }

    t_parent->setEnabled(true);
    return true;
  }


  bool NetworkProxyDialog::testProxy(QNetworkProxy &t_proxy)
  {
    return testProxy(t_proxy, this);
  }

  void NetworkProxyDialog::setSettings(QSettings &t_settings) const
  {
    t_settings.setValue("networkproxy_type", m_proxyType->itemData(m_proxyType->currentIndex()));
    t_settings.setValue("networkproxy_hostname", m_hostName->text());
    t_settings.setValue("networkproxy_port", m_port->text());
    t_settings.setValue("networkproxy_user", m_user->text());
  }

  QNetworkProxy NetworkProxyDialog::createProxy()
  {
    setSettings(m_settings);
    return createProxy(m_settings);
  }

  QNetworkProxy NetworkProxyDialog::createProxy(const QSettings &t_settings)
  {
    QNetworkProxy::ProxyType type = static_cast<QNetworkProxy::ProxyType>(t_settings.value("networkproxy_type", QNetworkProxy::NoProxy).toInt());
    QString hostname = t_settings.value("networkproxy_hostname").toString();
    int port = t_settings.value("networkproxy_port").toInt();
    QString user = t_settings.value("networkproxy_user").toString();
    return QNetworkProxy(type, hostname, port, user, "");
  }

}


