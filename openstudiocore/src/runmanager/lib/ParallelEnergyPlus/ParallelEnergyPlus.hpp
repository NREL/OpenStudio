/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
