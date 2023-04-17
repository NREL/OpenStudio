/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLFILES_HPP
#define MODEL_OUTPUTCONTROLFILES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputControlFiles_Impl;

  }  // namespace detail

  /** OutputControlFiles is a ModelObject that wraps the OpenStudio IDD object 'OS:OutputControl:Files'. */
  class MODEL_API OutputControlFiles : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputControlFiles() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputControlFiles(const OutputControlFiles& other) = default;
    OutputControlFiles(OutputControlFiles&& other) = default;
    OutputControlFiles& operator=(const OutputControlFiles&) = default;
    OutputControlFiles& operator=(OutputControlFiles&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool outputCSV() const;

    bool outputMTR() const;

    bool outputESO() const;

    bool outputEIO() const;

    bool outputTabular() const;

    bool outputSQLite() const;

    bool outputJSON() const;

    bool outputAUDIT() const;

    bool outputZoneSizing() const;

    bool outputSystemSizing() const;

    bool outputDXF() const;

    bool outputBND() const;

    bool outputRDD() const;

    bool outputMDD() const;

    bool outputMTD() const;

    bool outputSHD() const;

    bool outputDFS() const;

    bool outputGLHE() const;

    bool outputDelightIn() const;

    bool outputDelightELdmp() const;

    bool outputDelightDFdmp() const;

    bool outputEDD() const;

    bool outputDBG() const;

    bool outputPerfLog() const;

    bool outputSLN() const;

    bool outputSCI() const;

    bool outputWRL() const;

    bool outputScreen() const;

    bool outputExtShd() const;

    bool outputTarcog() const;

    //@}
    /** @name Setters */
    //@{

    bool setOutputCSV(bool outputCSV);

    bool setOutputMTR(bool outputMTR);

    bool setOutputESO(bool outputESO);

    bool setOutputEIO(bool outputEIO);

    bool setOutputTabular(bool outputTabular);

    bool setOutputSQLite(bool outputSQLite);

    bool setOutputJSON(bool outputJSON);

    bool setOutputAUDIT(bool outputAUDIT);

    bool setOutputZoneSizing(bool outputZoneSizing);

    bool setOutputSystemSizing(bool outputSystemSizing);

    bool setOutputDXF(bool outputDXF);

    bool setOutputBND(bool outputBND);

    bool setOutputRDD(bool outputRDD);

    bool setOutputMDD(bool outputMDD);

    bool setOutputMTD(bool outputMTD);

    bool setOutputSHD(bool outputSHD);

    bool setOutputDFS(bool outputDFS);

    bool setOutputGLHE(bool outputGLHE);

    bool setOutputDelightIn(bool outputDelightIn);

    bool setOutputDelightELdmp(bool outputDelightELdmp);

    bool setOutputDelightDFdmp(bool outputDelightDFdmp);

    bool setOutputEDD(bool outputEDD);

    bool setOutputDBG(bool outputDBG);

    bool setOutputPerfLog(bool outputPerfLog);

    bool setOutputSLN(bool outputSLN);

    bool setOutputSCI(bool outputSCI);

    bool setOutputWRL(bool outputWRL);

    bool setOutputScreen(bool outputScreen);

    bool setOutputExtShd(bool outputExtShd);

    bool setOutputTarcog(bool outputTarcog);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputControlFiles_Impl;

    explicit OutputControlFiles(std::shared_ptr<detail::OutputControlFiles_Impl> impl);

    friend class detail::OutputControlFiles_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputControlFiles(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputControlFiles");
  };

  /** \relates OutputControlFiles*/
  using OptionalOutputControlFiles = boost::optional<OutputControlFiles>;

  /** \relates OutputControlFiles*/
  using OutputControlFilesVector = std::vector<OutputControlFiles>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLFILES_HPP
