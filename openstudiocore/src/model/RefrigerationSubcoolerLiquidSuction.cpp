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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

