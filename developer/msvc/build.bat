cd C:\working\openstudioTrunk\build
devenv OpenStudio.sln /Build Debug /Project ALL_BUILD
devenv OpenStudio.sln /Build Release /Project ALL_BUILD
devenv OpenStudio.sln /Build RelWithDebInfo /Project ALL_BUILD
cd C:\working\openstudioTrunk\build\PerturbationAnalysisTool-prefix\src\PerturbationAnalysisTool-build
devenv PerturbationAnalysisTool.sln /Build Debug
devenv PerturbationAnalysisTool.sln /Build Release
devenv PerturbationAnalysisTool.sln /Build RelWithDebInfo
cd C:\working\openstudioTrunk\build