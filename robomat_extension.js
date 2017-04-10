/* Extension demonstrating a blocking command block */
/* Sayamindu Dasgupta <sayamindu@media.mit.edu>, May 2014 */

new (function() {
    var ext = this;

    // Cleanup function when the extension is unloaded
    ext._shutdown = function() {};

    // Status reporting code
    // Use this to report missing hardware, plugin or unsupported browser
    ext._getStatus = function() {
        return {status: 2, msg: 'Ready'};
    };

    ext.motor = function(number,direction,power,time,callback) {
 		$.ajax({
              url: 'http://192.168.1.43/motor/'+number+'/'+direction+'/'+power+'/'+time,
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);

                  window.setTimeout(function() {
                    callback();
                  }, time);
              }
        });    
 	  };

    ext.engine = function(direction,power,time,callback) {
    $.ajax({
              url: 'http://192.168.1.43/engine/'+direction+'/'+power+'/'+time,
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);

                  window.setTimeout(function() {
                    callback();
                  }, time);
              }
        });    
    };

    ext.engine_stop = function() {
    $.ajax({
              url: 'http://192.168.1.43/engine/0/0/0',
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);
              }
        });    
    };


    // Block and block menu descriptions
    var descriptor = {
        blocks: [
            ['w', 'RobotMat motor %s direcction %s power %s time %s', 'motor','1','0','0','0'],
            ['w', 'RobotMat engine direcction %s power %s time %s', 'engine','0','0','0'],
            [' ', 'RobotMat engine stop', 'engine_stop'],
        ]
    };

    // Register the extension
    ScratchExtensions.register('RobotMat extention', descriptor, ext);
})();