#define F_CPU 16000000UL

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

int licznik = 0;
int cyfry[4];

ISR(TIMER0_COMP_vect) {
	if (licznik == 10000)
		licznik = 0;
	int ktoraCyfra = 0;
	int tmpLicznik = licznik;
	while (ktoraCyfra < 4) { // konwersja licznika na 4 cyfry
		int cyfra = tmpLicznik % 10;
		tmpLicznik /= 10;
		cyfry[ktoraCyfra] = cyfra;
		ktoraCyfra++;
	}
	licznik+= 4;
	PORTA = cyfra[cyfry[0]]; // 
	PORTB = 0x07; // wlaczenie ostatniej cyfry (zgaszenie pozostalych)
	_delay_ms(1); // to bylo w wersji gdzie licznik byl w funkcji main, na zajeciach nie zdazylem zaimplementowac timera, wiec nie wiem jak to zadziala z delayem w przerwaniu
	PORTA = cyfra[cyfry[1]];
	PORTB = 0x0b; // wlaczenie przedostatniej cyfry (zgaszenie pozostalych)
	_delay_ms(1);
	PORTA = cyfra[cyfry[2]];
	PORTB = 0x0d; // wlaczenie drugiej cyfry (zgaszenie pozostalych)
	_delay_ms(1);
	PORTA = cyfra[cyfry[3]];
	PORTB = 0x0e | (1 << PB7); // wlaczenie pierwszej cyfry (zgaszenie pozostalych), na zajeciach nie zrobilem kropki miedzy pierwsza a druga cyfra, 
								// ale jesli mialbym ja robic to zrobilbym to tak (PB7 przy zalozeniu ze kropka jest podpieta do pinu PB7)
	_delay_ms(1); 
}

int main(void)
{
    DDRA = 0xff; // porty A ustawione na wyjscie
    DDRB = 0xff; // porty B ustawione na wyjscie
    PORTA = 0x01; // poczatkowa cyfra (bez wiekszego znaczenia)
    PORTB = 0x0f; // poczatkowe wybranie ktore cyfry swieca (zadna)
    
	
	OCR0 = 250; // gdy timer dojdzie do 250 bedzie przerwanie (TIMER0_COMP, wywolane porownaniem aktualnego stanu licznika z wartoscia rejestru OCR0)
	TIMSK |= (1 << OCIE0); // ustawienie przerwania poprzez porownanie licznika z OCR0
	TCCR0 |= (1 << CS00 | 1 << CS01); // ustawienie preskalera na 64, dzieki temu czas sie "przeskaluje" i bedzie sie zliczal 64 razy wolniej
	// wynikowa konfiguracja daje jedno przerwanie na 250 podbic timera, ktore sie zdarza co 64 cykl zegar, czyli przerwanie bedzie co 16 000 cykli
	// daje to czestotliwosc przerwan 1000 Hz (co 1 ms)
	sei(); // aktywowanie globalnego systemu przerwan
    while(1)
    {
        
    }
}