# TODO: Determine a better way to point to the resources directory, or put the osps in the pacakage itself.
# TODO: Glob osp paths
analysis <- OpenStudioAnalysis(paste("../../resources/project/version/project_0_8_0.osp"),analysisName="My Test Analysis")
show(analysis)
expect_that(nrow(getDataPoints(analysis)), equals(100))

