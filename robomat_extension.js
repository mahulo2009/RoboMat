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

    ext.send_forward = function(speed,callback) {
 		$.ajax({
              url: 'http://192.168.1.43/?cmd=forward&param='+speed,
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);
              }
        });    
 	};

    ext.send_backward = function(speed,callback) {
 		$.ajax({
              url: 'http://192.168.1.43/?cmd=backward&param='+speed,
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);
              }
        });    
 	};


    ext.send_wheel = function(wheel,callback) {
 		$.ajax({
              url: 'http://192.168.1.43/?cmd=wheel&param='+wheel,
              success: function( result ) {
                  // Got the data - parse it and return the temperature
                  console.log('Result ' + result);
              }
        });    
 	};


    // Block and block menu descriptions
    var descriptor = {
        blocks: [
            ['W', 'RobotMat forware speed %s', 'send_forward','0'],
            ['W', 'RobotMat backware speed %s', 'send_backward','0'],
            ['W', 'RobotMat wheel %s', 'send_wheel','0'],
        ]
    };

    // Register the extension
    ScratchExtensions.register('RobotMat extention', descriptor, ext);
})();