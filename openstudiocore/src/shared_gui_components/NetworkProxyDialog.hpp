/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef SHAREDGUICOMPONENTS_NETWORKPROXYDIALOG_HPP
#define SHAREDGUICOMPONENTS_NETWORKPROXYDIALOG_HPP

#include <QNetworkProxy>
#include <QDialog>
#include <QSettings>
#include <QNetworkReply>

#include "../utilities/core/Logger.hpp"

class QComboBox;
class QLineEdit;

namespace openstudio
{
  class NetworkProxyDialog : public QDialog
  {
    Q_OBJECT;
    REGISTER_LOGGER("openstudio.NetworkProxyDialog");

    public:
      // Saves to / loads from the QSettings object passed in
      NetworkProxyDialog(QSettings &t_settings, QWidget *t_parent);
      virtual ~NetworkProxyDialog() {}

      QNetworkProxy createProxy();

      static QNetworkProxy createProxy(const QSettings &t_settings);
      bool testProxy(QNetworkProxy &t_proxy);
      static bool testProxy(QNetworkProxy &t_proxy, QWidget *t_parent);
      void setSettings(QSettings &t_settings) const;

    private slots:
      void saveClicked();
      void cancelClicked();
      void testClicked();
      void proxyTypeChanged(int index);


    private:
      static std::pair<QNetworkReply::NetworkError, QString> testProxyConnection(const QNetworkProxy &t_proxy, QWidget *t_parent);

      QSettings &m_settings;

      QComboBox *m_proxyType;
      QLineEdit *m_hostName;
      QLineEdit *m_port;
      QLineEdit *m_user;
  };

}


#endif // SHAREDGUICOMPONENTS_NETWORKPROXYDIALOG_HPP
