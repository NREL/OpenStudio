
/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
