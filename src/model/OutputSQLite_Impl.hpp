/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTSQLITE_IMPL_HPP
#define MODEL_OUTPUTSQLITE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputSQLite_Impl is a ModelObject_Impl that is the implementation class for OutputSQLite.*/
    class MODEL_API OutputSQLite_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputSQLite_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputSQLite_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputSQLite_Impl(const OutputSQLite_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputSQLite_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string optionType() const;

      std::string unitConversionforTabularData() const;

      //@}
      /** @name Setters */
      //@{

      bool setOptionType(const std::string& optionType);

      bool setUnitConversionforTabularData(const std::string& unitConversionforTabularData);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputSQLite");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTSQLITE_IMPL_HPP
