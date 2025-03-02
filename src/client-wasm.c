/*

Copied from: tool/emsdk/upstream/emscripten/test/websocket/test_websocket_send.c

*/
#include <emscripten/websocket.h>
#include <stdlib.h>
#include <stdio.h>

EMSCRIPTEN_WEBSOCKET_T sock;

bool WebSocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData) {
    printf("WebSocket connection opened.\n");
    emscripten_websocket_send_utf8_text(sock, "Hello from Emscripten!");
    return 0;
}


bool WebSocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData) {
    printf("Received message: %s\n", (char*)e->data);
    return 0;
}


int main() {
    printf("Starting!\n");
    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);
    attr.url = "ws://localhost:8000/websocket";
    // attr.protocols = "binary,base64";  // <- I think this was the problem!
    attr.protocols = NULL;  // Only appears to work with no protocol at the moment
    printf("attr.url: %s\n", attr.url);
    
    sock = emscripten_websocket_new(&attr);
    printf("sock: %d\n", sock);
    if (sock < 0) {
        printf("Error creating WebSocket: %d\n", sock);
        return 1;
    }
    emscripten_websocket_set_onopen_callback(sock, (void*)0x42, WebSocketOpen);
    emscripten_websocket_set_onmessage_callback(sock, (void*)0x45, WebSocketMessage);
    printf("Done!\n");

  return 0;
}