package com.autovibe;

public class ConnectionHolder {
    private static MqttHandler handler;

    public static void setHandler(MqttHandler h) {
        handler = h;
    }

    public static MqttHandler getHandler() {
        return handler;
    }
}
