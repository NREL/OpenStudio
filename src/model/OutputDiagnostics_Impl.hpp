/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTDIAGNOSTICS_IMPL_HPP
#define MODEL_OUTPUTDIAGNOSTICS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputDiagnostics_Impl is a ModelObject_Impl that is the implementation class for OutputDiagnostics.*/
    class MODEL_API OutputDiagnostics_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputDiagnostics_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputDiagnostics_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputDiagnostics_Impl(const OutputDiagnostics_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputDiagnostics_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<std::string> keys() const;

      //@}
      /** @name Setters */
      //@{

      // Return false if key isn't valid. If is already present, not added twice (ensures unicity) and logs an info in that case too
      bool addKey(const std::string& key);

      // Calls clearKeys, then for each k, addKey(k)
      bool setKeys(const std::vector<std::string>& keys);

      // Just a convenience function to add 'DisplayExtraWarnings' as a key since it's so common
      bool enableDisplayExtraWarnings();

      // Clears out every key
      void clearKeys();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputDiagnostics");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTDIAGNOSTICS_IMPL_HPP
