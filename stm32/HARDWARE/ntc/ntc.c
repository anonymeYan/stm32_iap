#include "ntc.h"





u16 get_ntctemperature(u16 ntcx)
{
	u16 i;
	float temp;
	if(ntcx>9533)
temp=0;
else 
	if(ntcx<66)
temp=SIZE*5;
  else //if((ntcx<9534)&&(ntcx>244))
for(i=1;i<=SIZE;i++)
if(ntcx>ntc[i])
{temp=5*(i-1);
temp+=(float)(ntc[i-1]-ntcx)/(ntc[i-1]-ntc[i])*5;
i=SIZE+1;
}
return temp;
}


u16 GetTempreture(u16 ntcy)
{
	u16 i;
	float temp;
	if(ntcy>=17212)
temp=0;
else 
	if(ntcy<=67)
temp=130*10;
  else //if((ntcx<9534)&&(ntcx>244))
for(i=1;i<=130;i++)
if(ntcy>ntc1[i])
{temp=10*(i-1);
temp+=(float)(ntc1[i-1]-ntcy)/(ntc1[i-1]-ntc1[i])*10;
i=131;
}
return temp;




}









