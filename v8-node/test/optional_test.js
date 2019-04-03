openstudio = require('../OpenStudio.js').openstudio;

var i = 10;


var o = new openstudio.OptionalInt(i);
console.log(o.get());


