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

    VECTOR_TYPE<int> contaminants() const
    {
        return m_contaminants;
    }

    VECTOR_TYPE <Species> species() const
    {
        return m_species;
    }
    void setSpecies(const VECTOR_TYPE<Species> species)
    {
        m_species = species;
        rebuildContaminants();
    }

    VECTOR_TYPE <Level> levels() const
    {
        return m_levels;
    }
    void setLevels(const VECTOR_TYPE<Level> levels)
    {
        m_levels = levels;
    }

    VECTOR_TYPE <DaySchedule> daySchedules() const
    {
        return m_daySchedules;
    }
    void setDaySchedules(const VECTOR_TYPE<DaySchedule> daySchedules)
    {
        m_daySchedules = daySchedules;
    }

    VECTOR_TYPE <WeekSchedule> weekSchedules() const
    {
        return m_weekSchedules;
    }
    void setWeekSchedules(const VECTOR_TYPE<WeekSchedule> weekSchedules)
    {
        m_weekSchedules = weekSchedules;
    }

    VECTOR_TYPE <WindPressureProfile> windPressureProfiles() const
    {
        return m_windPressureProfiles;
    }
    void setWindPressureProfiles(const VECTOR_TYPE<WindPressureProfile> windPressureProfiles)
    {
        m_windPressureProfiles = windPressureProfiles;
    }

    VECTOR_TYPE<PlrTest1> getPlrTest1()
    {
        VECTOR_TYPE<PlrTest1> afe;
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

    int airflowElementNrByName(STRING name) const;

    VECTOR_TYPE<CdvDat> getCdvDat()
    {
        VECTOR_TYPE<CdvDat> ctrl;
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

    VECTOR_TYPE <Ahs> ahs() const
    {
        return m_ahs;
    }
    void setAhs(const VECTOR_TYPE<Ahs> ahs)
    {
        m_ahs = ahs;
    }

    VECTOR_TYPE<Zone> zones() const
    {
        return m_zones;
    }
    void setZones(const VECTOR_TYPE<Zone> zones)
    {
        m_zones = zones;
    }

    VECTOR_TYPE<Path> paths() const
    {
        return m_paths;
    }
    void setPaths(const VECTOR_TYPE<Path> paths)
    {
        m_paths = paths;
    }

    bool valid() const
    {
      return m_valid;
    }

private:
    void rebuildContaminants();
    void readZoneIc(Reader &input);
    STRING writeZoneIc(int start=0);
    template <class T> STRING writeSectionVector(VECTOR_TYPE<T> vector, STRING label=STRING_INIT, int start=0);
    template <class T, template <class T> class U> STRING writeSectionVector(U<QSharedPointer<T> > vector,
                                                                             STRING label=STRING_INIT,
                                                                             int start=0);

    bool m_valid;

    MAP_TYPE<STRING,STRING> m_unsupported;

    RunControl m_rc;
    VECTOR_TYPE<int> m_contaminants;
    VECTOR_TYPE<Species> m_species;
    VECTOR_TYPE<Level> m_levels;
    VECTOR_TYPE<DaySchedule> m_daySchedules;
    VECTOR_TYPE<WeekSchedule> m_weekSchedules;
    VECTOR_TYPE<WindPressureProfile> m_windPressureProfiles;
    QVector<QSharedPointer<AirflowElement> > m_airflowElements;
    QVector<QSharedPointer<ControlNode> > m_controlNodes;
    VECTOR_TYPE<Ahs> m_ahs;
    VECTOR_TYPE<Zone> m_zones;
    VECTOR_TYPE<Path> m_paths;
};

template <class T> STRING Model::writeSectionVector(VECTOR_TYPE<T> vector, STRING label, int start)
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
