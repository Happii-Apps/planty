
float takeMoistureReading() {

  float soilSensorValue = 0; 

  for (int i = 0; i <= 100; i++) 
  { 
    float newValue = analogRead(SOIL_MOISTURE_SENSOR_ANALOG_PIN);
    Serial.println("Mositure Value [" + String(i) + "]: " + String(newValue)); 
    soilSensorValue = soilSensorValue + newValue; 
    delay(1); 
  } 

  soilSensorValue = soilSensorValue/100.0; 

  return soilSensorValue; 
}