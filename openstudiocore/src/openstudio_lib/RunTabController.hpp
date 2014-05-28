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

#ifndef OPENSTUDIO_RUNTABCONTROLLER_HPP
#define OPENSTUDIO_RUNTABCONTROLLER_HPP

#include "MainTabController.hpp"
#include "../utilities/core/Path.hpp"

namespace openstudio {

namespace runmanager {
  class RunManager;
  class JobStatusWidget;
}

namespace model {
  class Model;
}

class RunView;

class RunTabController : public MainTabController
{
  Q_OBJECT

 public:

  RunTabController(const model::Model & model, const openstudio::path &t_modelPath,
      const openstudio::path &t_tempFolder, openstudio::runmanager::RunManager t_runManager);

  virtual ~RunTabController() {}

  openstudio::RunView * runView();

  runmanager::RunManager runManager();

  enum TabID
  {
    OUTPUT,
    TREE
  };

 signals:
    void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputFile);
    void toolsUpdated();
    void useRadianceStateChanged(bool);


 public slots:
    void updateToolsWarnings();

 private:

  RunView * m_runView;

  openstudio::runmanager::JobStatusWidget * m_status;
};

} // openstudio

#endif // OPENSTUDIO_RUNTABCONTROLLER_HPP
