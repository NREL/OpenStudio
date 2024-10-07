/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TABLELOOKUP_HPP
#define MODEL_TABLELOOKUP_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  class ModelObjectList;
  class TableIndependentVariable;

  namespace detail {

    class TableLookup_Impl;

  }  // namespace detail

  /** TableLookup is a Curve that wraps the OpenStudio IDD object 'OS:Table:Lookup'. */
  class MODEL_API TableLookup : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Create a TableLookup object with the specified number
    * of independent variables.
    */
    explicit TableLookup(const Model& model);

    virtual ~TableLookup() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    TableLookup(const TableLookup& other) = default;
    TableLookup(TableLookup&& other) = default;
    TableLookup& operator=(const TableLookup&) = default;
    TableLookup& operator=(TableLookup&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> normalizationMethodValues();
    static std::vector<std::string> validNormalizationMethodValues();

    static std::vector<std::string> outputUnitTypeValues();
    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    std::string normalizationMethod() const;

    double normalizationDivisor() const;

    boost::optional<double> minimumOutput() const;

    boost::optional<double> maximumOutput() const;

    std::string outputUnitType() const;

    //@}
    /** @name Setters */
    //@{

    bool setNormalizationMethod(const std::string& normalizationMethod);

    bool setNormalizationDivisor(double normalizationDivisor);

    bool setMinimumOutput(double minimumOutput);
    void resetMinimumOutput();

    bool setMaximumOutput(double maximumOutput);
    void resetMaximumOutput();

    bool setOutputUnitType(const std::string& outputUnitType);

    //@}
    /** @name Other */
    //@{

    bool addOutputValue(double outputValue);
    bool removeOutputValue(unsigned groupIndex);
    void removeAllOutputValues();
    std::vector<double> outputValues() const;
    unsigned numberofOutputValues() const;
    // Directly set the Output Values from a vector, will delete any existing Output Values
    bool setOutputValues(const std::vector<double>& outputValues);

    bool addIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
    bool removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
    void removeAllIndependentVariables();
    std::vector<TableIndependentVariable> independentVariables() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::TableLookup_Impl;

    explicit TableLookup(std::shared_ptr<detail::TableLookup_Impl> impl);

    friend class detail::TableLookup_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.TableLookup");
  };

  /** \relates TableLookup*/
  using OptionalTableLookup = boost::optional<TableLookup>;

  /** \relates TableLookup*/
  using TableLookupVector = std::vector<TableLookup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLELOOKUP_HPP
