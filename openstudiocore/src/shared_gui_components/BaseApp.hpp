#ifndef OPENSTUDIO_BASEAPP_HPP
#define OPENSTUDIO_BASEAPP_HPP

#include <QWidget>
#include <boost/optional.hpp>
//#include "EditController.hpp"

namespace openstudio {
  class MeasureManager;
  class EditController;

  namespace analysisdriver {
    class SimpleProject;
  }
  
  namespace model {
    class Model;
  }

  class BaseApp
  {
    public:

      virtual ~BaseApp() {}

      virtual boost::optional<analysisdriver::SimpleProject> project() = 0;
      virtual QWidget *mainWidget() = 0;
      virtual openstudio::MeasureManager &measureManager() = 0;
      virtual void updateSelectedMeasureState() = 0;

      virtual void addMeasure() = 0;
      virtual void duplicateSelectedMeasure() = 0;
      virtual void updateMyMeasures() = 0;
      virtual void updateBCLMeasures() = 0;
      virtual void downloadUpdatedBCLMeasures() = 0;
      virtual void openBclDlg() = 0;

      virtual void chooseHorizontalEditTab() = 0;
      virtual QSharedPointer<openstudio::EditController> editController() = 0;
      virtual boost::optional<openstudio::model::Model> currentModel() = 0;
  };

}

#endif
