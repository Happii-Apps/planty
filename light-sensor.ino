float takeLightReading() {

  float sensorValue; 
  float sensorVoltage; 

  sensorValue = analogRead(GUVA_SENSOR_PIN);
  sensorVoltage = sensorValue / 4095 * 3.3;
  
  Serial.print("sensor reading = ");
  Serial.print(sensorValue);
  Serial.println("");
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");

  return 0; 
}