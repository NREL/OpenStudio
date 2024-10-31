/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class DetailedOpeningFactorData;

  namespace detail {

    /** AirflowNetworkDetailedOpening_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkDetailedOpening.*/
    class MODEL_API AirflowNetworkDetailedOpening_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDetailedOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDetailedOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDetailedOpening_Impl(const AirflowNetworkDetailedOpening_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDetailedOpening_Impl() override = default;

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

      std::string typeofRectangularLargeVerticalOpening() const;

      bool isTypeofRectangularLargeVerticalOpeningDefaulted() const;

      double extraCrackLengthorHeightofPivotingAxis() const;

      bool isExtraCrackLengthorHeightofPivotingAxisDefaulted() const;

      std::vector<DetailedOpeningFactorData> openingFactors() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);

      bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);

      void resetAirMassFlowExponentWhenOpeningisClosed();

      bool setTypeofRectangularLargeVerticalOpening(const std::string& typeofRectangularLargeVerticalOpening);

      void resetTypeofRectangularLargeVerticalOpening();

      bool setExtraCrackLengthorHeightofPivotingAxis(double extraCrackLengthorHeightofPivotingAxis);

      void resetExtraCrackLengthorHeightofPivotingAxis();

      bool setOpeningFactors(const std::vector<DetailedOpeningFactorData>& factors);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDetailedOpening");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDETAILEDOPENING_IMPL_HPP
