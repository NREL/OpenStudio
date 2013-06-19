#ifndef _runmanager_jobtype_hpp_
#define _runmanager_jobtype_hpp_

#include <utilities/core/Enum.hpp>

namespace openstudio {
namespace runmanager {

  OPENSTUDIO_ENUM(JobType,
      ((Workflow))
      ((EnergyPlus))
      ((ModelToIdf))
      ((CalculateEconomics))
      ((ExpandObjects))
      ((ReadVars))
      ((XMLPreprocessor))
      ((PreviewIES))
      ((Ruby))
      ((Null))
      ((ModelObjectPerturbation))
      ((EnergyPlusPostProcess))
      ((ParallelEnergyPlusSplit))
      ((ParallelEnergyPlusJoin))
      ((IdfToModel))
      ((EnergyPlusPreProcess))
      ((Basement))
      ((Slab))
      ((OpenStudioPostProcess))
      ((ModelToRad))
      ((ModelToRadPreProcess))
      ((Dakota))
      ((UserScript))
    );

}
}


#endif
