/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TABLEINDEPENDENTVARIABLE_HPP
#define MODEL_TABLEINDEPENDENTVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class TableLookup;

  namespace detail {

    class TableIndependentVariable_Impl;

  }  // namespace detail

  /** TableIndependentVariable is a ResourceObject that wraps the OpenStudio IDD object 'OS:Table:IndependentVariable'. */
  class MODEL_API TableIndependentVariable : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit TableIndependentVariable(const Model& model);

    virtual ~TableIndependentVariable() override = default;
    ;
    // Default the copy and move operators because the virtual dtor is explicit
    TableIndependentVariable(const TableIndependentVariable& other) = default;
    TableIndependentVariable(TableIndependentVariable&& other) = default;
    TableIndependentVariable& operator=(const TableIndependentVariable&) = default;
    TableIndependentVariable& operator=(TableIndependentVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> interpolationMethodValues();
    static std::vector<std::string> validInterpolationMethodValues();

    static std::vector<std::string> extrapolationMethodValues();
    static std::vector<std::string> validExtrapolationMethodValues();

    static std::vector<std::string> unitTypeValues();
    static std::vector<std::string> validUnitTypeValues();

    /** @name Getters */
    //@{

    std::string interpolationMethod() const;

    std::string extrapolationMethod() const;

    boost::optional<double> minimumValue() const;

    boost::optional<double> maximumValue() const;

    boost::optional<double> normalizationReferenceValue() const;

    std::string unitType() const;

    //@}
    /** @name Setters */
    //@{

    bool setInterpolationMethod(const std::string& interpolationMethod);

    bool setExtrapolationMethod(const std::string& extrapolationMethod);

    bool setMinimumValue(double minimumValue);
    void resetMinimumValue();

    bool setMaximumValue(double maximumValue);
    void resetMaximumValue();

    bool setNormalizationReferenceValue(double normalizationReferenceValue);
    void resetNormalizationReferenceValue();

    bool setUnitType(const std::string& unitType);

    //@}
    /** @name Other */
    //@{

    // Convenience method to return TableLookup objects that point to this TableIndependentVariable
    std::vector<TableLookup> tableLookups() const;

    bool addValue(double value);
    bool removeValue(unsigned groupIndex);
    void removeAllValues();
    std::vector<double> values() const;
    // Directly set the Values from a vector, will delete any existing Values
    bool setValues(const std::vector<double>& values);
    unsigned numberofValues() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::TableIndependentVariable_Impl;

    explicit TableIndependentVariable(std::shared_ptr<detail::TableIndependentVariable_Impl> impl);

    friend class detail::TableIndependentVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.TableIndependentVariable");
  };

  /** \relates TableIndependentVariable*/
  using OptionalTableIndependentVariable = boost::optional<TableIndependentVariable>;

  /** \relates TableIndependentVariable*/
  using TableIndependentVariableVector = std::vector<TableIndependentVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLEINDEPENDENTVARIABLE_HPP
