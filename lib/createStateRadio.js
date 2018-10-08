var shared = require('./shared.js');

module.exports = function(json){
	
	var param = {
		device: {
			name: 'Sensor',
			protocol: '433Mhz',
			service: 'radioemitter',
			identifier: json.value
		},
		types: [
			{
				type: 'binary',
				identifier: '',
				sensor: true,
				min: 0,
				max: 1
			}
		]
	};

	var state = {
	  	value: 1
	};
	if(json.hasOwnProperty('Addr')) {
		param.device.identifier = (parseInt(json.period/10)*10) + `.` + parseInt(json.Addr) + `.` + parseInt(json.unit);
		param.device.protocol = `DIO`;
		state.value = json.value
	}else{
		param.device.identifier = (parseInt(json.period/10)*10) + `.` + param.device.identifier;
	}
	
	
	if(shared.getConfEn()){
		gladys.device.create(param);
	}
	return gladys.deviceState.createByIdentifier(param.device.identifier, param.device.service, param.types[0].type, state)
	.catch(function(e) {
		sails.log.warn('RadioEmitter error during creation or update of device : ' + e + '.');
		sails.log.warn('Pass to configuration mode if you want to add new devices.');
	});
};
