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
#ifndef PRJMODEL_H
#define PRJMODEL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjPublic.hpp"

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {
namespace prj {

class CONTAM_API Model
{
public:
    Model(){m_valid=false;}
    explicit Model(STRING filename);
    explicit Model(Reader &input);
    bool read(STRING filename);
    bool read(Reader &input);
    STRING toString();

    RunControl rc() const
    {
        return m_rc;
    }
    void setRc(const RunControl rc)
    {
        m_rc = rc;
    }

    VECTOR<int> contaminants() const
    {
        return m_contaminants;
    }

    VECTOR <Species> species() const
    {
        return m_species;
    }
    void setSpecies(const VECTOR<Species> species)
    {
        m_species = species;
        rebuildContaminants();
    }

    VECTOR <Level> levels() const
    {
        return m_levels;
    }
    void setLevels(const VECTOR<Level> levels)
    {
        m_levels = levels;
    }

    VECTOR <DaySchedule> daySchedules() const
    {
        return m_daySchedules;
    }
    void setDaySchedules(const VECTOR<DaySchedule> daySchedules)
    {
        m_daySchedules = daySchedules;
    }

    VECTOR <WeekSchedule> weekSchedules() const
    {
        return m_weekSchedules;
    }
    void setWeekSchedules(const VECTOR<WeekSchedule> weekSchedules)
    {
        m_weekSchedules = weekSchedules;
    }

    VECTOR <WindPressureProfile> windPressureProfiles() const
    {
        return m_windPressureProfiles;
    }
    void setWindPressureProfiles(const VECTOR<WindPressureProfile> windPressureProfiles)
    {
        m_windPressureProfiles = windPressureProfiles;
    }

    VECTOR<PlrTest1> getPlrTest1()
    {
        VECTOR<PlrTest1> afe;
        for(int i=0;i<m_airflowElements.size();i++)
        {
          if(m_airflowElements[i]->dataType() == "plr_test1")
            {
                afe.push_back(*(m_airflowElements[i].dynamicCast<PlrTest1>().data()));
            }
        }
        return afe;
    }

//    template <class T> void addAirflowElement(T element);
//    {
//        T *copy = new T;
//        *copy = element;
//        copy->setNr(m_airflowElements.size()+1);
//        m_airflowElements.push_back(QSharedPointer<AirflowElement>((AirflowElement*)copy));
//    }

    VECTOR<CdvDat> getCdvDat()
    {
        VECTOR<CdvDat> ctrl;
        for(int i=0;i<m_controlNodes.size();i++)
        {
            QSharedPointer<CdvDat> cast = m_controlNodes[i].dynamicCast<CdvDat>();
            if(!cast.isNull())
            {
                ctrl.push_back(*(cast.data()));
            }
        }
        return ctrl;
    }

    template <class T> void addControlNode(T element)
    {
        T *copy = new T;
        *copy = element;
        ControlNode *pointer = dynamic_cast<ControlNode*>(copy);
        if(pointer)
        {
            copy->setNr(m_controlNodes.size()+1);
            m_controlNodes.push_back(QSharedPointer<ControlNode>(pointer));
        }
    }

    VECTOR <Ahs> ahs() const
    {
        return m_ahs;
    }
    void setAhs(const VECTOR<Ahs> ahs)
    {
        m_ahs = ahs;
    }

    VECTOR<Zone> zones() const
    {
        return m_zones;
    }
    void setZones(const VECTOR<Zone> zones)
    {
        m_zones = zones;
    }

    bool valid() const
    {
      return m_valid;
    }

private:
    void rebuildContaminants();
    void readZoneIc(Reader &input);
    STRING writeZoneIc(int start=0);
    template <class T> STRING writeSectionVector(VECTOR<T> vector, STRING label=STRING_INIT, int start=0);
    template <class T, template <class T> class U> STRING writeSectionVector(U<QSharedPointer<T> > vector,
                                                                             STRING label=STRING_INIT,
                                                                             int start=0);

    bool m_valid;

    MAP<STRING,STRING> m_unsupported;

    RunControl m_rc;
    VECTOR<int> m_contaminants;
    VECTOR<Species> m_species;
    VECTOR<Level> m_levels;
    VECTOR<DaySchedule> m_daySchedules;
    VECTOR<WeekSchedule> m_weekSchedules;
    VECTOR<WindPressureProfile> m_windPressureProfiles;
    QVector<QSharedPointer<AirflowElement> > m_airflowElements;
    QVector<QSharedPointer<ControlNode> > m_controlNodes;
    VECTOR<Ahs> m_ahs;
    VECTOR<Zone> m_zones;
    VECTOR<Path> m_paths;
};

template <class T> STRING Model::writeSectionVector(VECTOR<T> vector, STRING label, int start)
{
    STRING string;
    int number = vector.size()-start;
    if(IS_NULL(label))
    {
        string += TO_STRING(number) + '\n';
    }
    else
    {
        string += TO_STRING(number) + " ! " + label + '\n';
    }
    for(unsigned int i=start;i<vector.size();i++)
    {
        string += vector[i].write();
    }
    string += "-999\n";
    return string;
}

template <class T, template <class T> class U> STRING Model::writeSectionVector(U<QSharedPointer<T> > vector,
                                                                                STRING label, int start)
{
    STRING string;
    int number = vector.size()-start;
    if(IS_NULL(label))
    {
        string += TO_STRING(number) + '\n';
    }
    else
    {
        string += TO_STRING(number) + " ! " + label + '\n';
    }
    for(int i=start;i<vector.size();i++)
    {
        string += vector[i]->write();
    }
    string += "-999\n";
    return string;
}

} // prj
} // contam
} // openstudio

#endif // PRJMODEL_H
