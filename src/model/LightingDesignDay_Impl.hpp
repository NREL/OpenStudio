/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTINGDESIGNDAY_IMPL_HPP
#define MODEL_LIGHTINGDESIGNDAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class DateTime;
class Date;
class Time;

namespace model {

  namespace detail {

    /** LightingDesignDay_Impl is a ModelObject_Impl that is the implementation class for LightingDesignDay.*/
    class MODEL_API LightingDesignDay_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      LightingDesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LightingDesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LightingDesignDay_Impl(const LightingDesignDay_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LightingDesignDay_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //}
      /** @name Getters */
      //@{

      std::string cieSkyModel() const;

      int snowIndicator() const;

      bool isSnowIndicatorDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setCIESkyModel(const std::string& cIESkyModel);

      bool setSnowIndicator(int snowIndicator);

      void resetSnowIndicator();

      //@}

      openstudio::Date date() const;

      bool setDate(const openstudio::Date& date);

      std::vector<openstudio::Time> simulationTimes() const;

      std::vector<openstudio::DateTime> simulationDateTimes() const;

      bool addSimulationTime(const openstudio::Time& time);

      void clearSimulationTimes();

      // ensure that this object does not contain the date 2/29
      void ensureNoLeapDays();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LightingDesignDay");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGDESIGNDAY_IMPL_HPP
