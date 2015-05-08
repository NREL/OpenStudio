
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

#ifndef OPENSTUDIO_SPACESSUBSURFACESGRIDVIEW_HPP
#define OPENSTUDIO_SPACESSUBSURFACESGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "GridViewSubTab.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

class QComboBox;
class QLineEdit;

namespace openstudio{

  class SpacesSubsurfacesGridController;

  class SpacesSubsurfacesGridView : public GridViewSubTab
  {
    Q_OBJECT

  public:

    SpacesSubsurfacesGridView(bool isIP, const model::Model & model, QWidget * parent = nullptr);

    virtual ~SpacesSubsurfacesGridView() {}

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
    
  private:

    REGISTER_LOGGER("openstudio.SpacesSubsurfacesGridView");

    virtual void addObject(const openstudio::IddObjectType& iddObjectType);

    virtual void purgeObjects(const openstudio::IddObjectType& iddObjectType);

    void showStoryFilter() { m_showStoryFilter = true; }

    void showThermalZoneFilter() { m_showThermalZoneFilter = true; }

    void showSpaceTypeFilter() { m_showSpaceTypeFilter = true; }

    void showSpaceNameFilter() { m_showSpaceNameFilter = true; }

    void showThermalZoneNameFilter() { m_showThermalZoneNameFilter = true; }

    void showWindExposureFilter() { m_showWindExposureFilter = true; }

    void showSunExposureFilter() { m_showSunExposureFilter = true; }

    void showOutsideBoundaryConditionFilter() { m_showOutsideBoundaryConditionFilter = true; }

    void showSurfaceTypeFilter() { m_showSurfaceTypeFilter = true; }

    void showInteriorPartitionGroupFilter() { m_showInteriorPartitionGroupFilter = true; }

    void showLoadTypeFilter() { m_showLoadTypeFilter = true; }

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

    std::set<openstudio::model::ModelObject> m_objectsFilteredByThermalZoneName;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByWindExposure;

    std::set<openstudio::model::ModelObject> m_objectsFilteredBySunExposure;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByOutsideBoundaryCondition;

    std::set<openstudio::model::ModelObject> m_objectsFilteredBySurfaceType;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByInteriorPartitionGroup;

    bool m_showStoryFilter = false;

    bool m_showThermalZoneFilter = false;

    bool m_showSpaceTypeFilter = false;

    bool m_showSpaceNameFilter = false;

    bool m_showThermalZoneNameFilter = false;

    bool m_showWindExposureFilter = false;

    bool m_showSunExposureFilter = false;

    bool m_showOutsideBoundaryConditionFilter = false;

    bool m_showSurfaceTypeFilter = false;

    bool m_showInteriorPartitionGroupFilter = false;

    bool m_showLoadTypeFilter = false;

  private slots:

    void onDropZoneItemClicked(OSItem* item);

    void spaceNameFilterChanged();

    void thermalZoneNameFilterChanged();

    void storyFilterChanged(const QString & text);

    void thermalZoneFilterChanged(const QString & text);

    void spaceTypeFilterChanged(const QString & text);

    void loadTypeFilterChanged(const QString & text);

    void windExposureFilterChanged(const QString & text);

    void sunExposureFilterChanged(const QString & text);

    void outsideBoundaryConditionFilterChanged(const QString & text);

    void surfaceTypeFilterChanged(const QString & text);

    void interiorPartitionGroupFilterChanged(const QString & text);

  };

  class SpacesSubsurfacesGridController : public OSGridController
  {

    Q_OBJECT

  public:

    SpacesSubsurfacesGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~SpacesSubsurfacesGridController() {}

    virtual void refreshModelObjects();

    virtual void categorySelected(int index);

  protected:

    virtual void setCategoriesAndFields();

    virtual void addColumns(const QString &category, std::vector<QString> & fields);

    virtual void checkSelectedFields();

    virtual QString getColor(const model::ModelObject & modelObject);

  public slots:

    virtual void onItemDropped(const OSItemId& itemId);

  };

} // openstudio

#endif // OPENSTUDIO_SPACESSUBSURFACESGRIDVIEW_HPP

