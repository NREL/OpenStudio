/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_METERCUSTOMDECREMENT_IMPL_HPP
#define MODEL_METERCUSTOMDECREMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class FuelType;

namespace model {

  namespace detail {

    /** MeterCustomDecrement_Impl is a ModelObject_Impl that is the implementation class for MeterCustomDecrement.*/
    class MODEL_API MeterCustomDecrement_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      MeterCustomDecrement_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      MeterCustomDecrement_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      MeterCustomDecrement_Impl(const MeterCustomDecrement_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~MeterCustomDecrement_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> fuelType() const;

      std::string sourceMeterName() const;

      // Return a vector of (Key, Var) pairs
      std::vector<std::pair<std::string, std::string>> keyVarGroups();

      // Return the number of (KeyName, OutputVariableorMeterName) groups
      unsigned numKeyVarGroups() const;

      // Lower Level functions
      /** Get the Key Name at index. Indexing starts at 0. */
      boost::optional<std::string> keyName(unsigned index) const;

      /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
      boost::optional<std::string> outputVariableorMeterName(unsigned index) const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const FuelType& fuelType);

      void resetFuelType();

      bool setSourceMeterName(const std::string& sourceMeterName);

      // Add a new (Key, Var) group
      bool addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName);

      // Remove the (Key, Var) group at given index
      bool removeKeyVarGroup(unsigned groupIndex);

      // Remove all the (Key, Var) groups
      void removeAllKeyVarGroups();

      // Lower level functions
      /** Set the Key Name at index. Indexing starts at 0. */
      bool setKeyName(unsigned index, const std::string& str);

      /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
      bool setOutputVariableorMeterName(unsigned index, const std::string& str);

      //@}
      /** @name Other */
      //@{

      //@}
      /** @name Type Casting */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MeterCustomDecrement");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOMDECREMENT_IMPL_HPP
