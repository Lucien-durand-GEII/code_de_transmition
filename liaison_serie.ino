/*! \mainpage
*   \section introd introduction
*           intro a completer
*
*   \section deviceinfo information sur les composant utiliser
*            une arduinoMega
*    \section contacteinfo pour plus de question
*               Pour plus d'information 
*               Lucien Durand 06 82 22 50 37
*/

/*! \file *********************************************************************
 *
 * \brief
 *      code permettant de transmettre des donner d'une arduino a un port serie.
 *      ici il transmet B a l'infinie
 *
 * \par Application note:
 *      
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Lucien Durand\n
 *      Support email: lucien.durand.75@gmail.com
 *
 * $Name: Lucien Durand$
 * $Revision: 1.8 $
 * $RCSfile: code de transmition de donner$
 * $Date: 23/12/2024 09:38$  \n
 ******************************************************************************/

/*! \brief calcule
* dans l'ordre des lignes
* vitesse de l'orloge ici 16MHz
* nombre de baud
* calcule du port ici 103
*/
#define FOSC 16000000 // vitesse de l'orloge ici 16MHz
#define BAUD 9600 //nombre de baud
#define MYUBRR FOSC/16/BAUD-1 // calcule du port ici 103
unsigned  char data;
unsigned char flag = 0;
/*! \brief initialisation
 * ici on trouve toute les initialisations qui vont du nombre de baud, jusqu'a 
 * l'initialisation du canal de transmition
 */
void USART_Init(unsigned int ubrr)
{
UBRR0H = (unsigned char)(ubrr>>8);/*Set baud rate */
UBRR0L = (unsigned char)ubrr;
UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);/*bit pour la transmtion seul ici UCROB = 0000100*/
UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);/* bit pour  ici UCSR0C = 00000011*/
DDRB = 0b00100000;
PORTB = 0b0;
}

/*! \brief fonction de transmition
 * on creer une fonction qui va transmettre des donner
 */
void USART_Transmit(unsigned char data)
{
while (!(UCSR0A & (1<<UDRE0)));/* on attend que la transmition soit finis */
UDR0 = data;/* Put data into buffer, sends the data */
}

unsigned char USART_Receive(void)
{
while (!(UCSR0A & (1<<RXC0)));
return UDR0;
}

ISR(USART_RX_vect){
  data = UDR0;
  flag =1;
  PORTB = 0b00100000;
}

void USART_puts(unsigned char *str)
{
do
{
USART_Transmit(*str);
} while (*++str!=0);
}

int main(void)
{
USART_Init(MYUBRR);
sei();
  while(1){
    //USART_Transmit(USART_Receive()+1);
    _delay_ms(1);
    if (flag == 1){
      //USART_Transmit(data);
      USART_puts("IUT DE Troyes 10");
      flag = 0;
    }
    
  }
}
