#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define NO_PACK 10
int rndas(int a){
    int rn = (random() %10) %a;
    return rn == 0?1:rn;
}
int main()
{
    /* code */
    int pack_sz[NO_PACK];
    int  p_sz_rm, i, clk, op, b_size, o_rate, p_time;
    for(i=0;i<NO_PACK;i++)
        pack_sz[i]=rndas(6)*10;
    for(i=0;i<NO_PACK;i++)
        printf("packet[%d]:%d bytes\n", i, pack_sz[i]) ;
    printf("enter output rate\n");
    scanf("%d",&o_rate);
    printf("enter bucket size\n");
    scanf("%d", &b_size);

    for (i = 0; i < NO_PACK; i++)
    {
      if((pack_sz[i]+p_sz_rm)>b_size)
        if(pack_sz[i]>b_size)
            printf("incoming packet %d bytes is greater than bucket size %d bytes --REJECTED\n",pack_sz[i],b_size);
        else
            printf("Bucket capacity exceded -PACKETS REJECTED\n");
      else{

          p_sz_rm +=pack_sz[i];
          printf("incoming packet size:%d\n",pack_sz[i]);
          printf("bytes remaning:%d\n",p_sz_rm);
          p_time = rndas(4)*10;
          printf("time ramining is:%d units\n",p_time);
          for(clk=10;clk<=p_time;clk+=10)
          {
              sleep(1);
              if(p_sz_rm)
              {
                  if(p_sz_rm<=o_rate)
                      op = p_sz_rm, p_sz_rm = 0;
                   else
                      op = o_rate,p_sz_rm -= o_rate;
                      
                 printf("packet size %d transmitted\n", op);
                 printf("bytes remaining %d\n", p_sz_rm);
              }
              else{
                  printf("time left for transmitting %d units\n ", p_time-clk);
                  printf("No packets left to transmit\n");
              }
              

          }
      }
    }

    return 0;
}


