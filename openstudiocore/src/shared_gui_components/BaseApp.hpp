/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef SHAREDGUICOMPONENTS_BASEAPP_HPP
#define SHAREDGUICOMPONENTS_BASEAPP_HPP

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

#endif // SHAREDGUICOMPONENTS_BASEAPP_HPP
