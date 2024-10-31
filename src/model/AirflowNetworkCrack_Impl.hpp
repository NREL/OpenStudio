/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCRACK_IMPL_HPP
#define MODEL_AIRFLOWNETWORKCRACK_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkCrack_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkCrack.*/
    class MODEL_API AirflowNetworkCrack_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkCrack_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkCrack_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkCrack_Impl(const AirflowNetworkCrack_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkCrack_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double airMassFlowCoefficient() const;

      double airMassFlowExponent() const;

      bool isAirMassFlowExponentDefaulted() const;

      boost::optional<AirflowNetworkReferenceCrackConditions> referenceCrackConditions() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions);

      bool setAirMassFlowExponent(double airMassFlowExponent);

      void resetAirMassFlowExponent();

      bool setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);

      void resetReferenceCrackConditions();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkCrack");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCRACK_IMPL_HPP
