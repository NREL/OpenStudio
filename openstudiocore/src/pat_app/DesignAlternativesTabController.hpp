/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
#define PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP

#include "../shared_gui_components/OSListView.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/RubyMeasure.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>

namespace openstudio{

namespace measuretab {

class VariableGroupListController;
class VariableGroupItemDelegate;

} // measuretab
  
namespace pat {

class DesignAlternativesTabView;


namespace altstab {

class AltsTabVariableGroupItemDelegate;
class DesignAltListController;
class DesignAltItemDelegate;
class PerturbationListController;
class AlternativeModelMeasureListController;
class AlternativeModelMeasureItem;

} // altstab

class DesignAlternativesTabController : public QObject
{
  Q_OBJECT

  public:

  QPointer<DesignAlternativesTabView> designAlternativesTabView;

  DesignAlternativesTabController();

  virtual ~DesignAlternativesTabController();

  private slots:

  // Schedule an update for the buttons that are active or inactive based on selections.
  // This method uses a single shot timer in a way that the buttons only get updated once per event loop.
  // Calls updateButtonStatusBasedOnSelectionNow() and the expiration of the timer.
  void updateButtonStatusBasedOnSelection();

  // Update the buttons that are active or inactive based on selections.
  void updateButtonStatusBasedOnSelectionNow();

  private:

  QSharedPointer<measuretab::VariableGroupListController> m_variableGroupListController;
  QSharedPointer<altstab::AltsTabVariableGroupItemDelegate> m_altsTabVariableGroupItemDelegate;
  QSharedPointer<altstab::DesignAltListController> m_designAltListController;
  QSharedPointer<altstab::DesignAltItemDelegate> m_designAltItemDelegate;

  bool m_isButtonStateDirty;
};

namespace altstab {

// Many of these classes are alternatives to the classes in \openstudiocore\src\shared_gui_components\VariableList.hpp
// If possible, we should share the classes in shared_gui_components to these
// These classes are only used on the design alternatives tab

// AltsTabVariableGroupItemDelegate views a VariableGroupItem and returns an AltsTabVariableGroupItemView
class AltsTabVariableGroupItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:
    
  virtual ~AltsTabVariableGroupItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// AltsTabVariableItemDelegate views a VariableItem and returns an AltsTabVariableItemView
class AltsTabVariableItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~AltsTabVariableItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// AltsTabMeasureItemDelegate views a MeasureItem and returns an AltsTabMeasureItemView
class AltsTabMeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~AltsTabMeasureItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// DesignAltListController controls a list of DesignAltItems
// It can add or remove analysis::Datapoints
class DesignAltListController : public OSListController
{
  Q_OBJECT

  public:

  DesignAltListController(QSharedPointer<OSItemSelectionController> measureSelectionController);

  virtual ~DesignAltListController() {}

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  void removeItemForDataPoint(analysis::DataPoint datapoint);

  public slots:

  void addOneItemForEachSelectedMeasure();

  void addOneItemWithAllSelectedMeasures();

  void addItemFromExternalFile();

  private:

  std::vector<analysis::DataPoint> dataPoints() const;

  std::string suggestDesignAltName(const boost::optional<analysis::Measure>& measure) const;

  QSharedPointer<OSItemSelectionController> m_measureSelectionController;
};

// DesignAltItem corresponds to an analysis::DataPoint
class DesignAltItem : public OSListItem
{
  Q_OBJECT

  public:

  DesignAltItem(const analysis::DataPoint & dataPoint, bool isBaseline, bool isAlternativeModel, bool alternateModelMeasureNeedsUpdate);

  virtual ~DesignAltItem() {}

  QString name() const;

  QString description() const;

  analysis::DataPoint dataPoint() const;

  bool isBaseline() const;

  bool isAlternativeModel() const;

  bool alternateModelMeasureNeedsUpdate() const;

  QSharedPointer<PerturbationListController> perturbationListController() const;

  QSharedPointer<AlternativeModelMeasureListController> alternativeModelMeasureListController() const;

