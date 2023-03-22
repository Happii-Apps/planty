
// The Firestore payload upload callback function
void fcsUploadCallback(CFS_UploadStatusInfo info)
{
    if (info.status == fb_esp_cfs_upload_status_init)
    {
        Serial.printf("\nUploading data (%d)...\n", info.size);
    }
    else if (info.status == fb_esp_cfs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_cfs_upload_status_complete)
    {
        Serial.println("Upload completed ");
    }
    else if (info.status == fb_esp_cfs_upload_status_process_response)
    {
        Serial.print("Processing the response... ");
    }
    else if (info.status == fb_esp_cfs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

void setupFirebase() {
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  // /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  #if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
  #endif

  // // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  // For sending payload callback
  config.cfs.upload_callback = fcsUploadCallback;
}

void createDataPoint() {
  // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
  FirebaseJson content;

  timeClient.forceUpdate();
  formattedDate = timeClient.getEpochTime();

  // We will create the nested document in the parent path "a0/b0/c0
  // a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
  // and d? is the document id in the document collection id c0 which we will create.
  String documentPath = "Sensors/" + String("device_UUID_example") + "/data/" + String(formattedDate);

  // If the document path contains space e.g. "a b c/d e f"
  // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

  float moisture = takeMoistureReading(); 
  float humidity = readHumidity(); 
  float temp = readCelciusTemperature(); 
  float light = takeLightReading(); 

  // double
  content.set("fields/moisture/doubleValue", moisture);
  content.set("fields/humidity/doubleValue", humidity);
  content.set("fields/temp/doubleValue", temp);
  content.set("fields/light/doubleValue", light);

  // boolean
  // content.set("fields/temp/booleanValue", true);

  // // integer
  // content.set("fields/myInteger/integerValue", String(random(500, 1000)));

  // // null
  // content.set("fields/myNull/nullValue"); // no value set

  String doc_path = "projects/";
  doc_path += FIREBASE_PROJECT_ID;
  doc_path += "/databases/(default)/documents/coll_id/doc_id"; // coll_id and doc_id are your collection id and document id

  // // reference
  // content.set("fields/myRef/referenceValue", doc_path.c_str());

  formattedDate = timeClient.getEpochTime(); 
  content.set("fields/created_epoch/stringValue", formattedDate);   
  
  Serial.println(formattedDate);
  // content.set("fields/created/timestampValue", formattedDate); // fire UTC "Zulu" format

  // bytes
  // content.set("fields/myBytes/bytesValue", "aGVsbG8="); // base64 encoded

  // // array
  // content.set("fields/myArray/arrayValue/values/[0]/stringValue", "test");
  // content.set("fields/myArray/arrayValue/values/[1]/integerValue", "20");
  // content.set("fields/myArray/arrayValue/values/[2]/booleanValue", true);

  // // map
  // content.set("fields/myMap/mapValue/fields/name/stringValue", "wrench");
  // content.set("fields/myMap/mapValue/fields/mass/stringValue", "1.3kg");
  // content.set("fields/myMap/mapValue/fields/count/integerValue", "3");

  // // lat long
  // content.set("fields/myLatLng/geoPointValue/latitude", 1.486284);
  // content.set("fields/myLatLng/geoPointValue/longitude", 23.678198);

  Serial.print("Create a document... ");

  if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  else
    Serial.println(fbdo.errorReason());
}
