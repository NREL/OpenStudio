/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP
#define MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** LightingSimulationControl_Impl is a ModelObject_Impl that is the implementation class for LightingSimulationControl.*/
    class MODEL_API LightingSimulationControl_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      LightingSimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LightingSimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LightingSimulationControl_Impl(const LightingSimulationControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LightingSimulationControl_Impl() override = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      bool runSimulationforDesignDays() const;

      bool isRunSimulationforDesignDaysDefaulted() const;

      bool runSimulationforWeatherFileRunPeriods() const;

      bool isRunSimulationforWeatherFileRunPeriodsDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setRunSimulationforDesignDays(bool runSimulationforDesignDays);

      void resetRunSimulationforDesignDays();

      bool setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

      void resetRunSimulationforWeatherFileRunPeriods();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LightingSimulationControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP
