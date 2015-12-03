#define F_CPU 1000000UL // u mnie jest inne taktowanie zegara (1 MHz)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


uint8_t cyfra[] = { // deklaracja tablicy mapujacej cyfry na segmenty wyswietlacza
	~0b00111111,
	~0b00000110,
	~0b01011011,
	~0b01001111,
	~0b01100110,
	~0b01101101,
	~0b01111101,
	~0b00000111,
	~0b01111111,
	~0b01101111
};

int licznik = 0; // mierzy ms ktore uplynely
int cyfry[4];

ISR(TIMER0_COMP_vect) {
	licznik += 2; // na moim mikrokontrolerze jest inne taktowanie zegara, stad podbijam licznik o 2
	if (licznik >= 10000)
		licznik = 0;
}

int main(void)
{
    DDRA = 0xff; // porty A ustawione na wyjscie
    DDRB = 0xff; // porty B ustawione na wyjscie
    PORTA = cyfra[0]; // poczatkowa cyfra (bez wiekszego znaczenia)
    PORTB = 0x0f; // poczatkowe wybranie ktore cyfry swieca (zadna)
	
	OCR0 = 125; // ta wartosc zalezy od tej w linii 39 i u mnie tez jest inna niz na zajeciach
	TIMSK |= (1 << OCIE0);
	TCCR0 |= (1 << CS01); // u mnie poprawnie liczy przy preskalerze ustawionym na 8
	sei();

    while (1) 
    {
		int ktoraCyfra = 0;
		int tmpLicznik = licznik;
		while (ktoraCyfra < 4) { // konwersja licznika na 4 cyfry
			int cyfra = tmpLicznik % 10;
			tmpLicznik /= 10;
			cyfry[ktoraCyfra] = cyfra;
			ktoraCyfra++;
		}
		PORTA = cyfra[cyfry[0]]; //
		PORTB = 0x01; // wlaczenie ostatniej cyfry (zgaszenie pozostalych)
		_delay_ms(1);

		PORTA = cyfra[cyfry[1]];
		PORTB = 0x02; // wlaczenie przedostatniej cyfry (zgaszenie pozostalych)
		_delay_ms(1);

		PORTA = cyfra[cyfry[2]];
		PORTB = 0x04; // wlaczenie drugiej cyfry (zgaszenie pozostalych)
		_delay_ms(1);

		PORTA = cyfra[cyfry[3]] ^ (1 << PA7); // ostatnia cyfre wyswietlamy razem z kropka (przy zalozeniu ze jest na pinie PA7)
		PORTB = 0x08; // wlaczenie pierwszej cyfry (zgaszenie pozostalych)
		_delay_ms(1);
    }
}

