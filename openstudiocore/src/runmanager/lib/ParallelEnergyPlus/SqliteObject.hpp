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

#ifndef RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP
#define RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP

#include <iomanip>
#include <vector>

#include "../../../utilities/core/Path.hpp"

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

#endif // RUNMANAGER_LIB_PARALLELENERGYPLUS_SQLITEOBJECT_HPP
