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
#ifndef SIMTXT_H
#define SIMTXT_H

#include <QString>
#include <QVector>
#include <QFile>

#include <utilities/data/TimeSeries.hpp>
#include <utilities/core/Path.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
  //class TimeSeries;
  class DateTime;
  //class Vector;
}

namespace openstudio{ 
  namespace contam {
    namespace sim {
      class Results
      {
      public:

        // we would normally use openstudio::path for file path's
        virtual bool read(openstudio::path path)=0;
        virtual void clear();

        std::vector<int> nr(){return m_nr;}
        std::vector<DateTime> dateTimes(){return m_dateTimes;}

      protected:
        bool computeDateTimes(QVector<QString> day, QVector<QString> time);

        std::vector<int> m_nr;  // nr is the CONTAM index
        std::vector<DateTime> m_dateTimes;
      };

      class CONTAM_API LinkFlow : public Results
      {
      public:
        bool read(QString fileName);
        bool read(openstudio::path path);
        void clear();

        // i'm not sure what the vector of vectors is for?  we would prefer to use openstudio::TimeSeries if possible, 
        // we also have a class openstudio::Vector for doing linear algebra.  finally if this is what you really want i'd
        // use std::vector since it swig's well and also does implicit copying
        QVector<QVector<double> > dP(){return m_dP;}
        QVector<QVector<double> > F0(){return m_F0;}
        QVector<QVector<double> > F1(){return m_F1;}

        openstudio::TimeSeries deltaP(int nr);

      protected:
        QVector<QVector<double> > m_dP;
        std::vector<openstudio::Vector > m_deltaP;
        QVector<QVector<double> > m_F0;
        QVector<QVector<double> > m_F1;
      };

      /*
      class Contaminants : public Results
      {
      public:
      bool read(QString fileName);
      void clear();
      //QVector<double> integrate();

      QVector<QVector<double> > mf(){return m_mf;}

      protected:
      QVector<QVector<double> > m_mf;
      };

      class NodeFlow : public Results
      {
      public:
      bool read(QString fileName);
      void clear();

      QVector<QVector<double> > T(){return m_T;}
      QVector<QVector<double> > P(){return m_P;}
      QVector<QVector<double> > D(){return m_D;}

      protected:
      QVector<QVector<double> > m_T;
      QVector<QVector<double> > m_P;
      QVector<QVector<double> > m_D;
      };
      */
    }
  }
}

#endif // SIMTXT_H
