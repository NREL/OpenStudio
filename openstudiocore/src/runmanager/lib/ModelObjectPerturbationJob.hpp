/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef OPENSTUDIO_MODELOBJECTPERTURBATIONJOB_HPP
#define OPENSTUDIO_MODELOBJECTPERTURBATIONJOB_HPP

#include <utilities/core/Logger.hpp>
#include "Job_Impl.hpp"
#include "ModelInModelOutJob.hpp"
#include <model/Model.hpp>

namespace openstudio {
namespace ruleset {
  class ModelRuleset;
}
namespace runmanager {
namespace detail {

  /**
   * Job type for perturbing model objects. It is an implementation of the ModelInModelOutJob
   * base class and can be merged with other ModelObjectPerturbationJob objects for 
   * efficiency.
   *
   * \sa openstudio::runmanager::ModelInModelOutJob
   * \sa \ref ModelobjectPerturbationJobType
   */
  class ModelObjectPerturbationJob : public ModelInModelOutJob
  {
    Q_OBJECT;

    public:

      ModelObjectPerturbationJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData);

      virtual ~ModelObjectPerturbationJob();

      static JobParams formatRuleData(const openstudio::ruleset::ModelRuleset& modelRuleset);

    protected:

      virtual model::Model modelToModelRun(const model::Model &t_model);

    private:
      REGISTER_LOGGER("openstudio.runmanager.ModelObjectPerturbationJob");

      openstudio::ruleset::ModelRuleset getModelRuleset() const;
  }; 

}
}
}

#endif

