#include "mosquitto.h"
#include "ofMain.h"
#include "ofThreadChannel.h"

struct ofxMQTTMessage {
  string topic;
  string payload;
  bool retain;
  int qos;

  auto getPayload() { return payload; }
  auto getTopic() { return topic; }
  auto getRetain() { return retain; }
  auto getQos() { return qos; }

};

class ofxMQTT {
 private:
  struct mosquitto *mosq;
  bool alive = false;
  size_t received_messages{0};

  string hostname;
  int port;
  string clientId;
  string username;
  string password;
  string willTopic;
  string willPayload;

  int mid = 0;
  int nextMid();
  bool threaded;
  ofThreadChannel<ofxMQTTMessage> messagesChannel;

 public:
  ofxMQTT();
  ofxMQTT(bool threaded = false);
  ~ofxMQTT();

  void begin(string hostname);
  void begin(string hostname, int port);
  void setWill(string topic);
  void setWill(string topic, string payload);
  bool connect(string clientId);
  bool connect(string clientId, string username, string password);
  void publish(string topic, int qos = 0, bool retain = false);
  void publish(string topic, string payload, int qos = 0, bool retain = false);
  void subscribe(string topic, int qos = 0);
  void unsubscribe(string topic);
  void update();
  bool connected();
  void disconnect();

  std::optional<ofxMQTTMessage> getNextMessage();
  std::string lib_version();

  ofEvent<void> onOnline;
  ofEvent<ofxMQTTMessage> onMessage;
  ofEvent<void> onOffline;

  // never call these functions:
  void _on_connect(int rc);
  void _on_disconnect(int rc);
  void _on_message(const struct mosquitto_message *message);
};

// prefer explicit name than opaque bool arg
// and the use cases are probably not dynamic "swap-in"
class ofxThreadedMQTT : public ofxMQTT {
 public:
  ofxThreadedMQTT() : ofxMQTT(true){};
};
