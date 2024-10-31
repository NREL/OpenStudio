/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLFILES_IMPL_HPP
#define MODEL_OUTPUTCONTROLFILES_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputControlFiles_Impl is a ModelObject_Impl that is the implementation class for OutputControlFiles.*/
    class MODEL_API OutputControlFiles_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OutputControlFiles_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputControlFiles_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputControlFiles_Impl(const OutputControlFiles_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputControlFiles_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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

      bool outputSpaceSizing() const;

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

      bool setOutputSpaceSizing(bool outputSpaceSizing);

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
     private:
      REGISTER_LOGGER("openstudio.model.OutputControlFiles");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLFILES_IMPL_HPP
