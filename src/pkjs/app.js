var liveconfig = require('./liveconfig');

var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customClay = require('./custom-clay');
var clay = new Clay(clayConfig, customClay, { autoHandleEvents: false });


Pebble.addEventListener("showConfiguration", function(e) {
  
    var websocket = new WebSocket("wss://liveconfig.fletchto99.com/receive/"+Pebble.getAccountToken()+"/"+Pebble.getWatchToken());
    websocket.onmessage = function(message) {
    var attr = JSON.parse(message.data);
    var config = {};
    config[attr.id] = attr.value;
    console.log(JSON.stringify(config));
    Pebble.sendAppMessage(config);
  };
      
  Pebble.openURL(clay.generateUrl());
});


Pebble.addEventListener("webviewclosed", function(e) {
	if (e && !e.response) {
    return;
  }

	Pebble.sendAppMessage(clay.getSettings(e.response), function(e)
	{
		console.log("Success sending to Pebble!");
	}, function() {
		console.log("Error sending to Pebble!");
	});

});
