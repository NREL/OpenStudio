library(methods)

#' Class OpenStudioAnalysis for accessing data from a single OpenStudio Analysis in R.
#' @export
setClass("OpenStudioAnalysis",
  representation(id="integer",
                 name="character",
                 uuid="character",
                 versionUUID="character",
                 path="character",
                 dataPoints="data.frame",
                 inputVariables="data.frame",
                 responseFunctions="data.frame",
                 objectiveFunctions="data.frame",                 
                 outputVariables="data.frame")
)

#' @title OpenStudioAnalysis Constructor
#'
#' @description
#' Construct an OpenStudioAnalysis from an osp file.
#'
#' @details
#' If only the path is provided, will load the Analysis with id=0. If multiple analyses are stored in the
#' osp, the one to load may be selected by specifiying the analysis's database id xor its name.
#'
#' @param path the path to an .osp file
#' @param analysisId the SQLite primary key (id) of the analysis to be loaded
#' @param analysisName the name of the analysis to be loaded
#' @return a new OpenStudioAnalysis object
#' @export
OpenStudioAnalysis <- function(path,analysisId,analysisName) {
  if (missing(analysisId) && missing(analysisName)) {
    return(createOpenStudioAnalysisFromId(path))
  }
  if (missing(analysisName)) {
    return(createOpenStudioAnalysisFromId(path,as.integer(analysisId)))
  }
  else {
    if (!missing(analysisId)) {
      stop("Conflicting call to OpenStudioAnalysis. Please specify analysisId or analysisName, not both.")
    }
    return(createOpenStudioAnalysisFromName(path,analysisName))
  }
}

#' Get the id of the argument.
#'
#' Returns the id of an S4 object.
#'
#' @param object Object whose id is to be returned.
#' @return id
#' 
#' @export
#' @docType methods
#' @rdname getId
setGeneric("getId",function(object) { standardGeneric("getId") })

#' @rdname getId
#' @aliases getId,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getId",
          "OpenStudioAnalysis",
          function(object) { return(object@id) })

#' Get the name of the argument.
#'
#' Returns the name of an S4 object.
#'
#' @param object Object whose name is to be returned.
#' @return name
#' 
#' @export
#' @docType methods
#' @rdname getName
setGeneric("getName",function(object) { standardGeneric("getName") })

#' @rdname getName
#' @aliases getName,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getName","OpenStudioAnalysis",
  function(object) {
    return(object@name)
  }
)

#' Get the UUID of the argument.
#'
#' Returns the UUID of an S4 object.
#'
#' @param object Object whose UUID is to be returned.
#' @return UUID
#' 
#' @export
#' @docType methods
#' @rdname getUUID
setGeneric("getUUID",function(object) { standardGeneric("getUUID") })

#' @rdname getUUID
#' @aliases getUUID,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getUUID","OpenStudioAnalysis",
  function(object) {
    return(object@uuid)
  }
)

#' Get the Version UUID of the argument.
#'
#' Returns the Version UUID of an S4 object.
#'
#' @param object Object whose Version UUID is to be returned.
#' @return Version UUID
#' 
#' @export
#' @docType methods
#' @rdname getVersionUUID
setGeneric("getVersionUUID",function(object) { standardGeneric("getVersionUUID") })

#' @rdname getVersionUUID
#' @aliases getVersionUUID,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getVersionUUID","OpenStudioAnalysis",
  function(object) {
    return(object@versionUUID)
  }
)

#' Get the path of the argument.
#'
#' Returns the path of an S4 object.
#'
#' @param object Object whose path is to be returned.
#' @return path
#' 
#' @export
#' @docType methods
#' @rdname getPath
setGeneric("getPath",function(object) { standardGeneric("getPath") })

#' @rdname getPath
#' @aliases getPath,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getPath","OpenStudioAnalysis",
  function(object) {
    return(object@path)
  }
)

#' Get the data points of the argument.
#'
#' Returns the data points of an S4 object.
#'
#' @param object Object whose data points are to be returned.
#' @return data points
#' 
#' @export
#' @docType methods
#' @rdname getDataPoints
setGeneric("getDataPoints",function(object) { standardGeneric("getDataPoints") })

