var assert = require('assert')
var openstudio = require('../OpenStudio.js').openstudio;

var model = openstudio.model.exampleModel();
var modelstr = model.toString();
var encoded = openstudio.compressToBase64(modelstr,9);

assert(encoded.length * 4 < modelstr.length)

