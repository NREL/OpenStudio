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

#ifndef UTILITIES_CORE_APPLICATION_HPP
#define UTILITIES_CORE_APPLICATION_HPP

#include "../UtilitiesAPI.hpp"
#include "Singleton.hpp"

#include <QApplication>

#include <boost/optional.hpp>

namespace openstudio{

  /** Singleton application wide configuration management.
  */
  class UTILITIES_API ApplicationSingleton
  {

    friend class Singleton<ApplicationSingleton>;

    public:

    /// get the QApplication, if no QApplication has been set this will create a default one
    QCoreApplication* application(bool gui=true);

    /// set the QApplication, this should be done before calling application(), 
    /// no op if it has already been set.  Returns true if set succeeded.
    bool setApplication(QCoreApplication *qApplication);

    /// get the QWidget wrapper around SketchUp window
    /// initialized by call to application, only implemented for windows
    QWidget* sketchUpWidget();

    /// Process pending Qt events
    /// returns true if some work was done
    bool processEvents();
    bool processEvents(int maxTime);

    /// Check if application has given setting
    bool hasSetting(const std::string& key);

    /// Remove setting
    void removeSetting(const std::string& key);
    
    /// Check if the application is headless
    bool isDefaultInstance();

    /// Get the value of setting as given type, be careful when using getSettingValueAsBool
    /// you must first check if the optional is set and then check its value
    boost::optional<bool> getSettingValueAsBool(const std::string& key);
    boost::optional<int> getSettingValueAsInt(const std::string& key);
    boost::optional<double> getSettingValueAsDouble(const std::string& key);
    boost::optional<std::string> getSettingValueAsString(const std::string& key);

    /// Set application value to given value
    void setSettingValue(const std::string& key, bool value);
    void setSettingValue(const std::string& key, int value);
    void setSettingValue(const std::string& key, double value);
    void setSettingValue(const std::string& key, const std::string& value);

    ~ApplicationSingleton();

    private:

    /// private constructor
    ApplicationSingleton();

    /// QApplication handle
    QCoreApplication* m_qApplication;

    /// QWidget wrapper around SketchUp window
    QWidget* m_sketchUpWidget;

    bool defaultInstance;

  };

  typedef openstudio::Singleton<ApplicationSingleton> Application;

#if _WIN32 || _MSC_VER
  /// Explicitly instantiate and export ApplicationSingleton Singleton template instance
  /// so that the same instance is shared between the DLL's that link to Utilities.dll
  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<ApplicationSingleton>;
#endif
} // openstudio

#endif // UTILITIES_CORE_APPLICATION_HPP
