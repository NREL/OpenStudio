/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RUNMANAGER_LIB_XMLPREPROCESSORJOB_HPP
#define RUNMANAGER_LIB_XMLPREPROCESSORJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "ToolBasedJob.hpp"
#include "../../energyplus/ErrorFile.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /// ToolBasedJob that calls the xmlpreproc executable
  ///
  /// \sa \ref XMLPreprocessorJobType
  class XMLPreprocessorJob : public ToolBasedJob
  {
    Q_OBJECT;

    public:
      XMLPreprocessorJob(const UUID &t_uuid,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
          const JobState &t_restoreData);

      virtual ~XMLPreprocessorJob();


    protected:
      virtual std::string detailedDescriptionImpl() const override
      {
        return "";
      }


      virtual std::string descriptionImpl() const override;
      virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const override;
      virtual void startHandlerImpl() override;

      virtual void basePathChanged() override;

    private:
      REGISTER_LOGGER("openstudio.runmanager.XMLPreprocessorJob");

      void getFiles(const Files &t_files) const;
       
      mutable boost::optional<openstudio::runmanager::FileInfo> m_xml;
      std::string m_description; //< Description of job

   }; 

}
}
}
#endif // RUNMANAGER_LIB_XMLPREPROCESSORJOB_HPP
