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

#include <iostream>
#include <fstream>
#include <sstream>

#include "SqliteObject.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>


static int callback(void *r, int argc, char **argv, char **azColName) 
{

  Results * m_results = static_cast<Results*>(r);

  //std::cout << "callback = " << argc << std::endl;

  if( m_results->header.size() == 0 ){
    for (int i = 0; i < argc; i++){
      m_results->header.push_back(azColName[i]);
      //std::cout << azColName[i] << std::endl;
    }
  }

  std::vector<std::string> row;
  for (int i = 0; i < argc; i++){
    //std::cout << argv[i] << std::endl;
    row.push_back(argv[i] ? argv[i] : "NULL");
  }

  m_results->data.push_back(row);

  return 0;
}



Results::Results(){

}

Results::~Results(){

}

void Results::clear(){

  header.resize(0);

  for(size_t r=0; r<data.size(); ++r){
    data[r].resize(0);
  }
  data.resize(0);

}


SqliteObject::SqliteObject(const openstudio::path &file)
{

  // Open a database connection
  m_db = nullptr;
  int rc = sqlite3_open(openstudio::toString(file).c_str(), &m_db);
  if (rc)
  {
    throw std::runtime_error("Could not open database: " + openstudio::toString(file));
  }

  m_results = new Results();
}

SqliteObject::~SqliteObject(){

  //std::cout << "closing database" << std::endl;
  sqlite3_close(m_db);

  if( m_results != nullptr) {
    delete m_results;
  }

}

double SqliteObject::sum(const std::string& name)
{
  // Which column?
  int col = findHeaderIndex(name);

  //std::cout << col << std::endl;

  double res = 0.0;
  if (col > 0)
  {
    for (size_t r = 0; r < m_results->data.size(); ++r)
    {
      double tmp = static_cast<double> (atof( m_results->data[r][col].c_str()));
      res += tmp;
    }
  }

  return res;
}

int SqliteObject::findHeaderIndex(const std::string& name){

  for(size_t i=0; i<m_results->header.size(); ++i){
    if( m_results->header[i] == name) {
      return i;
    }
  }

  return -1;
}


