#ifndef RUNMANAGER_PARALLELENERGYPLUS_HPP_
#define RUNMANAGER_PARALLELENERGYPLUS_HPP_

#include <utilities/core/Path.hpp>

#include <boost/filesystem.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/shared_ptr.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObject.hpp>


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

#endif
