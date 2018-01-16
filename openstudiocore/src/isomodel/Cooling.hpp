/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef ISOMODEL_COOLING_HPP
#define ISOMODEL_COOLING_HPP

namespace openstudio {
namespace isomodel {
  class Cooling
  {
  public:
    double temperatureSetPointOccupied() const  {return _temperatureSetPointOccupied;}
    double temperatureSetPointUnoccupied() const {return _temperatureSetPointUnoccupied;}
    double cop() const {return _cOP;}
    double partialLoadValue() const {return _partialLoadValue;}
    double hvacLossFactor() const {return _hvacLossFactor;}
    double pumpControlReduction() const {return _pumpControlReduction;}

    void setTemperatureSetPointOccupied(double value) {_temperatureSetPointOccupied = value;}
    void setTemperatureSetPointUnoccupied(double value) {_temperatureSetPointUnoccupied = value;}
    void setCOP(double value) {_cOP = value;}
    void setPartialLoadValue(double value) {_partialLoadValue = value;}
    void setHvacLossFactor(double value) {_hvacLossFactor = value;}
    void setPumpControlReduction(double value) {_pumpControlReduction = value;}

  private:
    double _temperatureSetPointOccupied;
    double _temperatureSetPointUnoccupied;
    double _cOP;
    double _partialLoadValue;
    double _hvacLossFactor;
    double _pumpControlReduction;
  };

} // isomodel
} // openstudio
#endif // ISOMODEL_COOLING_HPP

