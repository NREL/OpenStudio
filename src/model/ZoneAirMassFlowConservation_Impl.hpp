/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRMASSFLOWCONSERVATION_IMPL_HPP
#define MODEL_ZONEAIRMASSFLOWCONSERVATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ZoneAirMassFlowConservation_Impl is a ModelObject_Impl that is the implementation class for ZoneAirMassFlowConservation.*/
    class MODEL_API ZoneAirMassFlowConservation_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneAirMassFlowConservation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneAirMassFlowConservation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneAirMassFlowConservation_Impl(const ZoneAirMassFlowConservation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneAirMassFlowConservation_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string adjustZoneMixingandReturnForAirMassFlowBalance() const;

      bool isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted() const;

      std::string infiltrationBalancingMethod() const;

      bool isInfiltrationBalancingMethodDefaulted() const;

      std::string infiltrationBalancingZones() const;

      bool isInfiltrationBalancingZonesDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAdjustZoneMixingandReturnForAirMassFlowBalance(const std::string& adjustZoneMixingandReturnForAirMassFlowBalance);

      void resetAdjustZoneMixingandReturnForAirMassFlowBalance();

      bool setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod);

      void resetInfiltrationBalancingMethod();

      bool setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones);

      void resetInfiltrationBalancingZones();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneAirMassFlowConservation");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRMASSFLOWCONSERVATION_IMPL_HPP
