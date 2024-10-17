/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputControlFiles.hpp"
#include "OutputControlFiles_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_OutputControl_Files_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputControlFiles_Impl::OutputControlFiles_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputControlFiles::iddObjectType());
    }

    OutputControlFiles_Impl::OutputControlFiles_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputControlFiles::iddObjectType());
    }

    OutputControlFiles_Impl::OutputControlFiles_Impl(const OutputControlFiles_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputControlFiles_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputControlFiles_Impl::iddObjectType() const {
      return OutputControlFiles::iddObjectType();
    }

    bool OutputControlFiles_Impl::outputCSV() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputCSV);
    }

    bool OutputControlFiles_Impl::outputMTR() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputMTR);
    }

    bool OutputControlFiles_Impl::outputESO() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputESO);
    }

    bool OutputControlFiles_Impl::outputEIO() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputEIO);
    }

    bool OutputControlFiles_Impl::outputTabular() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputTabular);
    }

    bool OutputControlFiles_Impl::outputSQLite() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSQLite);
    }

    bool OutputControlFiles_Impl::outputJSON() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputJSON);
    }

    bool OutputControlFiles_Impl::outputAUDIT() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputAUDIT);
    }

    bool OutputControlFiles_Impl::outputSpaceSizing() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSpaceSizing);
    }

    bool OutputControlFiles_Impl::outputZoneSizing() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputZoneSizing);
    }

    bool OutputControlFiles_Impl::outputSystemSizing() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSystemSizing);
    }

    bool OutputControlFiles_Impl::outputDXF() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDXF);
    }

    bool OutputControlFiles_Impl::outputBND() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputBND);
    }

    bool OutputControlFiles_Impl::outputRDD() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputRDD);
    }

    bool OutputControlFiles_Impl::outputMDD() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputMDD);
    }

    bool OutputControlFiles_Impl::outputMTD() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputMTD);
    }

    bool OutputControlFiles_Impl::outputSHD() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSHD);
    }

    bool OutputControlFiles_Impl::outputDFS() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDFS);
    }

    bool OutputControlFiles_Impl::outputGLHE() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputGLHE);
    }

    bool OutputControlFiles_Impl::outputDelightIn() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightIn);
    }

    bool OutputControlFiles_Impl::outputDelightELdmp() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightELdmp);
    }

    bool OutputControlFiles_Impl::outputDelightDFdmp() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightDFdmp);
    }

    bool OutputControlFiles_Impl::outputEDD() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputEDD);
    }

    bool OutputControlFiles_Impl::outputDBG() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputDBG);
    }

    bool OutputControlFiles_Impl::outputPerfLog() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputPerfLog);
    }

    bool OutputControlFiles_Impl::outputSLN() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSLN);
    }

    bool OutputControlFiles_Impl::outputSCI() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputSCI);
    }

    bool OutputControlFiles_Impl::outputWRL() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputWRL);
    }

    bool OutputControlFiles_Impl::outputScreen() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputScreen);
    }

    bool OutputControlFiles_Impl::outputExtShd() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputExtShd);
    }

    bool OutputControlFiles_Impl::outputTarcog() const {
      return getBooleanFieldValue(OS_OutputControl_FilesFields::OutputTarcog);
    }

    bool OutputControlFiles_Impl::setOutputCSV(bool outputCSV) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputCSV, outputCSV);
    }

    bool OutputControlFiles_Impl::setOutputMTR(bool outputMTR) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputMTR, outputMTR);
    }

    bool OutputControlFiles_Impl::setOutputESO(bool outputESO) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputESO, outputESO);
    }

    bool OutputControlFiles_Impl::setOutputEIO(bool outputEIO) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputEIO, outputEIO);
    }

    bool OutputControlFiles_Impl::setOutputTabular(bool outputTabular) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputTabular, outputTabular);
    }

    bool OutputControlFiles_Impl::setOutputSQLite(bool outputSQLite) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSQLite, outputSQLite);
    }

    bool OutputControlFiles_Impl::setOutputJSON(bool outputJSON) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputJSON, outputJSON);
    }

    bool OutputControlFiles_Impl::setOutputAUDIT(bool outputAUDIT) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputAUDIT, outputAUDIT);
    }

    bool OutputControlFiles_Impl::setOutputSpaceSizing(bool outputSpaceSizing) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSpaceSizing, outputSpaceSizing);
    }

    bool OutputControlFiles_Impl::setOutputZoneSizing(bool outputZoneSizing) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputZoneSizing, outputZoneSizing);
    }

    bool OutputControlFiles_Impl::setOutputSystemSizing(bool outputSystemSizing) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSystemSizing, outputSystemSizing);
    }

    bool OutputControlFiles_Impl::setOutputDXF(bool outputDXF) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDXF, outputDXF);
    }

    bool OutputControlFiles_Impl::setOutputBND(bool outputBND) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputBND, outputBND);
    }

    bool OutputControlFiles_Impl::setOutputRDD(bool outputRDD) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputRDD, outputRDD);
    }

    bool OutputControlFiles_Impl::setOutputMDD(bool outputMDD) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputMDD, outputMDD);
    }

    bool OutputControlFiles_Impl::setOutputMTD(bool outputMTD) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputMTD, outputMTD);
    }

    bool OutputControlFiles_Impl::setOutputSHD(bool outputSHD) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSHD, outputSHD);
    }

    bool OutputControlFiles_Impl::setOutputDFS(bool outputDFS) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDFS, outputDFS);
    }

    bool OutputControlFiles_Impl::setOutputGLHE(bool outputGLHE) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputGLHE, outputGLHE);
    }

    bool OutputControlFiles_Impl::setOutputDelightIn(bool outputDelightIn) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightIn, outputDelightIn);
    }

    bool OutputControlFiles_Impl::setOutputDelightELdmp(bool outputDelightELdmp) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightELdmp, outputDelightELdmp);
    }

    bool OutputControlFiles_Impl::setOutputDelightDFdmp(bool outputDelightDFdmp) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDelightDFdmp, outputDelightDFdmp);
    }

    bool OutputControlFiles_Impl::setOutputEDD(bool outputEDD) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputEDD, outputEDD);
    }

    bool OutputControlFiles_Impl::setOutputDBG(bool outputDBG) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputDBG, outputDBG);
    }

    bool OutputControlFiles_Impl::setOutputPerfLog(bool outputPerfLog) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputPerfLog, outputPerfLog);
    }

    bool OutputControlFiles_Impl::setOutputSLN(bool outputSLN) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSLN, outputSLN);
    }

    bool OutputControlFiles_Impl::setOutputSCI(bool outputSCI) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputSCI, outputSCI);
    }

    bool OutputControlFiles_Impl::setOutputWRL(bool outputWRL) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputWRL, outputWRL);
    }

    bool OutputControlFiles_Impl::setOutputScreen(bool outputScreen) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputScreen, outputScreen);
    }

    bool OutputControlFiles_Impl::setOutputExtShd(bool outputExtShd) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputExtShd, outputExtShd);
    }

    bool OutputControlFiles_Impl::setOutputTarcog(bool outputTarcog) {
      return setBooleanFieldValue(OS_OutputControl_FilesFields::OutputTarcog, outputTarcog);
    }

  }  // namespace detail

  IddObjectType OutputControlFiles::iddObjectType() {
    return {IddObjectType::OS_OutputControl_Files};
  }

  bool OutputControlFiles::outputCSV() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputCSV();
  }

  bool OutputControlFiles::outputMTR() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputMTR();
  }

  bool OutputControlFiles::outputESO() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputESO();
  }

  bool OutputControlFiles::outputEIO() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputEIO();
  }

  bool OutputControlFiles::outputTabular() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputTabular();
  }

  bool OutputControlFiles::outputSQLite() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSQLite();
  }

  bool OutputControlFiles::outputJSON() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputJSON();
  }

  bool OutputControlFiles::outputAUDIT() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputAUDIT();
  }

  bool OutputControlFiles::outputSpaceSizing() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSpaceSizing();
  }

  bool OutputControlFiles::outputZoneSizing() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputZoneSizing();
  }

  bool OutputControlFiles::outputSystemSizing() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSystemSizing();
  }

  bool OutputControlFiles::outputDXF() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDXF();
  }

  bool OutputControlFiles::outputBND() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputBND();
  }

  bool OutputControlFiles::outputRDD() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputRDD();
  }

  bool OutputControlFiles::outputMDD() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputMDD();
  }

  bool OutputControlFiles::outputMTD() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputMTD();
  }

  bool OutputControlFiles::outputSHD() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSHD();
  }

  bool OutputControlFiles::outputDFS() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDFS();
  }

  bool OutputControlFiles::outputGLHE() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputGLHE();
  }

  bool OutputControlFiles::outputDelightIn() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDelightIn();
  }

  bool OutputControlFiles::outputDelightELdmp() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDelightELdmp();
  }

  bool OutputControlFiles::outputDelightDFdmp() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDelightDFdmp();
  }

  bool OutputControlFiles::outputEDD() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputEDD();
  }

  bool OutputControlFiles::outputDBG() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputDBG();
  }

  bool OutputControlFiles::outputPerfLog() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputPerfLog();
  }

  bool OutputControlFiles::outputSLN() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSLN();
  }

  bool OutputControlFiles::outputSCI() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputSCI();
  }

  bool OutputControlFiles::outputWRL() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputWRL();
  }

  bool OutputControlFiles::outputScreen() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputScreen();
  }

  bool OutputControlFiles::outputExtShd() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputExtShd();
  }

  bool OutputControlFiles::outputTarcog() const {
    return getImpl<detail::OutputControlFiles_Impl>()->outputTarcog();
  }

  bool OutputControlFiles::setOutputCSV(bool outputCSV) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputCSV(outputCSV);
  }

  bool OutputControlFiles::setOutputMTR(bool outputMTR) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputMTR(outputMTR);
  }

  bool OutputControlFiles::setOutputESO(bool outputESO) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputESO(outputESO);
  }

  bool OutputControlFiles::setOutputEIO(bool outputEIO) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputEIO(outputEIO);
  }

  bool OutputControlFiles::setOutputTabular(bool outputTabular) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputTabular(outputTabular);
  }

  bool OutputControlFiles::setOutputSQLite(bool outputSQLite) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSQLite(outputSQLite);
  }

  bool OutputControlFiles::setOutputJSON(bool outputJSON) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputJSON(outputJSON);
  }

  bool OutputControlFiles::setOutputAUDIT(bool outputAUDIT) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputAUDIT(outputAUDIT);
  }

  bool OutputControlFiles::setOutputSpaceSizing(bool outputSpaceSizing) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSpaceSizing(outputSpaceSizing);
  }

  bool OutputControlFiles::setOutputZoneSizing(bool outputZoneSizing) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputZoneSizing(outputZoneSizing);
  }

  bool OutputControlFiles::setOutputSystemSizing(bool outputSystemSizing) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSystemSizing(outputSystemSizing);
  }

  bool OutputControlFiles::setOutputDXF(bool outputDXF) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDXF(outputDXF);
  }

  bool OutputControlFiles::setOutputBND(bool outputBND) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputBND(outputBND);
  }

  bool OutputControlFiles::setOutputRDD(bool outputRDD) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputRDD(outputRDD);
  }

  bool OutputControlFiles::setOutputMDD(bool outputMDD) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputMDD(outputMDD);
  }

  bool OutputControlFiles::setOutputMTD(bool outputMTD) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputMTD(outputMTD);
  }

  bool OutputControlFiles::setOutputSHD(bool outputSHD) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSHD(outputSHD);
  }

  bool OutputControlFiles::setOutputDFS(bool outputDFS) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDFS(outputDFS);
  }

  bool OutputControlFiles::setOutputGLHE(bool outputGLHE) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputGLHE(outputGLHE);
  }

  bool OutputControlFiles::setOutputDelightIn(bool outputDelightIn) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDelightIn(outputDelightIn);
  }

  bool OutputControlFiles::setOutputDelightELdmp(bool outputDelightELdmp) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDelightELdmp(outputDelightELdmp);
  }

  bool OutputControlFiles::setOutputDelightDFdmp(bool outputDelightDFdmp) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDelightDFdmp(outputDelightDFdmp);
  }

  bool OutputControlFiles::setOutputEDD(bool outputEDD) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputEDD(outputEDD);
  }

  bool OutputControlFiles::setOutputDBG(bool outputDBG) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputDBG(outputDBG);
  }

  bool OutputControlFiles::setOutputPerfLog(bool outputPerfLog) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputPerfLog(outputPerfLog);
  }

  bool OutputControlFiles::setOutputSLN(bool outputSLN) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSLN(outputSLN);
  }

  bool OutputControlFiles::setOutputSCI(bool outputSCI) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputSCI(outputSCI);
  }

  bool OutputControlFiles::setOutputWRL(bool outputWRL) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputWRL(outputWRL);
  }

  bool OutputControlFiles::setOutputScreen(bool outputScreen) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputScreen(outputScreen);
  }

  bool OutputControlFiles::setOutputExtShd(bool outputExtShd) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputExtShd(outputExtShd);
  }

  bool OutputControlFiles::setOutputTarcog(bool outputTarcog) {
    return getImpl<detail::OutputControlFiles_Impl>()->setOutputTarcog(outputTarcog);
  }

  /// @cond
  OutputControlFiles::OutputControlFiles(std::shared_ptr<detail::OutputControlFiles_Impl> impl) : ModelObject(std::move(impl)) {}
  OutputControlFiles::OutputControlFiles(Model& model) : ModelObject(OutputControlFiles::iddObjectType(), model) {
    // This is a required-field, so pick one...
    // Same defaults as EnergyPlus IDD
    setOutputCSV(false);
    setOutputMTR(true);
    setOutputESO(true);
    setOutputEIO(true);
    setOutputTabular(true);
    setOutputSQLite(true);
    setOutputJSON(true);
    setOutputAUDIT(true);
    setOutputSpaceSizing(true);
    setOutputZoneSizing(true);
    setOutputSystemSizing(true);
    setOutputDXF(true);
    setOutputBND(true);
    setOutputRDD(true);
    setOutputMDD(true);
    setOutputMTD(true);
    setOutputSHD(true);
    setOutputDFS(true);
    setOutputGLHE(true);
    setOutputDelightIn(true);
    setOutputDelightELdmp(true);
    setOutputDelightDFdmp(true);
    setOutputEDD(true);
    setOutputDBG(true);
    setOutputPerfLog(true);
    setOutputSLN(true);
    setOutputSCI(true);
    setOutputWRL(true);
    setOutputScreen(true);
    setOutputExtShd(true);
    setOutputTarcog(true);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
