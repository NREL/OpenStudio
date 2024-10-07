/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKHORIZONTALOPENING_IMPL_HPP
#define MODEL_AIRFLOWNETWORKHORIZONTALOPENING_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkHorizontalOpening_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkHorizontalOpening.*/
    class MODEL_API AirflowNetworkHorizontalOpening_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkHorizontalOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkHorizontalOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkHorizontalOpening_Impl(const AirflowNetworkHorizontalOpening_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkHorizontalOpening_Impl() override = default;

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

      double slopingPlaneAngle() const;

      bool isSlopingPlaneAngleDefaulted() const;

      double dischargeCoefficient() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);

      bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);

      void resetAirMassFlowExponentWhenOpeningisClosed();

      bool setSlopingPlaneAngle(double slopingPlaneAngle);

      void resetSlopingPlaneAngle();

      bool setDischargeCoefficient(double dischargeCoefficient);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkHorizontalOpening");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKHORIZONTALOPENING_IMPL_HPP