#' @rdname getDataPoints
#' @aliases getDataPoints,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getDataPoints","OpenStudioAnalysis",
  function(object) {
    return(object@dataPoints)
  }
)

#' Get the input variables of the argument.
#'
#' Returns the input variables of an S4 object.
#'
#' @param object Object whose input variable are to be returned.
#' @return input variables
#' 
#' @export
#' @docType methods
#' @rdname getInputVariables
setGeneric("getInputVariables",function(object) { standardGeneric("getInputVariables") })

#' @rdname getInputVariables
#' @aliases getInputVariables,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getInputVariables","OpenStudioAnalysis",
  function(object) {
    return(object@inputVariables)
  }
)

#' Get the response functions of the argument.
#'
#' Returns the response functions of an S4 object.
#'
#' @param object Object whose response functions are to be returned.
#' @return response functions
#' 
#' @export
#' @docType methods
#' @rdname getResponseFunctions
setGeneric("getResponseFunctions",function(object) { standardGeneric("getResponseFunctions") })

#' @rdname getResponseFunctions
#' @aliases getResponseFunctions,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getResponseFunctions","OpenStudioAnalysis",
  function(object) {
    return(object@responseFunctions)
  }
)

#' Get the objective functions of the argument.
#'
#' Returns the objective functions of an S4 object.
#'
#' @param object Object whose objective functions are to be returned.
#' @return objective functions
#' 
#' @export
#' @docType methods
#' @rdname getObjectiveFunctions
setGeneric("getObjectiveFunctions",function(object) { standardGeneric("getObjectiveFunctions") })

#' @rdname getObjectiveFunctions
#' @aliases getObjectiveFunctions,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getObjectiveFunctions","OpenStudioAnalysis",
  function(object) {
    return(object@objectiveFunctions)
  }
)

#' Get the output variables of the argument.
#'
#' Returns the output variables of an S4 object.
#'
#' @param object Object whose output variable are to be returned.
#' @return output variables
#' 
#' @export
#' @docType methods
#' @rdname getOutputVariables
setGeneric("getOutputVariables",function(object) { standardGeneric("getOutputVariables") })

#' @rdname getOutputVariables
#' @aliases getOutputVariables,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getOutputVariables","OpenStudioAnalysis",
  function(object) {
    return(object@outputVariables)
  }
)

#' Creates a pairs plot of OpenStudioAnalysis data.
#'
#' @param analysis an OpenStudioAnalysis object
#' @param variables a string "all", "inputs", or "outputs" indicating what will show up 
#'        in the plot
#'
#' @aliases pairs,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod(f = "pairs", signature = "OpenStudioAnalysis",
  definition = function(x, variables = "all") {
    if (! variables %in% c("all", "inputs", "outputs")) 
      stop("'variables' must be one of \"all\", \"inputs\", or \"outputs\"")
    nInput <- length(x@inputVariables)
    nOutput <- length(x@outputVariables)
    if (variables == 'all') {
      pairs(slot(x, 'dataPoints')[, -c(1:3)])
    }
    else {
      if (variables == 'inputs') {
        pairs(slot(x, 'dataPoints')[, c(4:(3 + nInput))])
      }
      else {
        pairs(slot(x, 'dataPoints')[, c((4 + nInput):(3 + nInput + nOutput))])
      }
    }
  }
)

#' Prints a summary of the indicated OpenStudioAnalysis variables. 
#' 
#' @param analysis an OpenStudioAnalysis object
#' @param variables a string "all", "inputs", or "outputs" indicating what variables 
#'        will be summarized
#'
#' @aliases summary,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod(f = "summary", signature = "OpenStudioAnalysis",
  definition = function(object, variables = "all") {
    if (! variables %in% c("all", "inputs", "outputs")) 
      stop("'variables' must be one of \"all\", \"inputs\", or \"outputs\"")
    nInput <- length(object@inputVariables)
    nOutput <- length(object@outputVariables)
    if(variables == 'all'){
      summary(object@dataPoints[, -c(1:3)])
    }
    else {
      if (variables == 'inputs') {
        summary(object@dataPoints[, c(4:(3 + nInput))])
      }
      else {
        summary(object@dataPoints[, c((4 + nInput):(3 + nInput + nOutput))])
      }
    }
  }
)

