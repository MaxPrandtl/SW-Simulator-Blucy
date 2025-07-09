BROKER="localhost"
TOPIC="clock/timestamp"
RATE=0.1
while true; do
    # Get the current timestamp in milliseconds
    TIMESTAMP=$(date +%s%3N)
    # Publish to the MQTT broker
    mosquitto_pub -h "$BROKER" -t "$TOPIC" -m "$TIMESTAMP" -d
    # Sleep for the specified rate
    sleep "$RATE"
done