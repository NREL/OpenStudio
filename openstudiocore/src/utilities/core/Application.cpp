/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Application.hpp"
#include "ApplicationPathHelpers.hpp"
#include "String.hpp"

#include <QSettings>

#if _WIN32 || _MSC_VER
  #include <QWinWidget>
  #include <Windows.h>
  #include <boost/regex.hpp>
#else
  #include <dlfcn.h>
#endif


namespace openstudio{

ApplicationSingleton::ApplicationSingleton()
  : m_qApplication(nullptr),
    m_sketchUpWidget(nullptr),
    m_defaultInstance(false)
{}

ApplicationSingleton::~ApplicationSingleton()
{
  //if (m_sketchUpWidget){
  //  delete m_sketchUpWidget;
  //}

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

      // dir containing the current module, can be openstudio.so or openstudio.exe
      openstudio::path openstudioDirPath = getOpenStudioModuleDirectory();

      // Add the current module path to the backup plugin search location
      QCoreApplication::addLibraryPath(toQString(openstudioDirPath));

      // Make the ruby path the default plugin search location
//#if defined(Q_OS_MAC)
//      openstudio::path p = getApplicationRunDirectory().parent_path().parent_path().parent_path() / toPath("Ruby/openstudio");
//      QCoreApplication::addLibraryPath(toQString(p));
//#elif defined(Q_OS_WIN)
//      openstudio::path p = getApplicationRunDirectory().parent_path() / toPath("Ruby/openstudio");
//      QCoreApplication::addLibraryPath(toQString(p));
//#endif

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

