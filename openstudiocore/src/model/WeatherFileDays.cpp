/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "WeatherFileDays.hpp"
#include "WeatherFileDays_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_SizingPeriod_WeatherFileDays_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  WeatherFileDays_Impl::WeatherFileDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SizingPeriod_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WeatherFileDays::iddObjectType());
  }

  WeatherFileDays_Impl::WeatherFileDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WeatherFileDays::iddObjectType());
  }

  WeatherFileDays_Impl::WeatherFileDays_Impl(const WeatherFileDays_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& WeatherFileDays_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WeatherFileDays_Impl::iddObjectType() const {
    return WeatherFileDays::iddObjectType();
  }

  void WeatherFileDays_Impl::ensureNoLeapDays()
  {
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_SizingPeriod_WeatherFileDaysFields::BeginMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_SizingPeriod_WeatherFileDaysFields::BeginDayofMonth);
      if (day && (day.get() == 29)){
        this->setInt(OS_SizingPeriod_WeatherFileDaysFields::BeginDayofMonth, 28);
      }
    }

    month = getInt(OS_SizingPeriod_WeatherFileDaysFields::EndMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_SizingPeriod_WeatherFileDaysFields::EndDayofMonth);
      if (day && (day.get() == 29)){
        this->setInt(OS_SizingPeriod_WeatherFileDaysFields::EndDayofMonth, 28);
      }
    }
  }

} // detail

/// constructor
WeatherFileDays::WeatherFileDays(const Model& model)
  : SizingPeriod(WeatherFileDays::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WeatherFileDays_Impl>());
}

// constructor
WeatherFileDays::WeatherFileDays(std::shared_ptr<detail::WeatherFileDays_Impl> impl)
  : SizingPeriod(std::move(impl))
{}

IddObjectType WeatherFileDays::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SizingPeriod_WeatherFileDays);
  return result;
}

} // model
} // openstudio
