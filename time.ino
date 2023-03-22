void setupTimeClient() {
  timeClient.begin();
}

void updateTime() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
}