#' plot OpenStudioAnalysis
#' 
#' Creates a scatter plot of two variables in an OpenStudioAnalysis.
#'
#' @param analysis an OpenStudioAnalysis object
#' @param s the name of the variable/function to be plotted on the x-axis
#' @param t the name of the variable/function to be plotted on the y-axis
#'
#' @aliases plot,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod(f = "plot", signature = "OpenStudioAnalysis",
  definition = function(x, y, s = "x1", t = "y1", ...) {
    plot(x@dataPoints[[s]], x@dataPoints[[t]], xlab = s, ylab = t, col = "navy", pch = 20)
    grid()
  }  
)

#' @title as.data.frame for OpenStudioAnalysis
#'
#' @description 
#' Method to coerce OpenStudioAnalysis to data.frame type. Automatically used by 
#' methods like lm.
#'
#' @param analysis an OpenStudioAnalysis object
#' @return the getDataPoints data.frame
#'
#' @method as.data.frame OpenStudioAnalysis
#' @S3method as.data.frame OpenStudioAnalysis
as.data.frame.OpenStudioAnalysis <- function(x) {
  return(slot(x, "dataPoints"))
}

#' As("OpenStudioAnalysis", "data.frame")
#'
#' @name as
#' @family OpenStudioAnalysis
setAs("OpenStudioAnalysis", "data.frame", 
       function(from) as.data.frame.OpenStudioAnalysis(from))

#' Get a formula object.
#'
#' Returns a formula object for an S4 object according to some criteria.
#'
#' @param object Object for whom a formula is to be constructed
#' @return a formula object
#' 
#' @export
#' @docType methods
#' @rdname getFormula
setGeneric("getFormula", function(object, ...) { 
  standardGeneric("getFormula") 
  }
)

#' Returns an lm formula for an OpenStudioAnalysis.
#'
#' @param analysis an OpenStudioAnalysis object
#' @param output the name of the variable/function for which a model is to be constructed
#' @param type the type of model to be fit: "linear", "quadratic", or "interaction"
#'
#' @rdname getFormula
#' @aliases getFormula,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getFormula", signature = "OpenStudioAnalysis",
  definition = function(object, output = "y1", type = "linear", ...) {
    if (! type %in% c("linear", "quadratic", "interaction")) 
      stop("'type' must be one of \"linear\", \"quadratic\", or
            \"interaction\"")

    nInput <- nrow(object@inputVariables)
    inputColNames <- colnames(object@dataPoints[, 4:(3 + nInput)])
    
    if (type == "linear"){
     fr <- formula(paste(output, " ~ " , 
                         paste(inputColNames, collapse = " + "), sep = ""))
    }
    else {
      if (type == "quadratic") {
       fr <- formula(paste(output, " ~ ",
                           paste("(", paste(inputColNames, collapse = " + "), 
                                 ")^2", sep = ""), " + ",
                           paste(paste("I(", inputColNames, "^2)", sep=""), 
                                 collapse = " + "), sep = ""))
      }
      else {
       fr <- formula(paste(output, " ~ " , 
                           paste("(", paste(inputColNames, collapse = " + "), 
                                 ")^2", sep = ""), sep = ""))
      }
    }
    return(fr)
  }
)

#' Get formula objects.
#'
#' Returns formula objects for an S4 object according to some criteria.
#'
#' @param object Object for whom formulae are to be constructed
#' @return a list of formula objects
#' 
#' @export
#' @docType methods
#' @rdname getFormulas
setGeneric("getFormulas", function(object, ...) { 
  standardGeneric("getFormulas") 
  }
)

