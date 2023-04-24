/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~ExternalFile() = default;
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
