/**********************************************************************
 *  Copyright (c) 2013, The Pennsylvania State University.
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
#include "SimTxt.hpp"

#include <QTextStream>
#include <QStringList>
#include <QDateTime>

//#include <utilities/data/TimeSeries.hpp>
#include <utilities/time/DateTime.hpp>
//#include <utilities/time/Date.hpp>
//#include <utilities/time/Time.hpp>

namespace openstudio{ 
  namespace contam {
    namespace sim {

      // Generate time series-able representation of the data
      Vector convertToVector(std::vector<double> in)
      {
        Vector out(in.size());
        for(unsigned i=0;i<in.size();i++)
          out[i] = in[i];
        return out;
      }

      void Results::clear()
      {
        m_nr.clear();
        m_dateTimes.clear();
      }

      bool Results::computeDateTimes(QVector<QString> day, QVector<QString> time)
      {
        unsigned month,dayOfMonth;
        bool ok;
        QStringList split;
        int n = qMin(day.size(),time.size());
        for(int i=0;i<n;i++)
        {
          split = day[i].split('/');
          if(split.size() != 2)
            return false;
          month=split[0].toInt(&ok);
          if(!ok || month > 12)return false;
          dayOfMonth=split[1].toInt(&ok);
          if(!ok)return false;

          m_dateTimes.push_back(DateTime(Date(monthOfYear(month),dayOfMonth,2013),Time(time[i].toStdString())));
        }
        return true;
      }

      void LinkFlow::clear()
      {
        Results::clear();
        m_dP.clear();
        m_F0.clear();
        m_F1.clear();
      }

      bool LinkFlow::read(openstudio::path path)
      {
        return read(openstudio::toQString(path));
      }

      bool LinkFlow::read(QString fileName)
      {
        clear();
        QVector<QString> day;
        QVector<QString> time;
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly))
          return false;
        QTextStream textStream(&file);
        QMap<int,int> nrMap;
        // Read the header
        QString header = textStream.readLine();
        if(header.isNull())
          return false;
        QStringList row = header.split('\t');
        int ncols = 6;
        if(row.size() != ncols)
          return false;
        // Read the data
        QString line;
        while(!(line=textStream.readLine()).isNull())
        {
          QStringList row = line.split('\t');
          if(row.size() != ncols)
          {
            clear();
            return false;
          }
          if(time.size())
          {
            if(time[time.size()-1] != row[1])
            {
              day << row[0];
              time << row[1];
            }
          }
          else
          {
            day << row[0];
            time << row[1];
          }
          bool convOk;
          int nr = row[2].toInt(&convOk);
          if(!convOk)
          {
            clear();
            return false;
          }
          if(!nrMap.contains(nr))
          {
            int sz = nrMap.size();
            nrMap[nr] = sz;
            m_dP.resize(sz+1);
            m_F0.resize(sz+1);
            m_F1.resize(sz+1);
          }
          double dP = row[3].toDouble(&convOk);
          if(!convOk)
          {
            clear();
            return false;
          }
          double F0 = row[4].toDouble(&convOk);
          if(!convOk)
          {
            clear();
            return false;
          }
          double F1 = row[5].toDouble(&convOk);
          if(!convOk)
          {
            clear();
            return false;
          }
          m_dP[nrMap[nr]].push_back(dP);
          m_F0[nrMap[nr]].push_back(F0);
          m_F1[nrMap[nr]].push_back(F1);

        }
        // Unwind the zone number map;
        QList<int> keys = nrMap.keys();
        m_nr.resize(keys.size());
        for(int i=0;i<keys.size();i++)
          m_nr[nrMap[keys[i]]] = keys[i];
        file.close();
        // Compute the required date/time objects
        if(!computeDateTimes(day,time))
        {
          clear();
          return false;
        }
        return true;
      }

      openstudio::TimeSeries LinkFlow::deltaP(int nr)
      {
        return openstudio::TimeSeries(m_dateTimes,convertToVector(m_dP[nr]),"Pa");
      }

      openstudio::TimeSeries LinkFlow::flow0(int nr)
      {
        return openstudio::TimeSeries(m_dateTimes,convertToVector(m_F0[nr]),"kg/s");
      }

      openstudio::TimeSeries LinkFlow::flow1(int nr)
      {
        return openstudio::TimeSeries(m_dateTimes,convertToVector(m_F1[nr]),"kg/s");
      }
      /* Do these later
      void Contaminants::clear()
      {
      Results::clear();
      m_mf.clear();
      }

      bool Contaminants::read(QString fileName)
      {
      bool ok=false;
      clear();
      QFile file(fileName);
      if(file.open(QFile::ReadOnly))
      {
      QTextStream textStream(&file);
      QString line;
      QString header = textStream.readLine();
      if(header.isNull())
      return false;
      QStringList headerList = header.split('\t');
      int ncols = headerList.size();
      for(int i=3;i<ncols;i++)
      {
      m_nr.push_back(headerList[i].toInt(&ok));
      if(!ok)return false;
      }
      // The number of zones is ncols - 3 (date,time,ctm) - 1 (ambient)
      m_mf.resize(ncols-3);
      while(!(line=textStream.readLine()).isNull())
      {
      QStringList row = line.split('\t');
      if(row.size() != ncols)
      {
      clear();
      return false;
      }
      m_day << row[0];
      m_time << row[1];
      for(int i=3;i<ncols;i++)
      m_mf[i-3] << row[i].toDouble();
      }
      file.close();
      if(!computeSeconds())
      m_s.clear();
      ok=true;
      }
      return ok;
      }

      //    QVector<double> Contaminants::integrate()
      //    {
      //        QVector<double> value(m_mf.size());
      //        int n = m_mf[0].size();
      //        for(int i=1;i<m_mf.size();i++)
      //            n = qMin(n,m_mf[i].size());
      //        n = qMin(n,m_s.size());
      //        for(int i=0;i<m_mf.size();i++)
      //        {
      //            value[i]=0.0;
      //            for(int j=1;j<n;j++)
      //            {
      //                value[i] += 0.5*(m_mf[i][j-1]+m_mf[i][j])*(m_s[j]-m_s[j-1]);
      //            }
      //        }
      //        return value;
      //    }

      void NodeFlow::clear()
      {
      Results::clear();
      m_T.clear();
      m_P.clear();
      m_D.clear();
      }

      bool NodeFlow::read(QString fileName)
      {
      clear();
      QFile file(fileName);
      if(!file.open(QFile::ReadOnly))
      return false;
      QTextStream textStream(&file);
      QMap<int,int> nrMap;
      // Read the header
      QString header = textStream.readLine();
      if(header.isNull())
      return false;
      QStringList row = header.split('\t');
      int ncols = 6;
      if(row.size() != ncols && row.size() != ncols+2)
      return false;
      // Read the data
      QString line;
      while(!(line=textStream.readLine()).isNull())
      {
      QStringList row = line.split('\t');
      if(row.size() != ncols && row.size() != ncols+2)
      {
      clear();
      return false;
      }
      if(m_time.size())
      {
      if(m_time[m_time.size()-1] != row[1])
      {
      m_day << row[0];
      m_time << row[1];
      }
      }
      else
      {
      m_day << row[0];
      m_time << row[1];
      }
      bool convOk;
      int nr = row[2].toInt(&convOk);
      if(!convOk)
      {
      clear();
      return false;
      }
      if(!nrMap.contains(nr))
      {
      int sz = nrMap.size();
      nrMap[nr] = sz;
      m_T.resize(sz+1);
      m_P.resize(sz+1);
      m_D.resize(sz+1);
      }
      double T = row[3].toDouble(&convOk);
      if(!convOk)
      {
      clear();
      return false;
      }
      double P = row[4].toDouble(&convOk);
      if(!convOk)
      {
      clear();
      return false;
      }
      double D = row[5].toDouble(&convOk);
      if(!convOk)
      {
      if(nr==0)
      D=0.0;
      else
      {
      clear();
      return false;
      }
      }
      m_T[nrMap[nr]] << T;
      m_P[nrMap[nr]] << P;
      m_D[nrMap[nr]] << D;

      }
      // Unwind the zone number map;
      QList<int> keys = nrMap.keys();
      m_nr.resize(keys.size());
      for(int i=0;i<keys.size();i++)
      m_nr[nrMap[keys[i]]] = keys[i];
      file.close();
      if(!computeSeconds())
      m_s.clear();
      return true;
      }
      */

    }
  }
}
