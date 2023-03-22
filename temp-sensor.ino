
void setupTempSensor() {
  dht_sensor.begin(); // initialize the DHT sensor
}

float readHumidity() {
  float humidity = dht_sensor.readHumidity();
  if (isnan(humidity)) {
    return 0; 
  }
  return humidity; 
}

float readCelciusTemperature() {
  float tempC = dht_sensor.readTemperature();
  if (isnan(tempC)) {
    return 0; 
  }
  return tempC; 
}

float readFarTemperature() {
  float tempF = dht_sensor.readTemperature(true);
  if (isnan(tempF)) {
    return 0; 
  }
  return tempF; 
}