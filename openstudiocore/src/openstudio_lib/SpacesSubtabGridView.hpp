
/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_SPACESSUBTABGRIDVIEW_HPP
#define OPENSTUDIO_SPACESSUBTABGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "GridViewSubTab.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

class QComboBox;
class QLineEdit;

namespace openstudio{

  class SpacesSubsurfacesGridController;

  class SpacesSubtabGridView : public GridViewSubTab
  {
    Q_OBJECT

  public:

    SpacesSubtabGridView(bool isIP, const model::Model & model, QWidget * parent = nullptr);

    virtual ~SpacesSubtabGridView() {}

    QLineEdit *  m_thermalZoneNameFilter = nullptr;

    QLineEdit *  m_spaceNameFilter = nullptr;

    QComboBox *  m_storyFilter = nullptr;

    QComboBox *  m_thermalZoneFilter = nullptr;

    QComboBox *  m_spaceTypeFilter = nullptr;

    QComboBox *  m_loadTypeFilter = nullptr;

    QComboBox *  m_windExposureFilter = nullptr;

    QComboBox *  m_sunExposureFilter = nullptr;

    QComboBox *  m_outsideBoundaryConditionFilter = nullptr;

    QComboBox *  m_surfaceTypeFilter = nullptr;

    QComboBox *  m_interiorPartitionGroupFilter = nullptr;
    
  protected:

    REGISTER_LOGGER("openstudio.SpacesSubtabGridView");

    virtual void addObject(const openstudio::IddObjectType& iddObjectType);

    virtual void purgeObjects(const openstudio::IddObjectType& iddObjectType);

    void showStoryFilter();

    void showThermalZoneFilter();

    void showSpaceTypeFilter();

    void showSpaceNameFilter();

    void showWindExposureFilter();

    void showSunExposureFilter();

    void showOutsideBoundaryConditionFilter();

    void showSurfaceTypeFilter();

    void showInteriorPartitionGroupFilter();

    void showLoadTypeFilter();

    void initializeStoryFilter();

    void initializeThermalZoneFilter();

    void initializeSpaceTypeFilter();

    void initializeLoadTypeFilter();

    void initializeWindExposureFilter();

    void initializeSunExposureFilter();

    void initializeOutsideBoundaryConditionFilter();

    void initializeSurfaceTypeFilter();

    void initializeInteriorPartitionGroupFilter();

    void filterChanged();

    std::set<openstudio::model::ModelObject> m_objectsFilteredByStory;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByThermalZone;

    std::set<openstudio::model::ModelObject> m_objectsFilterdBySpaceType;

    std::set<openstudio::model::ModelObject> m_objectsFilteredBySpaceName;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByWindExposure;

    std::set<openstudio::model::ModelObject> m_objectsFilteredBySunExposure;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByOutsideBoundaryCondition;

    std::set<openstudio::model::ModelObject> m_objectsFilteredBySurfaceType;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByInteriorPartitionGroup;

    QGridLayout * m_filterGridLayout = nullptr;

    std::vector<model::ModelObject> m_spacesModelObjects = std::vector<model::ModelObject>();

  protected slots:

    virtual void spaceNameFilterChanged();

    virtual void storyFilterChanged(const QString & text);

    virtual void thermalZoneFilterChanged(const QString & text);

    virtual void spaceTypeFilterChanged(const QString & text);

    virtual void loadTypeFilterChanged(const QString & text);

    virtual void windExposureFilterChanged(const QString & text);

    virtual void sunExposureFilterChanged(const QString & text);

    virtual void outsideBoundaryConditionFilterChanged(const QString & text);

    virtual void surfaceTypeFilterChanged(const QString & text);

    virtual void interiorPartitionGroupFilterChanged(const QString & text);

  };

} // openstudio

#endif // OPENSTUDIO_SPACESSUBTABGRIDVIEW_HPP
