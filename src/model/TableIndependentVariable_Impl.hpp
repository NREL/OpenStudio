/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TABLEINDEPENDENTVARIABLE_IMPL_HPP
#define MODEL_TABLEINDEPENDENTVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class TableLookup;

  namespace detail {

    /** TableIndependentVariable_Impl is a ResourceObject_Impl that is the implementation class for TableIndependentVariable.*/
    class MODEL_API TableIndependentVariable_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      TableIndependentVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      TableIndependentVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      TableIndependentVariable_Impl(const TableIndependentVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~TableIndependentVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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

      std::vector<TableLookup> tableLookups() const;

      bool addValue(double value);
      bool removeValue(unsigned groupIndex);
      void removeAllValues();
      std::vector<double> values() const;
      bool setValues(const std::vector<double>& values);
      unsigned numberofValues() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.TableIndependentVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLEINDEPENDENTVARIABLE_IMPL_HPP
