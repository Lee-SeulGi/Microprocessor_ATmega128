// LED 4개를 5초 간격으로 점등, 소등 반복하기

#include <avr/io.h>		// AVR의 I/O 레지스터를 사용하므로 헤더파일 포함
#include <util/delay.h>	// _delay_ms() 함수를 사용하기 위해 헤더파일 포함

int main() {
	DDRA = 0xff;	// PA0 ~ PA7핀을 출력으로 설정

	// LED 4개씩 점등, 소등시키는 동작을 계속 반복
	while(1) {
		PORTA = 0xf0;	// LED Y0~Y3 점등, Y4~Y7 소등
		_delay_ms(5000);	// 5초 대기
		PORTA = 0x0f;	// LED Y0~Y3 소등, Y4~Y7 점등
		_delay_ms(5000);	// 5초 대기
	}
}
