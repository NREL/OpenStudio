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

#ifndef OPENSTUDIO_UTILITYBILLALLFUELTYPESLISTVIEW_HPP
#define OPENSTUDIO_UTILITYBILLALLFUELTYPESLISTVIEW_HPP

#include "OSCollapsibleItemList.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include "../utilities/data/DataEnums.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class FuelType;

class UtilityBillAllFuelTypesListView : public OSCollapsibleItemList
{
  Q_OBJECT

  public:
    UtilityBillAllFuelTypesListView(const model::Model& model, 
                            bool addScrollArea, 
                            OSItemType headerType,
                            QWidget * parent = nullptr);

    UtilityBillAllFuelTypesListView(const std::vector<std::pair<FuelType, std::string> >& utilityBillFuelTypesAndNames,
                            const model::Model& model,
                            bool addScrollArea,
                            OSItemType headerType,
                            QWidget * parent = nullptr);

    virtual ~UtilityBillAllFuelTypesListView() {}

    void addUtilityBillFuelType(const FuelType & fuelType, const std::string& name);

    void addModelObjectType(const IddObjectType& iddObjectType, const std::string& name);

    FuelType currentFuelType() const;

    boost::optional<openstudio::FuelType> selectedFuelType() const;

    virtual boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  private:

    std::vector<std::pair<FuelType, std::string> > m_utilityBillFuelTypesAndNames;

    model::Model m_model;
    OSItemType m_headerType;
    bool m_showLocalBCL;
};



} // openstudio

#endif // OPENSTUDIO_UTILITYBILLALLFUELTYPESLISTVIEW_HPP

