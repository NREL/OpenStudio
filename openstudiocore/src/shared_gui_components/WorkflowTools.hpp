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

#ifndef SHAREDGUICOMPONENTS_WORKFLOWTOOLS_HPP
#define SHAREDGUICOMPONENTS_WORKFLOWTOOLS_HPP

#include <vector>
#include <string>
#include <boost/optional.hpp>

namespace openstudio
{
  namespace runmanager
  {
    class RunManager;
  }

  namespace model
  {
    class Model;
  }

  namespace analysis
  {
    class Problem;
  }

  namespace analysisdriver
  {
    class SimpleProject;
  }

  void getRadiancePreRunWarningsAndErrors(std::vector<std::string> &t_warnings, 
      std::vector<std::string> &t_errors,
      openstudio::runmanager::RunManager &t_runManager,
      boost::optional<openstudio::model::Model> &t_model);

  bool projectHasRadiance(const openstudio::analysisdriver::SimpleProject &t_project);

  void addRadianceToProject(openstudio::analysisdriver::SimpleProject &t_project);

  void removeRadianceFromProject(openstudio::analysisdriver::SimpleProject &t_project);

  boost::optional<int> getModelMeasureInsertStep(const openstudio::analysis::Problem &t_problem);
}

#endif // SHAREDGUICOMPONENTS_WORKFLOWTOOLS_HPP
