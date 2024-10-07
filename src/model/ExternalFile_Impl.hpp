/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALFILE_IMPL_HPP
#define MODEL_EXTERNALFILE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {

namespace model {

  class ExternalFile;
  class ScheduleFile;
  class PythonPluginInstance;
  class ChillerElectricASHRAE205;

  namespace detail {

    /** ExternalFile_Impl is a ResourceObject_Impl that is the implementation class for ExternalFile.*/
    class MODEL_API ExternalFile_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalFile_Impl(const ExternalFile_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExternalFile_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      std::string fileName() const;

      path filePath() const;

      //boost::optional<std::string> columnSeparator() const;

      //char columnSeparatorChar() const;

      //bool isColumnSeparatorDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      //bool setColumnSeparator(const std::string& columnSeparator);

      //void resetColumnSeparator();

      //@}
      /** @name Other */
      //@{
      //bool isValid();

      std::vector<ScheduleFile> scheduleFiles() const;

      std::vector<PythonPluginInstance> pythonPluginInstances() const;

      std::vector<ChillerElectricASHRAE205> chillerElectricASHRAE205s() const;

      //@}
     protected:
      bool setFileName(const std::string& fileName);
      friend class openstudio::model::ExternalFile;

     private:
      REGISTER_LOGGER("openstudio.model.ExternalFile");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFILE_IMPL_HPP
