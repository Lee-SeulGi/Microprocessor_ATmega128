// Slide Switch LED 점등, 소등시키기

#include <avr/io.h>

int main(){
	unsigned char var1;

	DDRA = 0xff;	// 포트 A 출력으로 설정
	DDRC = 0x00;	// 포트 C 입력으로 설정
	PORTA = 0xff;	// LED Y0~Y7 소등

	while(1) {
		var1 = PINC & 0x0f;	// 포트 C 상위 4비트 마스크

		if(var1 == 0b00000001)		// X0 HIGH, 나머지 스위치 LOW이면
			PORTA = 0b11110001;	// LED Y0 소등, 나머지 LED 점등
		else if(var1 == 0b00000010)	// X1 HIGH, 나머지 스위치 LOW이면
			PORTA = 0b11110010;	// LED Y1 소등, 나머지 LED 점등
		else if(var1 == 0b00000100)	// X2 HIGH, 나머지 스위치 LOW이면
			PORTA = 0b11110100;	// LED Y2 소등, 나머지 LED 점등
		else if(var1 == 0b00001000)	// X3 HIGH, 나머지 스위치 LOW이면
			PORTA = 0b11111000;	// LED Y3 소등, 나머지 LED 점등
		else
			PORTA = 0xff;	// LED Y0~Y7 소등
	}
}
