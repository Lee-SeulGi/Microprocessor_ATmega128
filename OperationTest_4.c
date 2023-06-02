// FND에 7초 간격으로 숫자 0~3 표시하기

#include <avr/io.h>
#include <util/delay.h>

int main(){
	DDRA = 0xff;	// 포트 A 출력
	DDRB = 0xff;	// 포트 B 출력
	PORTB = 0x00;	// FND S0, S1, S2, S3에 표시

	while(1) {
		PORTA = 0xc0;	// FND에 0 표시
		_delay_ms(7000);	// 7초 대기
		PORTA = 0xf9;	// FND에 1 표시
		_delay_ms(7000);	// 7초 대기
		PORTA = 0xa4;	// FND에 2 표시
		_delay_ms(7000);	// 7초 대기
		PORTA = 0xb0;	// FND에 3 표시
		_delay_ms(7000);	// 7초 대기
	}
}
