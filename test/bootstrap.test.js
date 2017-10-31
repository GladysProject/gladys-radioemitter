before(function(){

    // mocking Gladys serial module
    global.gladys = {
        modules: {
            serial: { 
                sendCode: function() {
                    console.log('serial module called at ' + new Date().getTime());
                    return Promise.resolve();
                }
            }
        }
    }
});