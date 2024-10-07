/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSpecifiedFlowRate_IMPL_HPP
#define MODEL_AIRFLOWNETWORKSpecifiedFlowRate_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirflowNetworkSpecifiedFlowRate_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkSpecifiedFlowRate.*/
    class MODEL_API AirflowNetworkSpecifiedFlowRate_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkSpecifiedFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkSpecifiedFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkSpecifiedFlowRate_Impl(const AirflowNetworkSpecifiedFlowRate_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkSpecifiedFlowRate_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double airFlowValue() const;

      std::string airFlowUnits() const;

      bool isAirFlowUnitsDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirFlowValue(double airFlowValue);

      bool setAirFlowUnits(const std::string& airFlowUnits);

      void resetAirFlowUnits();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkSpecifiedFlowRate");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSpecifiedFlowRate_IMPL_HPP
