#ifndef OPENSTUDIO_SHAREDGUICOMPONENTS_NETWORKPROXYDIALOG
#define OPENSTUDIO_SHAREDGUICOMPONENTS_NETWORKPROXYDIALOG

#include <QNetworkProxy>
#include <QDialog>
#include <QSettings>

class QComboBox;
class QLineEdit;

namespace openstudio
{
  class NetworkProxyDialog : public QDialog
  {
    Q_OBJECT;

    public:
      // Saves to / loads from the QSettings object passed in
      NetworkProxyDialog(QSettings &t_settings, QWidget *t_parent);

      QNetworkProxy createProxy();

      static QNetworkProxy createProxy(const QSettings &t_settings);
      bool testProxy(QNetworkProxy &t_proxy);
      static bool testProxy(QNetworkProxy &t_proxy, QWidget *t_parent);
      void setSettings(QSettings &t_settings) const;

    private slots:
      void saveClicked();
      void cancelClicked();
      void testClicked();

    private:
      static bool testProxyConnection(const QNetworkProxy &t_proxy);

      QSettings &m_settings;

      QComboBox *m_proxyType;
      QLineEdit *m_hostName;
      QLineEdit *m_port;
      QLineEdit *m_user;
  };

}


#endif
