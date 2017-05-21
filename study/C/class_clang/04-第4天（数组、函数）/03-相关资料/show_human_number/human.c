#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 最多支持64位无符号数值的转换，且编译器须支持C99标准 */

int is_number_legal(char *str)
{
	int i;
	int length = 0;
	
	if(str[0]=='0' && str[1]=='x' || str[1]=='X'){
		str +=2;
		length = strlen(str);
		if(length <= 16){
			for(i=0;i<length;i++){/* 数值是否合法 */
				if(!((str[i]>='0' && str[i]<='9')
					||(str[i]>='a' && str[i]<='f')
					||(str[i]>='A' && str[i]<='F'))){
					return -1;
				}
			}
		}else{/* 数值溢出 */
			printf("error: number is too large\n");
			return -1;
		}
	}else{
		length = strlen(str);
		if(length <= 20){
			for(i=0;i<length;i++){/* 数值是否合法 */
				if(!(str[i]>='0' && str[i]<='9')){
					return -1;
				}
			}
			if(length == 20){
				if(str[0] != '1'){
					printf("error: number is too large\n");
					return -1;
				}
				unsigned long long a = 0;
				unsigned long long b = 0;
				unsigned long long c = 0;
				char a_tmp[21] = "";
				char b_tmp[21] = "";
				
				memset(a_tmp, '0', 20);
				a_tmp[0] = '1';
				memcpy(b_tmp, str+1, 19);
				a = decstr_to_dec(a_tmp);
				b = decstr_to_dec(b_tmp);
				c = a + b;
				if(c<a && c<b){/* 数值溢出 */
					printf("error: number is too large\n");
					return -1;
				}
			}
		}else{/* 数值溢出 */
			printf("error: number is too large\n");
			return -1;
		}
	}
	return 0;
}

unsigned long long hexstr_to_dec(char str[])
{
	unsigned long long n = 0;
	int i;
	
	for(i=0; str[i]!='\0'; i++){
		if(str[i]>='0' && str[i]<='9'){
			n = n*16 + str[i] - '0';
		}else if(str[i]>='a' && str[i]<='f'){
			n = n*16 + str[i] - 'a' + 10;
		}else if(str[i]>='A' && str[i]<='F'){
			n = n*16 + str[i]- 'A' + 10;
		}
	}
	return n;
}

unsigned long long decstr_to_dec(char str[])
{
	unsigned long long n = 0;
	int i;
	
	for(i=0; str[i]!='\0'; i++){
		if(str[i]>='0' && str[i]<='9'){
			n = n*10 + str[i] - '0';
		}
	}
	return n;
}

const char* dec_to_humanstr(unsigned long long size)
{
	char unit_chars[]= { 'B', 'K', 'M', 'G', 'T', 'P', 'E' };
	unsigned long long unit_result[7]= {};
	static char result_str[42] = ""; 
	char tmp[7] = "";
	int cycle_times;
	int i,j;

	if(size == 0)
		return "0";
	cycle_times = sizeof(unit_chars);
	memset(result_str, 0, sizeof(result_str));
	for(i=(cycle_times-1);i>=0;i--){
		unit_result[i] = size>>(i*10);
		size -= unit_result[i]<<(i*10);
	}
	for(i=(cycle_times-1);i>=0;i--){
		if(unit_result[i] != 0){
			for(j=i;j>=0;j--){
				if(unit_result[j] != 0){
					snprintf(tmp, sizeof(tmp), "%llu%c ", unit_result[j], unit_chars[j]);
				}else{
					snprintf(tmp, sizeof(tmp), "%llu%c ", unit_result[j], unit_chars[j]);
				}
				strcat(result_str, tmp);
			}
			break;
		}
	}
	return result_str;
}

/* 以人性化的方式显示出占用空间的大小 */
void show_human_number(char *str)
{
	unsigned long long num = 0;
	
	if(is_number_legal(str)){
		printf("legal number\n");
		exit(-1);
	}
	if(str[0]=='0' && str[1]=='x' || str[1]=='X'){/* 支持输入'0x'打头的十六进制数 */
		num = hexstr_to_dec(&str[2]);
		printf("Hex:%s\n", str);
	}else{
		num = decstr_to_dec(str);
		printf("Hex:%#llx\n", num);
	}
	printf("Decimal:%llu\n", num);
	printf("Human: %s\n", dec_to_humanstr(num));
}

int main(int argc, char *argv[])
{
	if( argc < 2){
		printf("usage: %s <number>\t"\
				"number must be Hex or Decimal\n", argv[0]);
		return -1;
	}
	show_human_number(argv[1]);
	return 0;
}
