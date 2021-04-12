#include "math.h"
/*** IMPLEMENTASI DIV DAN MOD ***/
int mod(int a, int m)
{
	return a - (div(a, m) * m);
}

int div(int a, int b)
{
	int result = 0;
	int i, A = a, B = b;
	char modifier = 1;
	if(b == 0) return result;
	
	if (a < 0)
	{
		modifier = -modifier;
		A = -a;
	}
	if (b < 0)
	{
		modifier = -modifier;
		B = -b;
	}
	
	for(i = B; i <= A; i += B)
	{
		result++;
	}
	return result * modifier;
}
/*** IMPLEMENTASI DIV DAN MOD ***/

int pow(int a, int b){
	int temp;
	if(a == 0) return 0;
	if(b == 0) return 1;
	if(b == 1) return a;
	temp = pow(a, b/2);
	return temp * temp * pow(a, mod(b, 2));
}

int abs(int x){ return x < 0 ? -x : x; }