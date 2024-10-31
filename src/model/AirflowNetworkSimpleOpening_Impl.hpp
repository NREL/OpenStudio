/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSIMPLEOPENING_IMPL_HPP
#define MODEL_AIRFLOWNETWORKSIMPLEOPENING_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkSimpleOpening_Impl is a AirflowNetworkComponent_Impl that is the implementation class for AirflowNetworkSimpleOpening.*/
    class MODEL_API AirflowNetworkSimpleOpening_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkSimpleOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkSimpleOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkSimpleOpening_Impl(const AirflowNetworkSimpleOpening_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkSimpleOpening_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double airMassFlowCoefficientWhenOpeningisClosed() const;

      double airMassFlowExponentWhenOpeningisClosed() const;

      bool isAirMassFlowExponentWhenOpeningisClosedDefaulted() const;

      double minimumDensityDifferenceforTwoWayFlow() const;

      double dischargeCoefficient() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);

      bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);

      void resetAirMassFlowExponentWhenOpeningisClosed();

      bool setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow);

      bool setDischargeCoefficient(double dischargeCoefficient);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkSimpleOpening");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSIMPLEOPENING_IMPL_HPP