#' Returns the lm formulas for an OpenStudioAnalysis.
#'
#' @param analysis an OpenStudioAnalysis object
#' @param type the type of model to be fit: "linear", "quadratic", or "interaction"
#' @param variables the outputs for which models are to be fit: "outputs", "response", or 
#'        "objective"
#'
#' @rdname getFormulas
#' @aliases getFormulas,OpenStudioAnalysis,OpenStudioAnalysis-method
setMethod("getFormulas", signature = "OpenStudioAnalysis",
  definition = function(object, type = "linear", variables = "outputs", ...) {
    if (! type %in% c("linear", "quadratic", "interaction")) 
      stop("'type' must be one of \"linear\", \"quadratic\", or
            \"interaction\"")
    if (! variables %in% c("response", "objective", "outputs")) 
      stop("'variables' must be one of \"response\", \"objective\",
           or \"outputs\"")
    
    nResponse <- nrow(object@responseFunctions)
    nObjective <- nrow(object@objectiveFunctions)
    nInput <- nrow(object@inputVariables)
    nOutput <- nrow(object@outputVariables)
    inputColNames <- colnames(object@dataPoints[, 4:(3 + nInput)])
    
    if (variables == "response") {
      if (nResponse == 0) {
        stop("There are no response variables in the OpenStudio object.")
      }
      else {
        nStart <- 3 + nInput
        dataColNames <- colnames(object@dataPoints[, 
                                  (nStart + 1):(nStart + nResponse)])
        nVars <- length(dataColNames)
      }
    }
    else {
      if (variables == "objective") {
        if (nObjective == 0) {
          stop("There are no objective variables in the OpenStudio Object.")            
        }
        else {
          nStart <- 3 + nInput + nResponse
          dataColNames <- colnames(object@dataPoints[, 
                                    (nStart + 1):(nStart + nObjective)])
          nVars <- length(dataColNames)
        }
      }
      else {
        if (nOutput == 0) {
          stop("There are no output variables in the OpenStudio Object.")
        }
        else {
          nStart <- 3 + nInput + nResponse + nObjective
          dataColNames <- colnames(object@dataPoints[, 
                                    (nStart + 1):(nStart + nOutput)])
          nVars <- length(dataColNames)
        }
      }
    } 

    results <- list()
    for (i in 1:nVars){
      if (type == "linear"){
       fr <- formula(paste(dataColNames[i], " ~ " , 
                           paste(inputColNames, collapse = " + "), sep = ""))
      }
      else {
        if (type == "quadratic") {
         fr <- formula(paste(dataColNames[i], " ~ ",
                             paste(inputColNames, collapse = " + "), " + ",
                             paste(paste("I(", inputColNames, "^2)", sep=""), 
                                   collapse = " + "), sep = ""))
        }
        else {
         fr <- formula(paste(dataColNames[i], " ~ " , 
                             paste("(", paste(inputColNames, collapse = " + "), 
                                   ")^2", sep = ""), " + ", 
                             paste(paste("I(", inputColNames, "^2)", sep=""), 
                                    collapse = " + "), sep = ""))
        }
      }
      results[[i]] <- fr
    }
    return(results)
  }
)

# DETAILS ----------------------------------------------------------------------

