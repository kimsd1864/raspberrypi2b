#include <stdio.h>
#include <pigpio.h>

#define DHT_PIN 4  // BCM GPIO number

int read_dht11(int *humidity, int *temperature) {
    uint8_t last_state = PI_HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    uint8_t data[5] = {0};

    // Send start signal
    gpioSetMode(DHT_PIN, PI_OUTPUT);
    gpioWrite(DHT_PIN, PI_LOW);
    gpioDelay(18000);  // 18ms low
    gpioWrite(DHT_PIN, PI_HIGH);
    gpioDelay(40);     // 20~40us high

    gpioSetMode(DHT_PIN, PI_INPUT);

    // Read the response and 40 bits of data
    for (i = 0; i < 85; i++) {
        counter = 0;
        while (gpioRead(DHT_PIN) == last_state) {
            counter++;
            gpioDelay(1);  // 1us
            if (counter == 255) break;
        }
        last_state = gpioRead(DHT_PIN);

        if (counter == 255) break;

        // Skip the first 3 state changes (response signal)
        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (counter > 40)  // Likely a '1'
                data[j / 8] |= 1;
            j++;
        }
    }

    if (j >= 40) {
        uint8_t sum = data[0] + data[1] + data[2] + data[3];
        if (data[4] == (sum & 0xFF)) {
            *humidity = data[0];
            *temperature = data[2];
            return 0;  // Success
        } else {
            return -2; // Checksum error
        }
    } else {
        return -1;     // Not enough data
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("Failed to initialize pigpio\n");
        return 1;
    }

    int humidity = 0, temperature = 0;
    int result = read_dht11(&humidity, &temperature);

    if (result == 0) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 (code: %d)\n", result);
    }

    gpioTerminate();
    return 0;
}
