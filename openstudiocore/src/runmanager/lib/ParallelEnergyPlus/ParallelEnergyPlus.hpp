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

#ifndef RUNMANAGER_LIB_PARALLELENERGYPLUS_PARALLELENERGYPLUS_HPP
#define RUNMANAGER_LIB_PARALLELENERGYPLUS_PARALLELENERGYPLUS_HPP

#include "../../../utilities/core/Path.hpp"

#include <boost/filesystem.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"


class ParallelEnergyPlus {

  public:

    ParallelEnergyPlus(const openstudio::path &t_path, int t_numPartitions, int t_numOffset);
    ~ParallelEnergyPlus();

    void writePartition(int t_num, const openstudio::path &t_path) const;

  private:
    REGISTER_LOGGER("openstudio.runmanager.ParallelEnergyPlus");

    static std::pair<openstudio::Workspace, openstudio::WorkspaceObject> getRunPeriod(const openstudio::path &t_path);
    static void getRunPeriod(const openstudio::WorkspaceObject &t_runPeriod, boost::gregorian::date &, boost::gregorian::date &);

    static void computeLinearCombination(int &A, int &B, int &X,
        int &Y, double &t_averageDays, const double daysPerPeriod, const double totalDays, const int t_numPartitions);

    static std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > createPartitions(const openstudio::WorkspaceObject &t_runPeriod, 
        int t_offset, int t_numPartitions);

    static void createPartitions(boost::gregorian::date &d1, const boost::gregorian::date &ed,
        const int A, const int startVal, const int endVal, const int t_offset, const boost::gregorian::date &t_startDate, 
        std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > &t_partitions);

    void modifyIdf(std::string &);


    openstudio::path m_idfPath;
    int m_numPartitions;


    // How many days prior should the simulator run?
    int m_offset;


    std::pair<openstudio::Workspace, openstudio::WorkspaceObject> m_runPeriod;

    // How many partitions?
    std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > m_partitions;

    std::string m_remove_sql;

};

#endif // RUNMANAGER_LIB_PARALLELENERGYPLUS_PARALLELENERGYPLUS_HPP
