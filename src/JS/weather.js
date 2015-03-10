var xhrRequest = function(url, type, callback){
		var xhr = new XMLHttpRequest();
		xhr.onload = function(){
			callback(this.responseText);
		};
		xhr.open(type,url);
		xhr.send();
	};



function locationSuccess(pos){
	//We will request the weather here
	var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude;

	xhrRequest(url, 'GET', 
		function(responseText){
			//Response text will have the JSON object
			var json = JSON.parse(responseText);
			var temperature = Math.round(json.main.temp - 273.15)
			console.log("Temp is" + temperature);

			//conditions
			var conditions = json.weather[0].main;
			console.log("Conditions are " + conditions);

		});
	
}

var dictionary = {
	'KEY_TEMPERATURE': temperature;
	'KEY_CONDITIONS': conditions
};

//send the data to pebble
Pebble.sendAppMessage(dictionary,
	function(e){
		console.log("Weather info sent!");
	},
	function(e){
		console.log("Error sending weather data");
	});

function locationError(err){
	console.log("Error Requesting Location");
}

function getWeather(){
	navigator.geolocation.getCurrentPosition(
		locationSuccess,
		locationError
		{timeout: 15000, maximumAge: 60000}
		);
}


//Listen for when watchface is ready
Pebble.addEventListener('ready',
		function(e){
			console.log("PebbleKit JS ready!");
		}

		getWeather();
		);

//Listen for when an AppMessage is received
Pebble.addEventListener('appmessage'){
	function(e){
		console.log("AppMessage received");
		getWeather();
	}

}


