
////////////////////////////
//                        //
//Get the data via XMLHTTP//
//                        //
////////////////////////////
var xhrRequest = function(url, type, callback){
  var xhr = new XMLHttpRequest();
  xhr.onload = function(){
    callback(this.responseText);
  };
  xhr.open(type,url);
  xhr.send();
};

///////////////////////////////////
//                               //
//Attempt to the the Weather Data//
//      Using device data        //
///////////////////////////////////
function getWeatherDevice(pos){
  
  //We will request the weather here
  // var url = "http://api.openweathermap.org/data/2.5/weather?lat=45.41&lon=-75.7";
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude;
  console.log("Before request");
  console.log(url);
  //send request

     xhrRequest(url, 'GET', 
            function(responseText){
              var json = JSON.parse(responseText);
              
              //Convert Kelvins
              var temperature = Math.round(json.main.temp - 273.15);
              console.log("Temperature is " + temperature);
              
              //conditions
              var conditions = json.weather[0].main;
              console.log("Conditions are " + conditions);
              
              var dictionary = {
                'KEY_TEMPERATURE': temperature,
                'KEY_CONDITIONS': conditions
              };
              //Send to pebble
              Pebble.sendAppMessage(dictionary,
                     function(e){
                       console.log("Weather info sent to Pebble Successfully");
                     },
                     function(e){
                       console.log("Error sending weather info to Pebble!");
                     });
            });
  

}

///////////////////////////////////
//                               //
//Attempt to the the Weather Data//
//           Via Hard Code       //
///////////////////////////////////
function getWeatherLocal(){
  
  //We will request the weather here
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=45.41&lon=-75.7";
  console.log("Before request");
  console.log(url);
  //send request

     xhrRequest(url, 'GET', 
            function(responseText){
              var json = JSON.parse(responseText);
              
              //Convert Kelvins
              var temperature = Math.round(json.main.temp - 273.15);
              console.log("Temperature is " + temperature);
              
              //conditions
              var conditions = json.weather[0].main;
              console.log("Conditions are " + conditions);
              
              var dictionary = {
                'KEY_TEMPERATURE': temperature,
                'KEY_CONDITIONS': conditions
              };
              //Send to pebble
              Pebble.sendAppMessage(dictionary,
                     function(e){
                       console.log("Weather info sent to Pebble Successfully");
                     },
                     function(e){
                       console.log("Error sending weather info to Pebble!");
                     });
            });
  

}


/////////////////////////////////
//                             //
//Handle the HTTP Request Error//
//                             //
/////////////////////////////////
function locationError(err){
  console.log("Error requesting location via api!");
  console.log("Check manually");
  //If there was an error just get the data the good old fashioned way
  getWeather_Local();
}


//////////////////////////////////////////////////////////////////////
//                                                                  //
//Attempt to get Device Co-Ords off of device.                      //
//If error log it and attempt to get Data from pre-coded url        //
//Needs a companion bit on pebble for user to select city or co-ords//
//                                                                  //
//////////////////////////////////////////////////////////////////////
function getWeather(){
  console.log("getting weather");
  navigator.geolocation.getCurrentPosition(
    getWeatherDevice,
    locationError,
    {timeout: 20000, maximumAge: 60000});
}




//Listen for when the watchface is opened
Pebble.addEventListener('ready',
                        function(e){
                          console.log("PebbleKit JS ready!");
                        
                         getWeather();
                       }
                       );

// List for when an AppMessage is received

Pebble.addEventListener('appmessage',
                       function(e){
                         console.log("AppMessage received!");
                         getWeather();
                       });