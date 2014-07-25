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

#ifndef OPENSTUDIO_RESULTSTABCONTROLLER_HPP
#define OPENSTUDIO_RESULTSTABCONTROLLER_HPP

#include "MainTabController.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {

namespace model {

class Model;

}

class ResultsTabView;

class ResultsTabController : public MainTabController
{
  Q_OBJECT

  public:
    ResultsTabController();

    virtual ~ResultsTabController() {}
    void searchForExistingResults(const openstudio::path &t_runDir);

  public slots:
    void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputFile);
    void onUnitSystemChange(bool t_isIP);

  private slots:

  //void removeResult(model::myModelClass &);
  void removeResult();
  
  void importResult();

  private:
  
  ResultsTabView * m_resultsTabView;

  REGISTER_LOGGER("openstudio::ResultsTabController");
};

} // openstudio

#endif // OPENSTUDIO_RESULTSTABCONTROLLER_HPP
