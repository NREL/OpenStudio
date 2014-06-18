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

#ifndef MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** RefrigerationCondenserCascade_Impl is a ModelObject_Impl that is the implementation class for RefrigerationCondenserCascade.*/
  class MODEL_API RefrigerationCondenserCascade_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCondenserCascade_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    RefrigerationCondenserCascade_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    RefrigerationCondenserCascade_Impl(const RefrigerationCondenserCascade_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~RefrigerationCondenserCascade_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double ratedCondensingTemperature() const;

    double ratedApproachTemperatureDifference() const;

    bool isRatedApproachTemperatureDifferenceDefaulted() const;

    double ratedEffectiveTotalHeatRejectionRate() const;

    std::string condensingTemperatureControlType() const;

    bool isCondensingTemperatureControlTypeDefaulted() const;

    boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

    boost::optional<double> condensateReceiverRefrigerantInventory() const;

    boost::optional<double> condensatePipingRefrigerantInventory() const;

    //@}
    /** @name Setters */
    //@{

    void setRatedCondensingTemperature(double ratedCondensingTemperature);

    bool setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference);

    void resetRatedApproachTemperatureDifference();

    bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

    bool setCondensingTemperatureControlType(std::string condensingTemperatureControlType);

    void resetCondensingTemperatureControlType();

    void setCondenserRefrigerantOperatingChargeInventory(boost::optional<double> condenserRefrigerantOperatingChargeInventory);

    void resetCondenserRefrigerantOperatingChargeInventory();

    void setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory);

    void resetCondensateReceiverRefrigerantInventory();

    void setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory);

    void resetCondensatePipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserCascade");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERCASCADE_IMPL_HPP

