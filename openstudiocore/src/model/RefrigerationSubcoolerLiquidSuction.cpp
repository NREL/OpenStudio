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

#include "RefrigerationSubcoolerLiquidSuction.hpp"
#include "RefrigerationSubcoolerLiquidSuction_Impl.hpp"

#include <utilities/idd/OS_Refrigeration_Subcooler_LiquidSuction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationSubcoolerLiquidSuction::iddObjectType());
  }

  RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationSubcoolerLiquidSuction::iddObjectType());
  }

  RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const RefrigerationSubcoolerLiquidSuction_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationSubcoolerLiquidSuction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationSubcoolerLiquidSuction_Impl::iddObjectType() const {
    return RefrigerationSubcoolerLiquidSuction::iddObjectType();
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::liquidSuctionDesignSubcoolingTemperatureDifference() const {
    return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference,true);
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::designLiquidInletTemperature() const {
    return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature,true);
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::designVaporInletTemperature() const {
    return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature,true);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::setLiquidSuctionDesignSubcoolingTemperatureDifference(boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference) {
    bool result(false);
    if (liquidSuctionDesignSubcoolingTemperatureDifference) {
      result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference, liquidSuctionDesignSubcoolingTemperatureDifference.get());
    }
    else {
      resetLiquidSuctionDesignSubcoolingTemperatureDifference();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::resetLiquidSuctionDesignSubcoolingTemperatureDifference() {
    bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference, "");
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::setDesignLiquidInletTemperature(boost::optional<double> designLiquidInletTemperature) {
    bool result(false);
    if (designLiquidInletTemperature) {
      result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature, designLiquidInletTemperature.get());
    }
    else {
      resetDesignLiquidInletTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::resetDesignLiquidInletTemperature() {
    bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::setDesignVaporInletTemperature(boost::optional<double> designVaporInletTemperature) {
    bool result(false);
    if (designVaporInletTemperature) {
      result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature, designVaporInletTemperature.get());
    }
    else {
      resetDesignVaporInletTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerLiquidSuction_Impl::resetDesignVaporInletTemperature() {
    bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationSubcoolerLiquidSuction::RefrigerationSubcoolerLiquidSuction(const Model& model)
  : ModelObject(RefrigerationSubcoolerLiquidSuction::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>());

  // Brian's values
  setLiquidSuctionDesignSubcoolingTemperatureDifference(6.0);

  // Brian's values
  setDesignLiquidInletTemperature(16.0);

  // Brian's values
  setDesignVaporInletTemperature(0.0);
}

IddObjectType RefrigerationSubcoolerLiquidSuction::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction);
}

boost::optional<double> RefrigerationSubcoolerLiquidSuction::liquidSuctionDesignSubcoolingTemperatureDifference() const {
  return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->liquidSuctionDesignSubcoolingTemperatureDifference();
}

boost::optional<double> RefrigerationSubcoolerLiquidSuction::designLiquidInletTemperature() const {
  return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->designLiquidInletTemperature();
}

boost::optional<double> RefrigerationSubcoolerLiquidSuction::designVaporInletTemperature() const {
  return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->designVaporInletTemperature();
}

void RefrigerationSubcoolerLiquidSuction::setLiquidSuctionDesignSubcoolingTemperatureDifference(double liquidSuctionDesignSubcoolingTemperatureDifference) {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setLiquidSuctionDesignSubcoolingTemperatureDifference(liquidSuctionDesignSubcoolingTemperatureDifference);
}

void RefrigerationSubcoolerLiquidSuction::resetLiquidSuctionDesignSubcoolingTemperatureDifference() {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetLiquidSuctionDesignSubcoolingTemperatureDifference();
}

void RefrigerationSubcoolerLiquidSuction::setDesignLiquidInletTemperature(double designLiquidInletTemperature) {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setDesignLiquidInletTemperature(designLiquidInletTemperature);
}

void RefrigerationSubcoolerLiquidSuction::resetDesignLiquidInletTemperature() {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetDesignLiquidInletTemperature();
}

void RefrigerationSubcoolerLiquidSuction::setDesignVaporInletTemperature(double designVaporInletTemperature) {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setDesignVaporInletTemperature(designVaporInletTemperature);
}

void RefrigerationSubcoolerLiquidSuction::resetDesignVaporInletTemperature() {
  getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetDesignVaporInletTemperature();
}

/// @cond
RefrigerationSubcoolerLiquidSuction::RefrigerationSubcoolerLiquidSuction(std::shared_ptr<detail::RefrigerationSubcoolerLiquidSuction_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

