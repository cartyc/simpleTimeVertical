


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
              var kelvins, celcius, farenheight;
              //Convert Kelvins
              kelvins = Math.round(json.main.temp);
              celcius = kelvins - 273.15;
              farenheight = (celcius * 1.8000) + 32.00;

              console.log("Temperature is " + kelvins);
              
              //conditions
              var conditions = json.weather[0].main;
              console.log("Conditions are " + conditions);
              
              var dictionary = {
<<<<<<< HEAD
                'KEY_TEMPERATURE': kelvins,
                'KEY_CONDITIONS': conditions,
=======
                'KEY_TEMPERATURE': celcius,
                'KEY_CONDITIONS': conditions
>>>>>>> master
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
    {timeout: 10000, maximumAge: 60000});
}




//Listen for when the watchface is opened
Pebble.addEventListener('ready',
                        function(e){
                          console.log("Ready for action!");
                        
                         getWeather();
                       }
                       );

// List for when an AppMessage is received

Pebble.addEventListener('appmessage',
                       function(e){
                         console.log("AppMessage received!");
                         getWeather();
                       });