#ifndef SQLITEOBJECT_HPP_
#define SQLITEOBJECT_HPP_

#include <iomanip>
#include <vector>

#include <utilities/core/Path.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>

#include "sqlite3.h"


class Results {

  public:

    Results();
    ~Results();

    void clear();

    std::vector<std::string> header;
    std::vector< std::vector<std::string> > data;

};


class SqliteObject {

  public:
    SqliteObject(const openstudio::path &file);
    ~SqliteObject();

    bool execute(const std::string& cmd);
    bool deleteDay(const boost::gregorian::date &);
    bool removeDesignDay();  //BLB

    void print();
    void getData(std::vector<std::string> &data);
    double sum(const std::string& name);
    void summary();
    void extract();
    void extract_by_month();
    void extract_by_variable();
    void extract_total();
    boost::gregorian::date getStartDay();


  private:
    sqlite3 *m_db;
    Results *m_results;

    openstudio::path m_file;
    int findHeaderIndex(const std::string& name);


};

#endif