#' Recursive function for extracting output variable data
appendOutputVariableData <- function(db,wherestrs,outputVariableData) {

  qstr <- paste("SELECT DISTINCT a.name, a.attributeValueType, a.attributeUnits ",wherestrs$getNames,sep="")
#  message("Query ",qstr," results in:")
  attributeData <- dbGetQuery(db,qstr)
  n <- nrow(attributeData)
#  print(attributeData)
  if (n > 0) {
    for (i in 1:n) {
      valueType = as.integer(attributeData$attributeValueType[i])
#      message("Processing output attribute ",i," of type ",valueType," from wherestrs$getNames = ",wherestrs$getNames,".")
      if (valueType == 7) {
        # make recursive call to find lowest level of nesting
        wherestrs$getNames <- paste("FROM AttributeRecords a WHERE a.parentAttributeRecordId=( SELECT a.id ",
                                     wherestrs$getNames," AND a.name='",attributeData$name[i],"' )",sep="")
        wherestrs$getData <- paste("FROM AttributeRecords a WHERE a.parentAttributeRecordId=( SELECT a.id ",
                                   wherestrs$getData," AND a.name='",attributeData$name[i],"' )",sep="")
        outputVariableData <- appendOutputVariableData(db,wherestrs,outputVariableData)
      }
      else {
        # simple attribute, record and pull data out
        message("Processing output variable ",outputVariableData$cnt + 1)
        qstr <- paste("SELECT a.attributeValue ",wherestrs$getData," AND a.name='",attributeData$name[i],"'",sep="")
        temp <- dbGetPreparedQuery(db,qstr,bind.data=data.frame(dpIds=outputVariableData$df$id))
        names(temp)[1] <- paste("y",outputVariableData$cnt + 1,sep="")
        if (nrow(temp) == nrow(outputVariableData$df)) {
          outputVariableData$colIndex <- outputVariableData$colIndex + 1
          outputVariableData$cnt <- outputVariableData$cnt + 1
          if ((nrow(temp) > 0) && (!is.na(as.numeric(as.character(temp[[1]][1]))))) {
            temp[[1]] <- as.numeric(as.character(temp[[1]]))
          }
          outputVariableData$df <- cbind(outputVariableData$df,temp)
          continuous <- ((as.integer(attributeData$attributeValueType[i]) == 1) | 
                         (as.integer(attributeData$attributeValueType[i]) == 2))
          outputVariableData$outputVariables<- 
              rbind(outputVariableData$outputVariables,
                    data.frame(id = as.integer(NA), 
                               name = as.character(attributeData$name[i]),
                               dfcol = as.integer(outputVariableData$colIndex), 
                               continuous = as.logical(continuous), 
                               units = as.character(attributeData$attributeUnits[i]), 
                               min = as.numeric(NA), 
                               max = as.numeric(NA),
                               stringsAsFactors=FALSE))
        }
        else {
          warning("Skipping output variable ",attributeData$name[i],", because query ",qstr,
                  " returned data with ",nrow(temp)," rows when expected ",nrow(outputVariableData$df)," rows:")
          warning(temp)
        }
      }
    }
  }
#  message("Returning from appendOutputVariableData with wherestrs$getNames = ",wherestrs$getNames,".")
  return(outputVariableData)
}

