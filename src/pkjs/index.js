// Import the Clay package
var Clay = require('@rebble/clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);


clay.registerComponent(require('./custom-toggle'));
clay.registerComponent(require('./custom-color'));
clay.registerComponent(require('./custom-hour'));
clay.registerComponent(require('./custom-minute'));


// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
  }
);
