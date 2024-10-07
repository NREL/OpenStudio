/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKLEAKAGERATIO_IMPL_HPP
#define MODEL_AIRFLOWNETWORKLEAKAGERATIO_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkLeakageRatio_Impl is a AirflowNetworkComponent_Impl that is the implementation class for AirflowNetworkLeakageRatio.*/
    class MODEL_API AirflowNetworkLeakageRatio_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkLeakageRatio_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkLeakageRatio_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkLeakageRatio_Impl(const AirflowNetworkLeakageRatio_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkLeakageRatio_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> effectiveLeakageRatio() const;

      double maximumFlowRate() const;

      double referencePressureDifference() const;

      double airMassFlowExponent() const;

      bool isAirMassFlowExponentDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setEffectiveLeakageRatio(double effectiveLeakageRatio);

      void resetEffectiveLeakageRatio();

      bool setMaximumFlowRate(double maximumFlowRate);

      bool setReferencePressureDifference(double referencePressureDifference);

      bool setAirMassFlowExponent(double airMassFlowExponent);

      void resetAirMassFlowExponent();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkLeakageRatio");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKLEAKAGERATIO_IMPL_HPP
