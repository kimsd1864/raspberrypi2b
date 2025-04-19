#include <stdio.h>
#include <pigpio.h>

#define DHT_PIN 4  // BCM GPIO 번호

int read_dht11(int *humidity, int *temperature) {
    uint8_t last_state = PI_HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    uint8_t data[5] = {0};

    // 핸드셰이크 시작
    gpioSetMode(DHT_PIN, PI_OUTPUT);
    gpioWrite(DHT_PIN, PI_LOW);
    gpioDelay(18000);  // 18ms
    gpioWrite(DHT_PIN, PI_HIGH);
    gpioDelay(40);     // 20~40us

    gpioSetMode(DHT_PIN, PI_INPUT);

    // 85개의 상태 변화 감지 (start + 40bit + 여유)
    for (i = 0; i < 85; i++) {
        counter = 0;
        while (gpioRead(DHT_PIN) == last_state) {
            counter++;
            gpioDelay(1);  // 1us
            if (counter == 255) break;
        }
        last_state = gpioRead(DHT_PIN);

        if (counter == 255) break;

        // 첫 3번 변화는 응답 시퀀스이므로 무시
        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (counter > 40)  // '1'일 확률 높음
                data[j / 8] |= 1;
            j++;
        }
    }

    if (j >= 40) {
        uint8_t sum = data[0] + data[1] + data[2] + data[3];
        if (data[4] == (sum & 0xFF)) {
            *humidity = data[0];
            *temperature = data[2];
            return 0;  // 성공
        } else {
            return -2; // 체크섬 에러
        }
    } else {
        return -1;     // 데이터 부족
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int humidity = 0, temperature = 0;
    int result = read_dht11(&humidity, &temperature);

    if (result == 0) {
        printf("온도: %d°C, 습도: %d%%\n", temperature, humidity);
    } else {
        printf("DHT11 읽기 실패 (코드: %d)\n", result);
    }

    gpioTerminate();
    return 0;
}
