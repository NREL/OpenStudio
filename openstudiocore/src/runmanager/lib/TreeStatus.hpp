#ifndef OPENSTUDIO_RUNMANAGER_TREE_STATUS_HPP__
#define OPENSTUDIO_RUNMANAGER_TREE_STATUS_HPP__


#include <string>
#include <utilities/core/Enum.hpp>
#include "RunManagerAPI.hpp"
#include <QMetaType>

namespace openstudio {
namespace runmanager {

  OPENSTUDIO_ENUM(TreeStatusEnum,
      ((Running))
      ((Waiting))
      ((Finished))
      ((Failed))
      ((Canceled))
    );
     
  

}
}

#endif

