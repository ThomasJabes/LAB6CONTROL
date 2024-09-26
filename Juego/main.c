/*
 * Juego.c
 *
 * Created: 20/09/2024 11:15:38
 * Author : thoma
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Definir los pines de los botones
#define BOTON_IZQUIERDA PD7
#define BOTON_DERECHA PD6
#define BOTON_ABAJO PD5
#define BOTON_ARRIBA PD4
#define BOTON_B PD3
#define BOTON_A PD2

void UART_Init(unsigned int baud) {
	// Configuración del baud rate
	unsigned int ubrr = F_CPU/16/baud-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	// Habilitar transmisión y recepción
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);

	// Configuración de 8 bits de datos y 1 bit de parada
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void UART_Transmit(char data) {
	// Esperar hasta que el buffer esté vacío
	while (!(UCSR0A & (1<<UDRE0)));
	// Colocar el dato en el buffer y enviarlo
	UDR0 = data;
}

void UART_Transmit_String(const char* str) {
	while (*str) {
		UART_Transmit(*str++);
	}
}

int main(void) {
	// Configurar los pines como entradas con pull-up activado
	DDRD &= ~(1<<BOTON_IZQUIERDA) & ~(1<<BOTON_DERECHA) & ~(1<<BOTON_ABAJO) &
	~(1<<BOTON_ARRIBA) & ~(1<<BOTON_B) & ~(1<<BOTON_A);
	PORTD |= (1<<BOTON_IZQUIERDA) | (1<<BOTON_DERECHA) | (1<<BOTON_ABAJO) |
	(1<<BOTON_ARRIBA) | (1<<BOTON_B) | (1<<BOTON_A);

	// Inicializar UART a 9600 baud
	UART_Init(9600);

	while (1) {
		// Verificar cada botón y enviar la acción correspondiente
		if (!(PIND & (1<<BOTON_IZQUIERDA))) {
			UART_Transmit_String("Izquierda\n");
			_delay_ms(200); // Antirrebote
		}
		if (!(PIND & (1<<BOTON_DERECHA))) {
			UART_Transmit_String("Derecha\n");
			_delay_ms(200); // Antirrebote
		}
		if (!(PIND & (1<<BOTON_ABAJO))) {
			UART_Transmit_String("Abajo\n");
			_delay_ms(200); // Antirrebote
		}
		if (!(PIND & (1<<BOTON_ARRIBA))) {
			UART_Transmit_String("Arriba\n");
			_delay_ms(200); // Antirrebote
		}
		if (!(PIND & (1<<BOTON_B))) {
			UART_Transmit_String("X\n");
			_delay_ms(200); // Antirrebote
		}
		if (!(PIND & (1<<BOTON_A))) {
			UART_Transmit_String("Y\n");
			_delay_ms(200); // Antirrebote
		}
	}
}

