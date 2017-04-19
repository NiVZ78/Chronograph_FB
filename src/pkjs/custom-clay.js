module.exports = function(minified) {
  var Clay = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;
  var clayConfig = this;
  
/* function toggleChoiceText() {
    // Default to hiding all the text choice
    clayConfig.getItemById('Background_Colour').hide();
 
    switch (this.get()) {
      case false: 
        clayConfig.getItemById('Background_Colour').show();
        break;
    }
  }*/
  
  Clay.on(Clay.EVENTS.AFTER_BUILD, function() {    
       
 // Trigger choice text
/*    var ChoiceToggle = clayConfig.getItemByMessageKey('KEY_AUTO_BACKGROUND');
    toggleChoiceText.call(ChoiceToggle);
    ChoiceToggle.on('change', toggleChoiceText);*/
    
    
    var connection = new WebSocket("wss://liveconfig.fletchto99.com/forward/" + Clay.meta.accountToken + "/" + Clay.meta.watchToken);
    connection.onopen = function() {
      Clay.getAllItems().map( function(item) {
        item.on('change', function() {
          connection.send(JSON.stringify({id: this.messageKey, value: this.get()}));
        });
      });
    };
  });
};