#' Implementation of constructing OpenStudioAnalysis from Id.
createOpenStudioAnalysisFromId <- function(path,analysisId=1) {

  # open database
  drv <- dbDriver("SQLite")
  db <- dbConnect(drv,dbname=path)
  
  # pull analysis information
  qstr <- paste("SELECT * FROM AnalysisRecords WHERE id=",analysisId,sep="")
  result <- dbSendQuery(db,qstr)
  analysisData <- fetch(result,n=1)
  if (nrow(analysisData) == 0) {
    stop("Unable to retrieve analysis with id ",analysisId,".")
  }
  dbClearResult(result)
  
  # start data frame
  qstr <- paste("SELECT * FROM DataPointRecords WHERE analysisRecordId=",analysisId," ORDER BY id",sep="")
#  message("Query ",qstr," results:")
  dataPointData <- dbGetQuery(db,qstr)
#  print(dataPointData)
  df <- data.frame(id=dataPointData$id,
                   complete=dataPointData$complete,
                   failed=dataPointData$failed)
  colIndex <- ncol(df)
  message("DataPoints data.frame:")
#  print(df)
  
  # pull input variable information
  qstr <- paste("SELECT * FROM VariableRecords WHERE problemRecordId=",analysisData$problemRecordId[1]," ORDER BY variableVectorIndex",sep="")
#  message("Query ",qstr," results:")
  inputVariableData <- dbGetQuery(db,qstr)
#  print(inputVariableData)
  nInputRows <- nrow(inputVariableData)
  inputVariables <- data.frame(id = integer(nInputRows), 
                               name = character(nInputRows), 
                               dfcol = integer(nInputRows), 
                               continuous = logical(nInputRows), 
                               units = character(nInputRows), 
                               min = numeric(nInputRows), 
                               max = numeric(nInputRows),
                               stringsAsFactors=FALSE)
  if (nInputRows > 0) {
    for (i in 1:nInputRows) {
      message(paste("Processing variable ",i,sep=""))
      colIndex <- colIndex + 1
      continuous <- (inputVariableData$variableRecordType[i] == 1)
      if (continuous) {
        qstr <- paste("SELECT dataPointValue FROM DataPointValueRecords WHERE dataPointRecordId=? AND continuousVariableRecordId=",
                      inputVariableData$id[i],sep="")
#        message("Query ",qstr," results:")                      
        temp <- dbGetPreparedQuery(db,qstr,bind.data=data.frame(dpIds=df$id))
        names(temp)[1] <- paste("x",i,sep="")
#        print(temp)
        df <- cbind(df,temp)
        inputVariables[i, ] <- data.frame(
                                 as.integer(inputVariableData$id[i]), 
                                 inputVariableData$name[i],
                                 as.integer(colIndex), 
                                 as.logical(continuous), 
                                 units = "", 
                                 as.numeric(inputVariableData$minimum[i]), 
                                 as.numeric(inputVariableData$maximum[i]),
                                 stringsAsFactors=FALSE)
      }
      else {
        qstr <- paste("SELECT o.perturbationVectorIndex FROM DiscretePerturbationRecords o, DataPoint_DiscretePerturbation_JoinRecords j WHERE ",
                      "o.variableRecordId=",inputVariableData$id[i]," AND j.leftId=? AND o.id=j.rightId",sep="")
#        message("Query ",qstr," results:")    
        temp <- dbGetPreparedQuery(db,qstr,bind.data=data.frame(dpIds=df$id))
        names(temp)[1] <- paste("x",i,sep="")
#        print(temp)
        df <- cbind(df,temp)
        qstr <- paste("SELECT MAX(perturbationVectorIndex) FROM DiscretePerturbationRecords WHERE variableRecordId=",inputVariableData$id[i],sep="")
        inputVariables[i, ] <- data.frame(
                                 as.integer(inputVariableData$id[i]), 
                                 inputVariableData$name[i],
                                 as.integer(colIndex), 
                                 as.logical(continuous), 
                                 units = "", 
                                 min=0,
                                 max=as.integer(dbGetQuery(db,qstr)),
                                 stringsAsFactors=FALSE)
      }
#      message("DataPoints data.frame:")
#      print(df)
    }
  }
  
  # pull response function information
  qstr <- paste("SELECT * FROM FunctionRecords WHERE problemRecordId=",
                analysisData$problemRecordId[1],
                " AND functionType=0 ORDER BY functionVectorIndex",
                sep="")
  responseFunctionData <- dbGetQuery(db,qstr)
  nResponseRows <- nrow(responseFunctionData)
  responseFunctions <- data.frame(id = integer(nResponseRows), 
                                  name = character(nResponseRows), 
                                  dfcol = integer(nResponseRows),
                                  stringsAsFactors=FALSE)
  if (nResponseRows > 0) {
    for (i in 1:nResponseRows) {
      message(paste("Processing response function ",i,sep=""))
      colIndex <- colIndex + 1
      qstr <- paste("SELECT dataPointValue FROM DataPointValueRecords WHERE dataPointRecordId=? AND functionRecordId=",
                    responseFunctionData$id[i],
                    sep="")
      temp <- dbGetPreparedQuery(db,qstr,bind.data=data.frame(dpIds=df$id))
      names(temp)[1] <- paste("r",i,sep="")
      df <- cbind(df,temp)
      responseFunctions[i, ] <- data.frame(
                                  as.integer(responseFunctionData$id[i]),
                                  as.character(responseFunctionData$name[i]),
                                  as.integer(colIndex),
                                  stringsAsFactors=FALSE)
    }
  }
  
  # pull objective function information
  qstr <- paste("SELECT * FROM FunctionRecords WHERE problemRecordId=",
                analysisData$problemRecordId[1],
                " AND functionType=1 ORDER BY functionVectorIndex",
                sep="")
  objectiveFunctionData <- dbGetQuery(db,qstr)
  nObjectiveRows <- nrow(objectiveFunctionData)
  objectiveFunctions <- data.frame(id = integer(nObjectiveRows), 
                                   name = character(nObjectiveRows), 
                                   dfcol = integer(nObjectiveRows),
                                   stringsAsFactors=FALSE)
  if (nObjectiveRows > 0) {
    for (i in 1:nObjectiveRows) {
      message(paste("Processing objective function ",i,sep=""))
      colIndex <- colIndex + 1
      qstr <- paste("SELECT dataPointValue FROM DataPointValueRecords WHERE dataPointRecordId=? AND functionRecordId=",
                    objectiveFunctionData$id[i],
                    sep="")
      temp <- dbGetPreparedQuery(db,qstr,bind.data=data.frame(dpIds=df$id))
      names(temp)[1] <- paste("o",i,sep="")
      df <- cbind(df,temp)
      objectiveFunctions[i, ] <- data.frame(
                                   as.integer(objectiveFunctionData$id[i]),
                                   as.character(objectiveFunctionData$name[i]),
                                   as.integer(colIndex),
                                   stringsAsFactors=FALSE)
    }
  }  
  
  # pull output variable (attribute) information
  wherestrs <- list()
  wherestrs$getNames <- paste("FROM AttributeRecords a, FileReferenceRecords f, DataPointRecords d ",
                              "WHERE a.fileReferenceRecordId=f.id AND d.xmlOutputDataRecordId=f.id",
                              sep="")
  wherestrs$getData <- paste("FROM AttributeRecords a, FileReferenceRecords f, DataPointRecords d ",
                              "WHERE a.fileReferenceRecordId=f.id AND d.xmlOutputDataRecordId=f.id AND d.id=?",
                              sep="")
  outputVariableData <- list(cnt=0,colIndex=colIndex,df=df,outputVariables=
                             data.frame(id = integer(0), 
                                        name = character(0), 
                                        dfcol = integer(0), 
                                        continuous = logical(0), 
                                        units = character(0), 
                                        min = numeric(0), 
                                        max = numeric(0),
                                        stringsAsFactors=FALSE))
  outputVariableData <- appendOutputVariableData(db,wherestrs,outputVariableData)
  df <- outputVariableData$df
  colIndex <- outputVariableData$colIndex
  outputVariables <- outputVariableData$outputVariables
    
  # close database
  dbDisconnect(db)
  
  # return analysis
  analysis <- new("OpenStudioAnalysis",
                  id=as.integer(analysisId),
                  name=analysisData$name[1],
                  uuid=analysisData$handle[1],
                  versionUUID=analysisData$uuidLast[1],
                  path=path,
                  dataPoints=df,
                  inputVariables=inputVariables,
                  responseFunctions=responseFunctions,
                  objectiveFunctions=objectiveFunctions,
                  outputVariables=outputVariables)
  return(analysis)
}

#' Implementation of constructing OpenStudioAnalysis from anlaysisName.
createOpenStudioAnalysisFromName <- function(path,analysisName) { 
  id = NULL
  
  # open database
  drv <- dbDriver("SQLite")
  db <- dbConnect(drv,dbname=path)
  
  # determine the id
  qstr <- paste("SELECT id FROM AnalysisRecords WHERE name='",analysisName,"'",sep="")
  result <- dbSendQuery(db,qstr)
  ids <- fetch(result,n=2)
  if (nrow(ids) > 1) {
    warning("Multiple analyses with the same name. Returning the first.")
  }
  if (nrow(ids) == 0) {
    stop("No analyses named '",analysisName,"'.")
  }
  id <- as.integer(ids[1,1])
  dbClearResult(result)
  
  # close database
  dbDisconnect(db)
  
  print(paste("Loading analysis named '",analysisName,"' which has id=",id,".",sep=""))
  
  return(createOpenStudioAnalysisFromId(path,id))
}
          

