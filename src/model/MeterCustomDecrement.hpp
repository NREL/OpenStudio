/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_METERCUSTOMDECREMENT_HPP
#define MODEL_METERCUSTOMDECREMENT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

//#include <memory>

namespace openstudio {

class FuelType;

namespace model {

  namespace detail {

    class MeterCustomDecrement_Impl;

  }  // namespace detail

  /** MeterCustomDecrement is a ModelObject that wraps the OpenStudio IDD object 'OS:Meter:Custom'. */
  class MODEL_API MeterCustomDecrement : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructs a new MeterCustomDecrement object in the model.
    explicit MeterCustomDecrement(const Model& model, const std::string& sourceMeterName);

    virtual ~MeterCustomDecrement() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MeterCustomDecrement(const MeterCustomDecrement& other) = default;
    MeterCustomDecrement(MeterCustomDecrement&& other) = default;
    MeterCustomDecrement& operator=(const MeterCustomDecrement&) = default;
    MeterCustomDecrement& operator=(MeterCustomDecrement&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

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
    bool setFuelType(const std::string& fuelType);

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
    /// @cond
    using ImplType = detail::MeterCustomDecrement_Impl;

    explicit MeterCustomDecrement(std::shared_ptr<detail::MeterCustomDecrement_Impl> impl);

    friend class detail::MeterCustomDecrement_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    std::shared_ptr<detail::MeterCustomDecrement_Impl> m_impl;

    REGISTER_LOGGER("openstudio.model.MeterCustomDecrement");
  };

  /** \relates MeterCustomDecrement*/
  using OptionalMeterCustomDecrement = boost::optional<MeterCustomDecrement>;

  /** \relates MeterCustomDecrement*/
  using MeterCustomDecrementVector = std::vector<MeterCustomDecrement>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOMDECREMENT_HPP
