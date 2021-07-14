#include "common_util.h"

uint32_t __asciihex2uint(uint8_t nibLChar, uint8_t nibHChar)
{
   uint32_t retVal = 0;
   
   if (nibHChar >= '0' && nibHChar <= '9')
       nibHChar = nibHChar - '0';
   else
   {
       if (nibHChar >= 'a' && nibHChar <= 'f')
           nibHChar = nibHChar - 'a' + 10;
       else
           return 0x100;
   }

   if (nibLChar >= '0' && nibLChar <= '9')
       nibLChar = nibLChar - '0';
   else
   {
       if (nibLChar >= 'a' && nibLChar <= 'f')
           nibLChar = nibLChar - 'a' + 10;
       else
           return 0x100;
   }

   retVal = nibHChar*16 + nibLChar;

   return retVal;
}


uint32_t hexToInt(int *hex) {
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

unsigned int intToHex(int n)
{
    char hexaDeciNum[12];
    int i = 0;
    while (n != 0) {
        int temp = 0;
        temp = n % 16;
        // check if temp < 10
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
        n = n / 16;
    }
    
    int cnt = 0;
    unsigned char dataIntToHex[9];
    memset(dataIntToHex, NULL, 9);
    for (int j = i - 1; j >= 0; j--)
    {
        printf("%c", hexaDeciNum[j]);
        dataIntToHex[cnt] = hexaDeciNum[j];
        cnt++;
    }
    cnt = 0;
    if(verbose == 1)
    {
        Serial.printf("Data: %s \n", dataIntToHex);
    }
    unsigned int dataInt = atoi((const char*)dataIntToHex);
    if(verbose == 1)
    {
        Serial.printf("dataInt: %d \n", dataInt);
    }
    memset(dataIntToHex, NULL, 9);
    return dataInt;
}
