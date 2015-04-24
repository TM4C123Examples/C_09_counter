#include "TM4C123.h"                    // Device header

void timer0Config(void);
void gpiofConfig(void);
int terminado=0;
int cont=0;
int main(){
	timer0Config();
	gpiofConfig();
	while(!terminado);
	GPIOF->DATA|=0x1<<1;
	while(1){
		cont++;
	}
}


void gpiofConfig(void){
	GPIOF->DEN|=0x1<<1;
	GPIOF->DIR|=0x1<<1;
	GPIOF->DATA&=~(0x1<<1);
}

void timer0Config(void){
	SYSCTL->RCGCGPIO|=0x3F;//Habilitamos todos los GPIO
	SYSCTL->RCGCTIMER|=(0x1<<0);//Habilitamos Timer0;
	//configuramos pin B6 como T0CCP0 para alimentar el counter 0;
	GPIOB->DEN|=(0x1<<6);
	GPIOB->AFSEL|=(0x1<<6);
	GPIOB->PCTL&=~(0xF<<24);
	GPIOB->PCTL|=(0x7<<24);
	
	
	TIMER0->CTL&=~((0x1<<0)|(0x1<<8));//Limpia los bits TbEN TaEN para desabilitar el timer a,b
																		//durante la configuracion
	
	TIMER0->CFG|=0x4;
	
	TIMER0->TAMR|=(0x3<<0);// Timer a en modo de captura
	TIMER0->TAMR&=~(0x1<<2);//modo contador de flancos
	TIMER0->CTL&=~((0x1<<2)|(0x1<<3));//Evento es flancos positivos
	TIMER0->TAMR&=~(0x1<<4);//counter down
	
	

	TIMER0->TAILR=25;
	TIMER0->TAPR=0;
	
	TIMER0->TAMATCHR=1;
	TIMER0->TAPMR=0;
	TIMER0->TAMR|=(0x1<<5);
	TIMER0->IMR|=0x1<<1;
	NVIC_EnableIRQ(TIMER0A_IRQn);
	
	TIMER0->CTL|=0x1<<0;
	
}


void TIMER0A_Handler(void){
		TIMER0->ICR=0x1<<1;
		terminado=1;
}
