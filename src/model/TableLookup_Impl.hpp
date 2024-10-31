/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TABLELOOKUP_IMPL_HPP
#define MODEL_TABLELOOKUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  class ModelObjectList;
  class TableIndependentVariable;

  namespace detail {

    /** TableLookup_Impl is a Curve_Impl that is the implementation class for TableLookup.*/
    class MODEL_API TableLookup_Impl : public Curve_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      TableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      TableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      TableLookup_Impl(const TableLookup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~TableLookup_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int numVariables() const override;

      virtual double evaluate(const std::vector<double>& independentVariables) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ModelObjectList> independentVariableList() const;

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
      bool setOutputValues(const std::vector<double>& outputValues);
      unsigned numberofOutputValues() const;

      bool addIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
      bool removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
      void removeAllIndependentVariables();
      std::vector<TableIndependentVariable> independentVariables() const;

      bool setIndependentVariableList(const boost::optional<ModelObjectList>& modelObjectList);
      void resetIndependentVariableList();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.TableLookup");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLELOOKUP_IMPL_HPP
