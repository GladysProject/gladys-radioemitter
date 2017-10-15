const Promise = require('bluebird');
const retry = 4;

// wait time between signals in ms
const waitTimeBetweenSignals = 200;

module.exports = function(params){

    let code = 0;
    // setting default parameters for RCSwitch protocol
    let canal = 1;
    let pulseLength = 100;
    // if module serial is not present, we cannot contact the arduino
    if(!gladys.modules.serial || typeof gladys.modules.serial.sendCode !== 'function') {
        console.log(`You need to install the serial module in Gladys.`);
        return Promise.reject(new Error('DEPENDENCY_MISSING'));
    }

    // Only binary type deviceType is handled
    if (params.deviceType.type !== 'binary') {
        console.log(`${params.deviceType.type} deviceType type is not handled`);
        return Promise.reject(new Error('Only binary type handled for now'));

    }
    else {
        switch (params.state.value) {
            // switch Off
            case 0:
                code = parseInt(params.deviceType.identifier);
                break;
            // switch On
            case 1:
                code = params.deviceType.max ===1 ?
                    // Phenix outlet case
                    parseInt(params.deviceType.identifier) + 1 :
                    // Non phenix outlet
                    parseInt(params.deviceType.max);
                break;
            default:
                console.log(`${params.deviceType.state} is not a valid state for binary deviceType.`);
                return Promise.reject(new Error('state should be 0 or 1'));
        }
    }
    // check for canal and pulseLength
    const temp = params.deviceType.deviceTypeIdentifier;
    if(temp){
        canal = parseInt(temp.split(':')[0]);
        pulseLength = parseInt(temp.split(':')[1]);
    }

    // radio is not secure transmission, we send the signals many time to be sure the signal is sent
    for (var i = 0; i <= retry; i++) {
        setTimeout(function(){
            gladys.modules.serial.sendCode(`{"function_name":"SendRadioCode","code":"${code}","canal":${canal},"pulseLength":${pulseLength}}%`);
        }, i * waitTimeBetweenSignals);
    }

    return Promise.resolve();
};