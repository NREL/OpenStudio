/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_IMPL_HPP
#define MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkEffectiveLeakageArea_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkEffectiveLeakageArea.*/
    class MODEL_API AirflowNetworkEffectiveLeakageArea_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkEffectiveLeakageArea_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkEffectiveLeakageArea_Impl(const AirflowNetworkEffectiveLeakageArea_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkEffectiveLeakageArea_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double effectiveLeakageArea() const;

      double dischargeCoefficient() const;

      bool isDischargeCoefficientDefaulted() const;

      double referencePressureDifference() const;

      bool isReferencePressureDifferenceDefaulted() const;

      double airMassFlowExponent() const;

      bool isAirMassFlowExponentDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setEffectiveLeakageArea(double effectiveLeakageArea);

      bool setDischargeCoefficient(double dischargeCoefficient);

      void resetDischargeCoefficient();

      bool setReferencePressureDifference(double referencePressureDifference);

      void resetReferencePressureDifference();

      bool setAirMassFlowExponent(double airMassFlowExponent);

      void resetAirMassFlowExponent();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkEffectiveLeakageArea");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_IMPL_HPP
