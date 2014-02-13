#ifndef OPENSTUDIO_RUNMANAGER_JSONWORKFLOWOPTIONS_HPP
#define OPENSTUDIO_RUNMANAGER_JSONWORKFLOWOPTIONS_HPP

namespace openstudio {
namespace runmanager {

struct JSONWorkflowOptions
{
  JSONWorkflowOptions()
    : throwOnNoWorkflowVariable(false),
      flatOutDir(false),
      optimizeJobTree(false)
  {
  }

  JSONWorkflowOptions(bool t_throwOnNoWorkflowVariable, bool t_flatOutDir, bool t_optimizeJobTree)
    : throwOnNoWorkflowVariable(t_throwOnNoWorkflowVariable),
      flatOutDir(t_flatOutDir),
      optimizeJobTree(t_optimizeJobTree)
  {
  }

  bool throwOnNoWorkflowVariable;
  bool flatOutDir;
  bool optimizeJobTree;

};

}
}

#endif
