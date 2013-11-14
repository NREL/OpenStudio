#ifndef _runmanager_jobtype_hpp_
#define _runmanager_jobtype_hpp_

#include <utilities/core/Enum.hpp>

namespace openstudio {
namespace runmanager {

/** \class JobType
 *  \brief Enumeration of RunManager job types.
 *  \details JobType::ModelObjectPerturbation has been deprecated. See the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
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
 *  \endcode */
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
