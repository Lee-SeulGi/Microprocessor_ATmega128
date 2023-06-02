// Tact Switch LED 점등, 소등시키기

#include <avr/io.h>

int main(){
	DDRF = 0xf0;// PF0~PF3 포트를 입력 모드로 사용
	DDRA = 0x0f;// PA0~PA3 포트를 출력 모드로 사용

	while(1) {
		PORTA = PINF;// PINF 값을 PORTA로 출력
	}
}
