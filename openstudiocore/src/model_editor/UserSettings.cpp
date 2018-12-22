/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "UserSettings.hpp"

#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"

#include <QString>
#include <QSettings>

std::vector<openstudio::BCLMeasure> localBCLMeasures()
{
  return openstudio::LocalBCL::instance().measures();
}

std::vector<openstudio::BCLMeasure> userMeasures()
  {
    openstudio::path path = userMeasuresDir();
    return openstudio::BCLMeasure::getMeasuresInDir(path);
  }

  openstudio::path userMeasuresDir()
  {
    QSettings settings("OpenStudio", "BCLMeasure");
    QString value = settings.value("userMeasuresDir", openstudio::toQString(openstudio::filesystem::home_path() / openstudio::toPath("OpenStudio/Measures"))).toString();
    openstudio::path result = openstudio::toPath(value);
    return openstudio::filesystem::system_complete(result);
  }

  bool setUserMeasuresDir(const openstudio::path& userMeasuresDir)
  {
    if (!userMeasuresDir.is_complete()){
      return false;
    }
    if (!exists(userMeasuresDir)){
      if (!openstudio::filesystem::create_directories(userMeasuresDir)) {
        return false;
      }
    }
    if (!is_directory(userMeasuresDir)){
      return false;
    }

    QSettings settings("OpenStudio", "BCLMeasure");
    settings.setValue("userMeasuresDir", openstudio::toQString(userMeasuresDir));
    return true;
  }

  void clearUserMeasuresDir()
  {
    QSettings settings("OpenStudio", "BCLMeasure");
    settings.remove("userMeasuresDir");
  }