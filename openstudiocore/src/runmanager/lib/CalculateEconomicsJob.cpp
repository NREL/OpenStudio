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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "CalculateEconomicsJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../energyplus/ForwardTranslator.hpp"


#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  CalculateEconomicsJob::CalculateEconomicsJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::CalculateEconomics, t_tools, t_params, t_files, t_restoreData)
  {

  }

  CalculateEconomicsJob::~CalculateEconomicsJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void CalculateEconomicsJob::requestStop()
  {
  }

  bool CalculateEconomicsJob::outOfDateImpl(const boost::optional<QDateTime> &) const
  {
    assert(!"Not implemented");
    return true;
  }

  void CalculateEconomicsJob::cleanup()
  {
    assert(!"Not implemented");
  }

  std::string CalculateEconomicsJob::description() const
  {
    assert(!"Not implemented");
    std::string desc = "CalculateEconomics";
    return desc;
  }

  void CalculateEconomicsJob::startImpl(const std::shared_ptr<ProcessCreator> &t_pc)
  {
    assert(!"Not implemented");
  }

  std::string CalculateEconomicsJob::getOutput() const
  {
    assert(!"Not implemented");
    return "";
  }

  /*
  std::vector<FileInfo> CalculateEconomicsJob::inputFiles() const
  {
    assert(!"Not implemented");
    return std::vector<FileInfo>();
  }
*/

  Files CalculateEconomicsJob::outputFilesImpl() const
  {
    assert(!"Not implemented");
    return Files();
  }

  void CalculateEconomicsJob::basePathChanged()
  {
  }

} // detail
} // runmanager
} // openstudio



