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

#ifndef SHAREDGUICOMPONENTS_COMPONENT_HPP
#define SHAREDGUICOMPONENTS_COMPONENT_HPP

#include "../utilities/bcl/BCL.hpp"
#include "../utilities/data/Attribute.hpp"

#include <QAbstractButton>

class QCheckBox;
class QLabel;

namespace openstudio {

class BCLMeasure;
class BCLSearchResult;

class Component : public QAbstractButton
{
  Q_OBJECT

public:
  Component(const BCLMeasure & bclMeasure,
    bool showAbridgedView = true,
    bool showCheckBox = true,
    QWidget * parent = nullptr);
  Component(const BCLSearchResult & bclSearchResult,
    bool showAbridgedView = true,
    bool showCheckBox = true,
    QWidget * parent = nullptr);
  Component(bool showAbridgedView = false,
    bool showCheckBox = false,
    QWidget * parent = nullptr);
  Component(const Component & other);
  virtual ~Component();
  Component & operator=(const Component & other);
  QCheckBox * checkBox();
  QLabel * msg();
  std::string uid() const;
  std::string versionId() const;
  void setChecked(bool checked);
  
protected:
  void paintEvent(QPaintEvent * event) override;

private:
  void createAbridgedLayout();
  void createCompleteLayout();
  void parseBCLMeasure(const BCLMeasure & bclMeasure);
  void parseBCLSearchResult(const BCLSearchResult & bclSearchResult);

  QString m_componentType;
  QString m_name;
  QString m_uid;
  QString m_versionId;
  QString m_description;
  QString m_fidelityLevel;
  boost::optional<std::string> m_error;
  std::vector<Attribute> m_attributes;
  std::vector<BCLMeasureArgument> m_arguments;
  std::vector<BCLFile> m_files;
  std::vector<BCLFileReference> m_fileReferences;
  std::vector<BCLProvenance> m_provenances;
  std::vector<std::string> m_tags;
  bool m_showAbridgedView;
  bool m_showCheckBox;
  bool m_available;
  bool m_updateAvailable;
  QCheckBox* m_checkBox;
  QLabel* m_msg;

signals:

private slots:

};

} // namespace openstudio

#endif // SHAREDGUICOMPONENTS_COMPONENT_HPP
