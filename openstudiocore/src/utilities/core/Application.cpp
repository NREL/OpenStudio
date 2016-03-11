/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "Application.hpp"
#include "ApplicationPathHelpers.hpp"
#include "String.hpp"
#include <OpenStudio.hxx>

#include <QWidget>
#include <QSettings>

#if _WIN32 || _MSC_VER
  #include <QWinWidget>
  #include <Windows.h>
  #include <boost/regex.hpp>
#endif

#include <iostream>

namespace openstudio{

ApplicationSingleton::ApplicationSingleton()
  : m_qApplication(nullptr),
    m_sketchUpWidget(nullptr),
    m_defaultInstance(false)
{}

ApplicationSingleton::~ApplicationSingleton()
{
  if (m_sketchUpWidget){
    delete m_sketchUpWidget;
  }

  if (m_qApplication)
  {
     m_qApplication->quit();
  }
}

/// get the QApplication, if no QApplication has been set this will create a default one
QCoreApplication* ApplicationSingleton::application(bool gui)
{
  if (!m_qApplication){

    if (QCoreApplication::instance())
    {

      m_qApplication = QCoreApplication::instance();

    } else {

      QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, true);
      QCoreApplication::setAttribute(Qt::AA_MacPluginApplication, true);

      // Add this path for gem case (possibly deprecated)
      QCoreApplication::addLibraryPath(toQString(getApplicationRunDirectory() / toPath("plugins")));

      // And for any other random cases (possibly deprecated, applies to super-built Qt on Linux)
      QCoreApplication::addLibraryPath(toQString(getApplicationRunDirectory().parent_path() / toPath("share/openstudio-" + openStudioVersion() + "/qtplugins")));

      // Make the run path the backup plugin search location
      QCoreApplication::addLibraryPath(toQString(getApplicationRunDirectory()));

      // Make the ruby path the default plugin search location
#if defined(Q_OS_MAC)
      openstudio::path p = getApplicationRunDirectory().parent_path().parent_path().parent_path() / toPath("Ruby/openstudio");
      QCoreApplication::addLibraryPath(toQString(p));
#elif defined(Q_OS_WIN)
      openstudio::path p = getApplicationRunDirectory().parent_path() / toPath("Ruby/openstudio");
      QCoreApplication::addLibraryPath(toQString(p));
#endif

      static char *argv[] = {nullptr};
      static int argc = sizeof(argv) / sizeof(char*) - 1;

      // Load the qpa plugin (If SketchUp is loading the OpenStudio plugin, the SketchUp run path will be added to the end of libraryPaths)
      if (gui){
        m_qApplication = new QApplication(argc, argv);
        dynamic_cast<QApplication*>(m_qApplication)->setQuitOnLastWindowClosed(false);
      }else{
        m_qApplication = new QCoreApplication(argc, argv);
      }

      
      m_defaultInstance = true;

      // check if we are in a SketchUp process
      #if _WIN32 || _MSC_VER
        if (gui){
          DWORD pId = GetCurrentProcessId();
          //HMODULE hModule = GetModuleHandle(NULL); // same as hInstance
          LPTSTR className = new TCHAR[255];
          LPTSTR typeName = new TCHAR[255];
          HWND h = GetTopWindow(0);
          while (h)
          {
            DWORD pId2;
            GetWindowThreadProcessId(h, &pId2);

            if (pId == pId2){

              GetClassName(h, className, 255);
              GetWindowText(h, typeName, 255);

              if (boost::regex_match(toString(typeName), boost::regex(".*- SketchUp.*"))){
                m_sketchUpWidget = new QWinWidget(h);
                break;
              }
            }

            h = GetNextWindow(h, GW_HWNDNEXT);
          }

          delete[] className;
          delete[] typeName;
        }
      #endif

    }
  }

  return m_qApplication;
}

bool ApplicationSingleton::hasApplication() const
{
  return (m_qApplication != nullptr);
}

bool ApplicationSingleton::hasGUI() const
{
  if (hasApplication()){
    return dynamic_cast<QApplication*>(m_qApplication);
  }
  return false;
}

/// set the QApplication, this should be done before calling application(), no op if it has already been set
bool ApplicationSingleton::setApplication(QCoreApplication* qApplication)
{
  if (!m_qApplication){
    m_qApplication = qApplication;
    return true;
  }
  return false;
}

QWidget* ApplicationSingleton::sketchUpWidget()
{
  return m_sketchUpWidget;
}

bool ApplicationSingleton::processEvents()
{
  application()->sendPostedEvents();
  const bool hadevents = application()->hasPendingEvents();
  application()->processEvents(QEventLoop::AllEvents);
  return hadevents;
}

bool ApplicationSingleton::processEvents(int maxTime)
{
  application()->sendPostedEvents();
  const bool hadevents = application()->hasPendingEvents();
  application()->processEvents(QEventLoop::AllEvents, maxTime);
  return hadevents;
}

bool ApplicationSingleton::hasSetting(const std::string& key)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  return settings.contains(toQString(key));
}

void ApplicationSingleton::removeSetting(const std::string& key)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.remove(toQString(key));
}

bool ApplicationSingleton::isDefaultInstance()
{
  return m_defaultInstance;
}

boost::optional<bool> ApplicationSingleton::getSettingValueAsBool(const std::string& key)
{
  boost::optional<bool> result;

  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);

  if (!settings.contains(toQString(key))){
    return boost::none;
  }

  QVariant value = settings.value(toQString(key));
  if (!value.isNull()){
    if (value.canConvert<bool>()){
      result = value.value<bool>();
    }
  }

  return result;
}

boost::optional<int> ApplicationSingleton::getSettingValueAsInt(const std::string& key)
{
  boost::optional<int> result;

  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);

  if (!settings.contains(toQString(key))){
    return boost::none;
  }

  QVariant value = settings.value(toQString(key));
  if (!value.isNull()){
    if (value.canConvert<int>()){
      result = value.value<int>();
    }
  }

  return result;
}

boost::optional<double> ApplicationSingleton::getSettingValueAsDouble(const std::string& key)
{
  boost::optional<double> result;

  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);

  if (!settings.contains(toQString(key))){
    return boost::none;
  }

  QVariant value = settings.value(toQString(key));
  if (!value.isNull()){
    if (value.canConvert<double>()){
      result = value.value<double>();
    }
  }

  return result;
}

boost::optional<std::string> ApplicationSingleton::getSettingValueAsString(const std::string& key)
{
  boost::optional<std::string> result;

  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);

  if (!settings.contains(toQString(key))){
    return boost::none;
  }

  QVariant value = settings.value(toQString(key));
  if (!value.isNull()){
    if (value.canConvert<std::string>()){
      result = value.value<std::string>();
    }
  }

  return result;
}

void ApplicationSingleton::setSettingValue(const std::string& key, bool value)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue(toQString(key), QVariant::fromValue(value));
}

void ApplicationSingleton::setSettingValue(const std::string& key, int value)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue(toQString(key), QVariant::fromValue(value));
}

void ApplicationSingleton::setSettingValue(const std::string& key, double value)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue(toQString(key), QVariant::fromValue(value));
}

void ApplicationSingleton::setSettingValue(const std::string& key, const std::string& value)
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue(toQString(key), QVariant::fromValue(value));
}

} // openstudio

