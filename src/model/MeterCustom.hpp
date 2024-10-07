/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_METERCUSTOM_HPP
#define MODEL_METERCUSTOM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

//#include <memory>

namespace openstudio {

class FuelType;

namespace model {

  namespace detail {

    class MeterCustom_Impl;

  }  // namespace detail

  /** MeterCustom is a ModelObject that wraps the OpenStudio IDD object 'OS:Meter:Custom'. */
  class MODEL_API MeterCustom : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructs a new MeterCustom object in the model.
    explicit MeterCustom(const Model& model);

    virtual ~MeterCustom() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MeterCustom(const MeterCustom& other) = default;
    MeterCustom(MeterCustom&& other) = default;
    MeterCustom& operator=(const MeterCustom&) = default;
    MeterCustom& operator=(MeterCustom&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters */
    //@{

    boost::optional<std::string> fuelType() const;

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
    using ImplType = detail::MeterCustom_Impl;

    explicit MeterCustom(std::shared_ptr<detail::MeterCustom_Impl> impl);

    friend class detail::MeterCustom_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    std::shared_ptr<detail::MeterCustom_Impl> m_impl;

    REGISTER_LOGGER("openstudio.model.MeterCustom");
  };

  /** \relates MeterCustom*/
  using OptionalMeterCustom = boost::optional<MeterCustom>;

  /** \relates MeterCustom*/
  using MeterCustomVector = std::vector<MeterCustom>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOM_HPP
