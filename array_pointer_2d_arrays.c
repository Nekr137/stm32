int foo(char ** data)
{
	uint8_t k = (uint8_t)data[0][0];
	return 1;
}

int main(void)
{
	char* arr2[] = { "AB", "CD" };
	char** data2 = arr2;
	foo(data2);	
}