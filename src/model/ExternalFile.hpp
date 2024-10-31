/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALFILE_HPP
#define MODEL_EXTERNALFILE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio {

namespace model {

  class ScheduleFile;
  class PythonPluginInstance;
  class ChillerElectricASHRAE205;

  namespace detail {

    class ExternalFile_Impl;

  }  // namespace detail

  /** ExternalFile is a ResourceObject that wraps the OpenStudio IDD object 'OS:External:File'. */
  class MODEL_API ExternalFile : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ExternalFile() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalFile(const ExternalFile& other) = default;
    ExternalFile(ExternalFile&& other) = default;
    ExternalFile& operator=(const ExternalFile&) = default;
    ExternalFile& operator=(ExternalFile&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> columnSeparatorValues();

    /** This factory method will look for an existing ExternalFile with the same file name. If not it will try to create one.
      * - If copyFile is true, the file will be copied to the first entry of the WorkflowJSON's absoluteFilePaths() (or absoluteRootDir() if empty);
      * - If copyFile is false, it assumes this is an absolute path.
      * In both cases, the filename should resolve to a valid path or this will return an empty optional. */
    static boost::optional<ExternalFile> getExternalFile(const Model& model, const std::string& filename, bool copyFile = true);

    /** @name Getters */
    //@{

    std::string fileName() const;

    path filePath() const;

    //boost::optional<std::string> columnSeparator() const;

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
    /// @cond
    using ImplType = detail::ExternalFile_Impl;

    explicit ExternalFile(std::shared_ptr<detail::ExternalFile_Impl> impl);

    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalFile");

    ExternalFile(const Model& model, const std::string& filename, bool copyFile = true);

    bool setFileName(const std::string& fileName);
  };

  /** \relates ExternalFile*/
  using OptionalExternalFile = boost::optional<ExternalFile>;

  /** \relates ExternalFile*/
  using ExternalFileVector = std::vector<ExternalFile>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALFILE_HPP
