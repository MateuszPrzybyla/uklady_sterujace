#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int licznik = 0;

void blink() {
    while(1) {
        PORTA ^= 0x01; // zamiana najmlodzszego bitu na przeciwny
        _delay_ms(1000);
    }
}

ISR(INT0_vect) // definicja funkcji wywolywanej w momencie wykrycia przerwania INT0
{
	licznik++; 
    PORTA = (PORTA & 0x01) | (licznik << 1); // najmlodszy bit zapamietujemy, 
	// a na pozostale bity (od 2 do najstarszego) wkladamy wartosc licznika (stad przesuniecie bitowe o 1 w lewo)
}

int main(void)
{
    DDRA = 0xff; // ustawienie wszystkich portow A na tryb out
    DDRD = 0x00; // ustawienie wszystkich portow D na tryb in
    PORTA = 0x00; // poczatkowa inicjalizacja portu A (LED)
    PORTD = (1 << PD2);
    MCUCR |= (1 << ISC01); // ustawienie przerwania, zostanie wywolane gdy na wejsciu INT0 pojawi sie zbocze opadajace
	                       // jest to bit ISC01 w rejestrze MCUCR
    GICR |= (1 << INT0);   // aktywacja przerwania INT0 (bit INT0 w rejestrze GICR)
    sei(); // aktywacja systemu przerwan ("globalna")
    blink(); // rozpoczecie mrugania skrajna dioda (najmlodszy bit portu A)
}
