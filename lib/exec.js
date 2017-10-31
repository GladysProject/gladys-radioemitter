const Promise = require('bluebird');
const Bottleneck = require('bottleneck');

// we limit this module at 1 radioemission per 200ms
const limiter = new Bottleneck(1, 200);

const NUMBER_RETRY_EMISSION = 4;

module.exports = function(params){

	// if module serial is not present, we cannot contact the arduino
	if(!gladys.modules.serial || typeof gladys.modules.serial.sendCode !== 'function') {
		sails.log.error(`You need to install the serial module in Gladys.`);
		return Promise.reject(new Error('DEPENDENCY_MISSING'));
	}

	var code = parseInt(params.deviceType.identifier) + parseInt(params.state.value);

	// radio is not secure transmision, we send the signals many time to be sure the signal is sent
	var promises = [];
	for(var i = 0; i < NUMBER_RETRY_EMISSION; i ++){
		promises.push(limiter.schedule(gladys.modules.serial.sendCode, `{"function_name":"SendRadioCode","code":"${code}"}%`));
	}

	return Promise.all(promises);
};