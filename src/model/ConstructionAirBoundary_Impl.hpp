/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTIONAIRBOUNDARY_IMPL_HPP
#define MODEL_CONSTRUCTIONAIRBOUNDARY_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ConstructionBase_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;

  namespace detail {

    /** ConstructionAirBoundary_Impl is a ConstructionBase_Impl that is the implementation class for ConstructionAirBoundary.*/
    class MODEL_API ConstructionAirBoundary_Impl : public ConstructionBase_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ConstructionAirBoundary_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ConstructionAirBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ConstructionAirBoundary_Impl(const ConstructionAirBoundary_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConstructionAirBoundary_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool isOpaque() const override;

      virtual bool isFenestration() const override;

      virtual bool isSolarDiffusing() const override;

      virtual bool isModelPartition() const override;

      virtual int renderingColorIndex() const override;

      //@}
      /** @name Getters */
      //@{

      std::string airExchangeMethod() const;

      bool isAirExchangeMethodDefaulted() const;

      double simpleMixingAirChangesPerHour() const;

      bool isSimpleMixingAirChangesPerHourDefaulted() const;

      // TODO: Check return type. From object lists, some candidates are: Schedule.
      boost::optional<Schedule> simpleMixingSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirExchangeMethod(const std::string& airExchangeMethod);

      void resetAirExchangeMethod();

      bool setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour);

      void resetSimpleMixingAirChangesPerHour();

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      // Note Schedules are passed by reference, not const reference.
      bool setSimpleMixingSchedule(Schedule& schedule);

      void resetSimpleMixingSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ConstructionAirBoundary");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONAIRBOUNDARY_IMPL_HPP
