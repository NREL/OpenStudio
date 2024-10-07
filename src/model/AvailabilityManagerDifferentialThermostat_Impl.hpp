/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** AvailabilityManagerDifferentialThermostat_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerDifferentialThermostat.*/
    class MODEL_API AvailabilityManagerDifferentialThermostat_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerDifferentialThermostat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerDifferentialThermostat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerDifferentialThermostat_Impl(const AvailabilityManagerDifferentialThermostat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerDifferentialThermostat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> hotNode() const;

      boost::optional<Node> coldNode() const;

      double temperatureDifferenceOnLimit() const;

      double temperatureDifferenceOffLimit() const;

      //@}
      /** @name Setters */
      //@{

      bool setHotNode(const boost::optional<Node>& node);

      void resetHotNode();

      bool setColdNode(const boost::optional<Node>& node);

      void resetColdNode();

      bool setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit);

      bool setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerDifferentialThermostat");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP
