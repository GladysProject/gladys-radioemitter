const Promise = require('bluebird');

module.exports = function(params){

	// if module serial is not present, we cannot contact the arduino
	if(!gladys.modules.serial || typeof gladys.modules.serial.sendCode !== 'function') {
		sails.log.error(`You need to install the serial module in Gladys.`);
		return Promise.reject(new Error('DEPENDENCY_MISSING'));
	}
	var res = params.deviceType.identifier.split(".");
	var period = parseInt(res[0]);
	switch(params.deviceType.protocol){
		// if device is standard RF old-style compatible
		case 'radio':
			var code = parseInt(res[1]) + parseInt(params.state.value);
			var msg = `{"function_name":"SendRadioCode","code":"${code}"}%`;
			sails.log.info(`Sending datas to arduino:` + msg);
			gladys.modules.serial.sendCode(msg);
			break;
		// if device is RF with different code
		case 'diffRF':
			var code = 0;
			switch (params.state.value) {
				case 0:
					code = parseInt(res[1]);
				break;
				case 1:
					code = parseInt(res[2]);
				break;

				default:
					console.log('DeviceType state is wrong. Should be 0 or 1.');
				break;
			}
			var msg = `{"function_name":"SendRadioCode","code":"${code}","period":"${period}"}%`
			sails.log.info(`Sending datas to arduino:` + msg);
			gladys.modules.serial.sendCode(msg);
			break;
		// else if device is DIO-compatible
		case 'DIO':
			var ident = parseInt(res[1]);
			var unit = parseInt(res[2]);
			var val = parseInt(params.state.value);
			var group = 0;
			var msg = `{"function_name":"SendNewCode","Addr":"${ident}","group":"${group}","unit":"${unit}","value":"${val}","period":"${period}"}%`
			sails.log.info(`Sending datas to arduino:` + msg);
			gladys.modules.serial.sendCode(msg);
			break;
		//any other device type can be defined here, just specify protocol
		default:
			sails.log.error(`Unknown RF Protocol or service`);
	}
	
	return Promise.resolve();
};
