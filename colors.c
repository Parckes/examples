#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}Color;

// считать RGB-формат с консоли
Color getColor();
// перевод из RGB-формата в число
unsigned long long convertToHTML(Color a);
// преобразование числа цвета в RGB-формат
Color convertToRGB(unsigned long long h);
// печать цвета в RGB-формате (печать значений в десятичном виде через пробел)
// red green blue: 
// 255 128 222
// Печатать только числа!!
void printRGB(Color b);

// печать цвета в HTML-формате. 
// Примеры: FFA902 0AA3FF
void printHTML(Color c);

int main(){
	Color z, z2;
	unsigned long long html;
	z = getColor();
	printRGB(z);
	html = convertToHTML(z);
	printf("%llu\n", html);
	printHTML(z);
	z2 = convertToRGB(html);
	printRGB(z2);
	return 0;
}

Color getColor()
{
	Color scan;	
	unsigned char red1 = 0, green1 = 0, blue1 = 0;
	scanf("%hhu %hhu %hhu", &red1, &green1, &blue1);
	scan.red = red1;
	scan.green = green1;
	scan.blue = blue1;
	return scan;
}

unsigned long long convertToHTML(Color a)
{
	unsigned long long to_html = a.blue + 256 * a.green + 65536 * a.red;
	
	return to_html;
}

Color convertToRGB(unsigned long long h)
{
	Color r;
	r.red = h / (256 * 256);
	h %= (256 * 256);
	r.green = h / 256;
	h %= 256;
	r.blue = h;
	return r;
}

void printRGB(Color b)
{
	printf("%hhu %hhu %hhu\n", b.red, b.green, b.blue);
}

void printHTML(Color c)
{
	printf("%02X%02X%02X\n", c.red, c.green, c.blue);
}
