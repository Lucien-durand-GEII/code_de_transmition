/*! \mainpage
*   \section introd introduction
*            Durand une durée de 8 semaine nous avons realiser un code de transmition et reception de données.
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
 *      Reception/transmition de donnée, ici sous forme de texte.
 *      il transmet le carractere a chaque demande, presse de la touche entrée par exemple, et s'arrette en attendant la prochaine  *      demande. la fonction principale de ce code est de transmettre une lettre via une transmition, et de recevoir via une autre. le  *      code va attendre que un des "flag"* soit lever pour lire ce dernier et l'envoyer sur le canale suivant. 
 *
 * \par Application note:
 *      
 *
 * \par Documentation
 *     Dattasheet Mega, page 223
 *	compilateur : arduino
 *     
 *
 * \author
 *      Lucien Durand\n
 *      Support email: lucien.durand.75@gmail.com / lucien.durand@etudiant.univ-reims.fr
 *
 * $Name: Lucien Durand$
 * $Revision: 1.8 $
 * $RCSfile: code de transmition/reception de donner$
 * $Date: 14/12/2024 07:58$  \n
 ******************************************************************************/


#define FOSC 16000000 // vitesse de l'orloge ici 16MHz
#define BAUD 9600 //nombre de baud
#define MYUBRR FOSC/16/BAUD-1 // calcule du port ici 103
/*! \brief calcule et initialisation
* pour commencer il fallais definir quelque petit parametre :
* on a commencer par definir la vitesse de l'horloge que nous avons fixer a 16 Mhz car c'est la vitesse de notre arduino
* ensuite le nombre de baud/seconde pour avoir le temps de comprendre nous avons choisie une vitesse assez faible de 9600baude/seconde
*et enfin nous avons choisie le bon port ici c'est le port UBRR0 103
*/

/*! \brief calcule et initialisation
*nous avons du utiliser des variable qui on permit de stoquer des donner et de signaler que nous avons bien recu quelque chose (comme avec une boite au lettre ou le facteur releve le drapeau pour signaler quil est passer
*il existe, dan sle code, 2 cannaux de transmition et 2 de reception donc chaque variable existe pour le 0 ou le 3
*/
unsigned  char data0;
unsigned char flag0 = 0;

unsigned  char data3;
unsigned char flag3 = 0;


void USART0_Init(unsigned int ubrr)// fonction pour le 0 
{
UBRR0H = (unsigned char)(ubrr>>8);/*Set baud rate */
UBRR0L = (unsigned char)ubrr;
UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);

UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void USART3_Init(unsigned int ubrr) // fonction pour le 3 
{
UBRR3H = (unsigned char)(ubrr>>8);/*Set baud rate */
UBRR3L = (unsigned char)ubrr;
UCSR3B = (1<<RXCIE3)|(1<<RXEN3)|(1<<TXEN3);
UCSR3C = (1<<UCSZ31)|(1<<UCSZ30);
/*! \brief initialisation
 *il nous faut configurer les cannaux de transmtion et de reception
 * ici c'est pour le 0 mais cela marche aussi pour le 3
 */
 /*! \brief bit pour la transmtion TXEN0 pour la transmition, RXEN0 pour la reception, RXCIE0 pour la transmition avec interuption
 * pour la transmition et et la reception nous utilisons le registre UCSR0B ou nous metton les bits TX et RX a 1 afin d'activer les fonctionalitée desirer
 */
void USART0_Transmit(unsigned char data){ 
while (!(UCSR0A & (1<<UDRE0)));
UDR0 = data0;
}
void USART3_Transmit(unsigned char data){ 
while (!(UCSR3A & (1<<UDRE3)));
UDR3 = data3;
}
/*! \brief fonction de transmition
 * on creer une fonction qui va transmettre des donners
 * cette fonction commence par attendre que UCSR0A est changer et que UDRE0 se soit mis a 1
 * si tout les condition sont reunie alors on attribut la valeur de data0 a UDR0
 * cela marche aussi pour le 3 par le meme principe
 */

unsigned char USART0_Receive(void){
while (!(UCSR0A & (1<<RXC0)));
return UDR0;
}
unsigned char USART3_Receive(void){
while (!(UCSR3A & (1<<RXC3)));
return UDR3;
}
/*! \brief fonction de reception
 * on creer une fonction qui va recvoir des donners
 * cette fonction commence par attendre que UCSR0A est changer et que RXC0 se soit mis a 1
 * si tout les condition sont reunie alors on renvoie la valeur stoquer dans UDR0
 * cela marche aussi pour le 3 par le meme principe
 */

ISR(USART0_RX_vect){ 
 // data0 = UDR0;
  flag0 =1;
//  PORTB = 0b00100000;
}
ISR(USART3_RX_vect)
 /// data3 = UDR3;
  flag3 =1;
}
/*! \brief interuption
 * on creer une fonction qui fais une interuption
 * on attribut UDR0 a data0et on leve le flag correspondant pour signaler que "le facteur est bien passer"
 * cela marche aussi pour le 3 par le meme principe
 */

void USART_puts(unsigned char *str){
  do{
    USART_Transmit(*str);
  } 
  while (*++str!=0);
}
/*! \brief fonction de d'ecriture (non utiliser ici)
 * on creer une fonction qui va ecrire correctement sur le moniteur arduino ou GTK therme
 * cette fonction fais comme la fonction precedente mais elle met tout a la ligne pour facilite la lecture
 */
void USART_putsln(unsigned char *str){
  USART_puts(str);
  USART_Transmit('\n');
  USART_Transmit('\r');
}



int main(void){
USART0_Init(MYUBRR);
USART3_Init(MYUBRR);
//DDRB = 0b00100000; 
//PORTB = 0b0;
sei();
  while(1){
    //USART_Transmit(USART_Receive()+1);
    //_delay_ms(1);
    if (flag0 == 1){
      USART3_Transmit(UDR0);
      //USART_Transmit(data);
      //USART_putsln("IUT DE Troyes 10");
      flag0 = 0;
    }
   if (flag3 == 1){
      USART0_Transmit(UDR3);
      //USART_Transmit(data);
      //USART_putsln("IUT DE Troyes 10");
      flag3 = 0;
    }
/*! \brief fonction principale
 * on commence par lancer l'initialisation des canaux
 * on autorise les interuption sinon elle ne se declancherons pas
 * ensuite temps que on ne recois pas 1
 * 	on regarde la valeur du flag0 et si il vaur 1alors on transmet UDR0 et on remet le flag a 1(on a receptioner notre courier)
 * ensuite on fais de meme avec le flag3 et UDR3
 */
  }
}
