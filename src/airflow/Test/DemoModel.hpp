/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef AIRFLOW_TEST_DEMOMODEL_HPP
#define AIRFLOW_TEST_DEMOMODEL_HPP

#include "../../model/Model.hpp"

boost::optional<openstudio::model::Model> buildDemoModel2012(openstudio::model::Model model);
boost::optional<openstudio::model::Model> buildDemoModel2014(openstudio::model::Model model);

#endif