bool SqliteObject::execute(const std::string& cmd){

  m_results->clear();
  //  char * zErrMsg = 0;
  //  sqlite3_stmt *stmt = NULL;
  //
  //  int rc = sqlite3_prepare_v2(m_db, const_cast<char *>(cmd.c_str()), cmd.length(), &stmt, NULL);
  //  if( rc!=SQLITE_OK ){
  //    fprintf(stderr, "SQL error: %s\n", zErrMsg);
  //    sqlite3_free(zErrMsg);
  //  }

  //   sqlite_compile(m_db, query, &query_tail, &vm, NULL);
  //  do
  //  {
  //    result = sqlite_step(vm, &numCols, &m_results, &columnNames);
  //    if (result == SQLITE_ROW)
  //    {
  //      for (i = 0; i < numCols; i++)
  //      {
  //        /* Do something with this field in the m_results; I guess we'll print it.*/
  //        printf("%s\t", m_results[i]);
  //      }
  //      printf("\n");
  //    }
  //  } while (result == SQLITE_ROW);
  //  sqlite_finalize(vm, NULL);

  //std::cout << " Execute: " << m_results->header.size() << std::endl;

  char *zErrMsg = nullptr;
  int rc = sqlite3_exec(m_db, cmd.c_str(), callback, m_results, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  //std::cout << " Execute: " << m_results->header.size() << std::endl;
  return true;
}

void SqliteObject::print(){

  std::vector<int> width(m_results->header.size());
  for (size_t r = 0; r < m_results->data.size(); ++r)
  {
    for (size_t c = 0; c < m_results->data[r].size(); ++c)
    {

      std::stringstream s1,s2;
      s1 << m_results->data[r][c];
      s2 << m_results->header[c];
      std::string w1 = s1.str();
      std::string w2 = s2.str();
      int maxlength = w1.length();
      if( w2.length() > w1.length()) {
        maxlength = w2.length();
      }

      if( width[c] < maxlength) {
        width[c] = maxlength;
      }

    }
  }

  std::cout << std::endl;
  // Header
  for(size_t i=0; i<m_results->header.size(); ++i){
    std::cout << std::setw (width[i] + 1);
    std::cout << m_results->header[i] << " ";
  }
  std::cout << std::endl;

  // Data
  for(size_t r=0; r<m_results->data.size(); ++r){
    for(size_t c=0; c<m_results->data[r].size(); ++c){
      std::cout << std::setw (width[c] + 1);
      std::cout << m_results->data[r][c] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void SqliteObject::summary(){

  std::stringstream cmd1,cmd2;
  cmd1 << "select * from reportMeterData order by ReportMeterDataDictionaryIndex";
  execute(cmd1.str());
  print();

  cmd2 << "select * from Time";
  execute(cmd2.str());
  print();

}

void SqliteObject::extract(){

  std::stringstream cmd;

  cmd << "select month, day, variableName, variableValue  ";
  cmd << "from ReportMeterData a, ReportMeterDataDictionary b, time t ";
  cmd << "where a.ReportMeterDataDictionaryIndex = b.ReportMeterDataDictionaryIndex ";
  cmd << "and a.timeIndex = t.timeIndex";

  execute(cmd.str());
  //print();

}

void SqliteObject::extract_by_month(){

  std::stringstream cmd;

  cmd << "select month, variableName, sum(variableValue)  ";
  cmd << "from ReportMeterData a, ReportMeterDataDictionary b, time t ";
  cmd << "where a.ReportMeterDataDictionaryIndex = b.ReportMeterDataDictionaryIndex ";
  cmd << "and a.timeIndex = t.timeIndex ";
  cmd << "group by month, variableName";

  execute(cmd.str());
  //print();

}

void SqliteObject::extract_by_variable(){

  std::stringstream cmd;

  cmd << "select  variableName, sum(variableValue)  ";
  cmd << "from ReportMeterData a, ReportMeterDataDictionary b, time t ";
  cmd << "where a.ReportMeterDataDictionaryIndex = b.ReportMeterDataDictionaryIndex ";
  cmd << "and a.timeIndex = t.timeIndex ";
  cmd << "group by  variableName";

  execute(cmd.str());
  //print();

}

void SqliteObject::extract_total(){

  std::stringstream cmd;

  cmd << "select sum(variableValue)  ";
  cmd << "from ReportMeterData a, ReportMeterDataDictionary b, time t ";
  cmd << "where a.ReportMeterDataDictionaryIndex = b.ReportMeterDataDictionaryIndex ";
  cmd << "and a.timeIndex = t.timeIndex ";

  execute(cmd.str());
  //print();

}


void SqliteObject::getData(std::vector<std::string> &data)
{

  // Data
  data.resize(0);
  for(size_t r=0; r<m_results->data.size(); ++r){
    size_t c = m_results->data[r].size()-1;
    //for(int c=0; c<m_results->data[r].size(); ++c){
    data.push_back(m_results->data[r][c]);
    //}

  }

}




bool SqliteObject::removeDesignDay()
{
  // What are the timeindex values to delete
  std::stringstream cmd1;
  cmd1 << "select TimeIndex from Time where time.daytype = 'WinterDesignDay' ";
  cmd1 << "or time.daytype = 'SummerDesignDay' or time.daytype = 'customday1' or time.daytype = 'customday2'";

  execute(cmd1.str());

  std::vector<int> time_index(0);
  for(size_t r=0; r<m_results->data.size(); ++r){
    time_index.push_back(atoi(m_results->data[r][0].c_str()));
  }

  // What are the ReportVarialbeExtendedDataIndexes associated with the timeindex values from ReportMeterData to delete
  std::stringstream cmd2;
  std::vector<int> RVED_index(0);
  for(size_t s=0; s<time_index.size(); ++s){
    cmd2 << "select ReportVariableExtendedDataIndex from reportmeterdata where timeindex =" << time_index[s];
    execute(cmd2.str());

    for(size_t r=0; r<m_results->data.size(); ++r){
      RVED_index.push_back(atoi(m_results->data[r][0].c_str()));
    }
    cmd2.str(std::string());  //clear out the string
  }

  // What are the ReportVarialbeExtendedDataIndexes associated with the timeindex values from ReportVariableData to delete
  std::stringstream cmd3;
  std::vector<int> RVEDV_index(0);
  for(size_t s=0; s<time_index.size(); ++s){
    //cmd3 << "select ReportVariableExtendedDataIndex from reportVariabledata where timeindex =" << time_index[s];
    cmd3 << "select ReportVariableExtendedDataIndex from reportVariabledata where timeindex =" << time_index[s] << " and ReportVariableExtendedDataIndex != 'NULL'";
    execute(cmd3.str());

    for(size_t r=0; r<m_results->data.size(); ++r){
      RVEDV_index.push_back(atoi(m_results->data[r][0].c_str()));
    }
    cmd3.str(std::string());  //clear out the string
  }



  execute("begin");
  // reportMeterData
  for(size_t r=0; r<time_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportMeterData where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }
  // reportMeterExtendedData
  for(size_t r=0; r<RVED_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportMeterExtendedData where ReportMeterExtendedDataIndex = " << RVED_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }

  // reportVariableData
  for(size_t r=0; r<time_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportVariableData where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }
  // reportVariableExtendedData
  for(size_t r=0; r<RVEDV_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportVariableExtendedData where ReportVariableExtendedDataIndex = " << RVEDV_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }


  // Time
  for(size_t r=0; r<time_index.size(); ++r){
    std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from Time where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }

  execute("commit");

  return true;
}

boost::gregorian::date SqliteObject::getStartDay()
{
  // What are the timeindex values to delete
  std::stringstream cmd1;
  cmd1 << "select month, day from time order by month, day limit 1";
  execute(cmd1.str());

  assert(m_results->data.size() == 1);

  boost::gregorian::date d(2010, atoi(m_results->data[0][0].c_str()), atoi(m_results->data[0][1].c_str()));

  return d;
}


bool SqliteObject::deleteDay(const boost::gregorian::date &d) 
{

  //std::cout << boost::gregorian::to_simple_string(d) << std::endl;

  boost::gregorian::greg_day gd = d.day();
  std::stringstream ssd;
  ssd << gd.as_number();
  std::string d_day = ssd.str();

  boost::gregorian::greg_month gm = d.month();
  std::stringstream ssm;
  ssm << gm.as_number();
  std::string d_month = ssm.str();

  //std::cout << " " << d_month << " " << d_day << std::endl;

  // What are the timeindex values to delete
  std::stringstream cmd1;
  cmd1 << "select TimeIndex from Time where month=" << d_month << " and day=" << d_day;
  execute(cmd1.str());

  std::vector<int> time_index(0);
  for(size_t r=0; r<m_results->data.size(); ++r){
    time_index.push_back(atoi(m_results->data[r][0].c_str()));
  }

  // What are the ReportVarialbeExtendedDataIndexes associated with the timeindex values from ReportMeterData to delete
  std::stringstream cmd2;
  std::vector<int> RVED_index(0);
  for(size_t s=0; s<time_index.size(); ++s){
    cmd2 << "select ReportVariableExtendedDataIndex from reportmeterdata where timeindex =" << time_index[s];
    execute(cmd2.str());

    for(size_t r=0; r<m_results->data.size(); ++r){
      RVED_index.push_back(atoi(m_results->data[r][0].c_str()));
    }
    cmd2.str(std::string());  //clear out the string
  }

  // What are the ReportVarialbeExtendedDataIndexes associated with the timeindex values from ReportVariableData to delete
  std::stringstream cmd3;
  std::vector<int> RVEDV_index(0);
  for(size_t s=0; s<time_index.size(); ++s){
    //cmd3 << "select ReportVariableExtendedDataIndex from reportVariabledata where timeindex =" << time_index[s];
    cmd3 << "select ReportVariableExtendedDataIndex from reportVariabledata where timeindex =" << time_index[s] << " and ReportVariableExtendedDataIndex != 'NULL'";
    execute(cmd3.str());

    for(size_t r=0; r<m_results->data.size(); ++r){
      RVEDV_index.push_back(atoi(m_results->data[r][0].c_str()));
    }
    cmd3.str(std::string());  //clear out the string
  }


  execute("begin");

  // reportMeterData
  for(size_t r=0; r<time_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportMeterData where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }
  // reportMeterExtendedData
  for(size_t r=0; r<RVED_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportMeterExtendedData where ReportMeterExtendedDataIndex = " << RVED_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }

  // reportVariableData
  for(size_t r=0; r<time_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportVariableData where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }
  // reportVariableExtendedData
  for(size_t r=0; r<RVEDV_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from reportVariableExtendedData where ReportVariableExtendedDataIndex = " << RVEDV_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }

  // Time
  for(size_t r=0; r<time_index.size(); ++r){
    //std::stringstream cmd2;
    cmd2.str(std::string());  //clear out the string
    cmd2 << "delete from Time where TimeIndex = " << time_index[r];
    execute(cmd2.str());
    //std::cout << cmd2.str() << std::endl;
    //print();
  }

  execute("commit");

  return true;
}





