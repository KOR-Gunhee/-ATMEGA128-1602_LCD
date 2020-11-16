#define F_CPU 16000000UL
// ATmega128�� �������� ���� ���ǵǾ� ����
#include <avr/io.h>

// _delay_ms() �Լ� ���� ���ǵǾ� ����
#include <util/delay.h>

#define sbi(x, y) (x |= (1 << y))  // x�� y ��Ʈ�� ����(1)
#define cbi(x, y) (x &= ~(1 << y)) // x�� y ��Ʈ�� Ŭ����(0)

// CON ��Ʈ�� ��Ʈ C�� ������� ����
#define LCD_CON      PORTC
// DATA ��Ʈ�� ��Ʈ A�� ������� ����
#define LCD_DATA     PORTA
// DATA ��Ʈ�� ��� ���� ���� ��ũ�θ� ����
#define LCD_DATA_DIR DDRA
// DATA ��Ʈ�� �Է� ���� ���� ��ũ�θ� ����
#define LCD_DATA_IN  PINA
// RS ��ȣ�� ��Ʈ ��ȣ ����
#define LCD_RS   0
// RW ��ȣ�� ��Ʈ ��ȣ ����
#define LCD_RW   1
// E ��ȣ�� ��Ʈ ��ȣ ����
#define LCD_E    2

// �ؽ�Ʈ LCD�� ���� ����(���)�� �д� �Լ�
unsigned char LCD_rCommand(void){
	unsigned char temp=1;
	
	LCD_DATA_DIR = 0X00;
	
	cbi(LCD_CON, LCD_RS); // 0�� ��Ʈ Ŭ����, RS = 0, ���
	sbi(LCD_CON, LCD_RW); // 1�� ��Ʈ ����, RW = 1, �б�
	sbi(LCD_CON, LCD_E);  // 2�� ��Ʈ ����, E = 1
	_delay_us(1);
	
	temp = LCD_DATA_IN;      // ��� �б�
	_delay_us(1);
	
	cbi(LCD_CON, LCD_E);  // ��� �б� ���� ��
	
	LCD_DATA_DIR = 0XFF;
	_delay_us(1);
	
	return temp;
}

// �ؽ�Ʈ LCD�� ���� �÷��� ���¸� Ȯ���ϴ� �Լ�
char LCD_BusyCheck(unsigned char temp){
	if(temp & 0x80)          return 1;
	else            return 0;
}

// �ؽ�Ʈ LCD�� ����� ����ϴ� �Լ� - ��, �����÷��� üũ���� ����
void LCD_wCommand(char cmd){
	cbi(LCD_CON, LCD_RS); // 0�� ��Ʈ Ŭ����, RS = 0, ���
	cbi(LCD_CON, LCD_RW); // 1�� ��Ʈ Ŭ����, RW = 0, ����
	sbi(LCD_CON, LCD_E);  // 2�� ��Ʈ ����, E = 1
	
	LCD_DATA = cmd;          // ��� ���
	_delay_us(1);
	cbi(LCD_CON, LCD_E);  // ��� ���� ���� ��
	
	_delay_us(1);
}

// �ؽ�Ʈ LCD�� ����� ����ϴ� �Լ� - ��, �����÷��� üũ��
void LCD_wBCommand(char cmd){
	while(LCD_BusyCheck(LCD_rCommand()))
	_delay_us(1);
	cbi(LCD_CON, LCD_RS); // 0�� ��Ʈ Ŭ����, RS = 0, ���
	cbi(LCD_CON, LCD_RW); // 1�� ��Ʈ Ŭ����, RW = 0, ����
	sbi(LCD_CON, LCD_E);  // 2�� ��Ʈ ����, E = 1
	
	LCD_DATA = cmd;          // ��� ���
	_delay_us(1);
	cbi(LCD_CON, LCD_E);  // ��� ���� ���� ��
	
	_delay_us(1);
}

// �ؽ�Ʈ LCD�� �ʱ�ȭ�ϴ� �Լ�
void LCD_Init(void){
	_delay_ms(100);
	// ���� �÷��׸� üũ���� �ʴ� Function Set
	LCD_wCommand(0x38);
	_delay_ms(10);
	// ���� �÷��׸� üũ���� �ʴ� Function Set
	LCD_wCommand(0x38);
	_delay_us(200);
	// ���� �÷��׸� üũ���� �ʴ� Function Set
	LCD_wCommand(0x38);
	_delay_us(200);
	
	// ���� �÷��׸� üũ�ϴ� Function Set
	LCD_wBCommand(0x38);
	// ���� �÷��׸� üũ�ϴ� Display On/Off Control
	LCD_wBCommand(0x0c);
	// ���� �÷��׸� üũ�ϴ� Clear Display
	LCD_wBCommand(0x01);
}

// �ؽ�Ʈ LCD�� 1����Ʈ �����͸� ����ϴ� �Լ�
void LCD_wData(char dat){
	while(LCD_BusyCheck(LCD_rCommand()))
	_delay_us(1);
	
	sbi(LCD_CON, LCD_RS); // 0�� ��Ʈ ����, RS = 1, ������
	cbi(LCD_CON, LCD_RW); // 1�� ��Ʈ Ŭ����, RW = 0, ����
	sbi(LCD_CON, LCD_E); // 2�� ��Ʈ ����, E = 1
	
	LCD_DATA = dat;       // ������ ���
	_delay_us(1);
	cbi(LCD_CON, LCD_E);  // ������ ���� ���� ��
	
	_delay_us(1);
}

// �ؽ�Ʈ LCD�� ���ڿ��� ����ϴ� �Լ�
void LCD_wString(char *str){
	while(*str)
	LCD_wData(*str++);
}

// C ����� �� ���� �Լ�
int main(void){
	// ��Ʈ A�� ���� ����, 0 : �Է�, 1 : ���
	DDRA = 0B11111111;
	
	// ��Ʈ C�� ���� ����, 0 : �Է�, 1 : ���
	DDRC = 0B11111111;
	
	LCD_Init();         // �ؽ�Ʈ LCD �ʱ�ȭ - �Լ� ȣ��

	
	LCD_wBCommand(0x80 | 0x00);  // DDRAM Address = 0 ����
	LCD_wString("IP : 127.0.0.1");     // �ؽ�Ʈ LCD ���ڿ� ���
	
	
	LCD_wBCommand(0x80 | 0x40);  // DDRAM Address = 0x40 ����
	LCD_wString("PORT : 10000"); // WESNET ���ڿ� ���
	
	// �Լ��� ���¿� ���� ������(int)�� ���� ��ȯ��
	return 1;
}