  public slots:

  void setName(const QString & name);

  void setDescription(const QString & description);

  void remove();

  private:

  QSharedPointer<PerturbationListController> m_perturbationListController;

  QSharedPointer<AlternativeModelMeasureListController> m_alternativeModelMeasureListController;

  analysis::DataPoint m_dataPoint;

  bool m_isBaseline;

  bool m_isAlternativeModel;

  bool m_alternateModelMeasureNeedsUpdate;
};

// DesignAltItemDelegate views a DesignAltItem and returns an DesignAltItemView
class DesignAltItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  public:

  virtual ~DesignAltItemDelegate() {}
};

// These classes are the list controller and related classes for the perturbations associated with a design point.

// PerturbationListController gets the perturbations (e.g. Measures) for a DesignAltItem
class PerturbationListController : public OSListController
{
  Q_OBJECT

  public:

  PerturbationListController(DesignAltItem * designAltItem);
  
  virtual ~PerturbationListController() {}

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  private:

  std::vector<analysis::Measure> measures() const;

  QPointer<DesignAltItem> m_designAltItem;
};

// PerturbationItem corresponds to an analysis::Measure
class PerturbationItem : public OSListItem
{
  Q_OBJECT

  public:

  PerturbationItem(const analysis::Measure & measure);
  
  virtual ~PerturbationItem() {}

  QString name() const;
  bool isFixedMeasureItem() const;

  private:
  boost::optional<analysis::MeasureGroup> measureGroupParent() const;

  analysis::Measure m_measure;
};

// PerturbationItemDelegate views a PerturbationItem and returns a QLabel
class PerturbationItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  public:

  virtual ~PerturbationItemDelegate() {}
};

// These classes are the list controller and related classes for the user defined measures associated with an alternative model,
// i.e. a model swapped into the workflow using the alternativeModelVariable.

// AlternativeModelMeasureListController gets the user defined measures for an AlternativeModel DesignAltItem
class AlternativeModelMeasureListController : public OSListController
{
  Q_OBJECT

public:

  AlternativeModelMeasureListController(DesignAltItem * designAltItem);

  virtual ~AlternativeModelMeasureListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

public slots:

  void addAlternativeModelMeasure();

  void alternativeModelMeasureItemChanged();

  void alternativeModelMeasureItemRemoved();

private slots:

  void emitModelReset();

private:

  boost::optional<analysis::RubyMeasure> rubySwapMeasure() const;

  // the current swap measure arguments that represent user defined model measures
  QJsonArray modelMeasures() const;

  void setModelMeasures(const QJsonArray& modelMeasures, bool forceRefresh);

  std::vector<QSharedPointer<AlternativeModelMeasureItem> > alternativeModelMeasureItems() const;

  QPointer<DesignAltItem> m_designAltItem;

  int m_count;
};

// AlternativeModelMeasureItem corresponds to a user defined measure on an alternative model
class AlternativeModelMeasureItem : public OSListItem
{
  Q_OBJECT

public:

  AlternativeModelMeasureItem(const QString& uuid,
                              const QString& displayName,
                              const QString& description,
                              const QString& taxonomyTag,
                              double capitalCost);

  virtual ~AlternativeModelMeasureItem() {}

  QString uuid() const;
  QString displayName() const;
  QString description() const;
  QString taxonomyTag() const;
  double capitalCost() const;

public slots:

  void viewChanged();

signals:

  void changed();

  void removed();

private:

  QString m_uuid;
  QString m_displayName;
  QString m_description;
  QString m_taxonomyTag;
  double m_capitalCost;
};

// AlternativeModelMeasureItemDelegate views an AlternativeModelMeasureItem and returns an AlternativeModelMeasureItemView
class AlternativeModelMeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

    QWidget * view(QSharedPointer<OSListItem> dataSource);

public:

  virtual ~AlternativeModelMeasureItemDelegate() {}
};



} // altstab

} // pat

} // openstudio

#endif // PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
