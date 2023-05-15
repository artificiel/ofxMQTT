#include "ofApp.h"

void ofApp::setup(){
  client.begin("127.0.0.1", 1883);
  client.connect("openframeworks", "public", "public");

	// if threaded, watch out for thread sync issues
  ofAddListener(client.onOnline, this, &ofApp::onOnline);
  ofAddListener(client.onOffline, this, &ofApp::onOffline);

}

void ofApp::update() {
	
	// tight!
	while (auto m = client.getNextMessage()) {
		ofLogNotice("thread safe") << m->getTopic() << " " << m->getPayload() << " " << ofIsCurrentThreadTheMainThread();
	}
}

void ofApp::exit(){
  client.disconnect();
}

void ofApp::onOnline(){
  ofLog() << "online";

  client.subscribe("hello");
}

void ofApp::onOffline(){
  ofLog() << "offline";
}

void ofApp::keyPressed(int key){
  client.publish("hello", "world");
}
