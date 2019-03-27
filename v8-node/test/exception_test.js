openstudio = require('../OpenStudio.js').openstudio;

try {
  var o2 = new openstudio.OptionalInt();
  console.log(o2.get());
} catch (err) {
  console.log(err);
}

