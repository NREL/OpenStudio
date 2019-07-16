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

    /// check if the application has been initialized
    bool hasApplication() const;

    /// check if the application has GUI enabled
    bool hasGUI() const;

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

    bool m_defaultInstance;

  };

  typedef openstudio::Singleton<ApplicationSingleton> Application;

#if _WIN32 || _MSC_VER
  /// Explicitly instantiate and export ApplicationSingleton Singleton template instance
  /// so that the same instance is shared between the DLL's that link to Utilities.dll
  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<ApplicationSingleton>;
#endif
} // openstudio

#endif // UTILITIES_CORE_APPLICATION_HPP
