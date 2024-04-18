#include "sc_print.h"
#include "csr.h"

#define mstatush 0x310
#define mstatus 0x300
#define MSTATUS_MAE_OFFSET  25
#define configure_big_endian set_csr(mstatush, 1<<5)
#define configure_little_endian clear_csr(mstatush, 1<<5)
#define enable_AEBO set_csr(mstatus, 1<<MSTATUS_MAE_OFFSET)
#define disable_AEBO clear_csr(mstatus, 1<<MSTATUS_MAE_OFFSET)

#define untyped_address(x) ((void *)(&x))
#define skewed_address(x) (untyped_address(x)+sizeof(x)-1)
#define alien_address(x) ((typeof(x)*)skewed_address(x))
#define alien_format(x) (*alien_address(x))

int big_endian_selected;

int test_instruction(char* name,uint32_t result,uint32_t expected)
{
  sc_printf("testing instruccion %s. result:%#08x expected:%#08x.",name, result, expected);
  if(result==expected)
    {sc_printf("\n");return 0;}
  //else
  sc_printf("    FAILS\n");
  return 1;
}

void simulate_store(unsigned int size, uint32_t value, void* address)
{
  uint32_t address_mask=size-1;
  uint32_t swap_bits = (uint32_t)address & address_mask;
  int8_t* aligned_byte_table;
  if(big_endian_selected)
    swap_bits^=address_mask;
  aligned_byte_table= (uint8_t*) ((uint32_t)address & ~address_mask);
  for(int i=0;i<size;i++)
  {
    aligned_byte_table[i^swap_bits] = (uint8_t)value;
    value>>=8;
  }
}

uint32_t simulate_load(unsigned int size, int is_signed, void* address)
{
  uint32_t address_mask=size-1;
  uint32_t swap_bits = (uint32_t)address & address_mask;
  int8_t* aligned_byte_source_table=(uint8_t*) ((uint32_t)address & ~address_mask);
  uint32_t value_to_return=0;
  int8_t* byte_target_table=&value_to_return;
  if(big_endian_selected)
    swap_bits^=address_mask;
  if(is_signed && aligned_byte_source_table[(size-1)^swap_bits]<0)//is negative
    value_to_return-=1;//carry out sign extension
  for(int i=0;i<size;i++)
    byte_target_table[i]=aligned_byte_source_table[i^swap_bits];
  return value_to_return;
}

uint32_t LW(uint32_t* address)
{
  uint32_t value;
  volatile uint32_t* effective_address=address;
  if(big_endian_selected)
    configure_big_endian;
  value = *effective_address;
  configure_little_endian;
  return value;
}

uint32_t LHU(void* address)
{
  uint16_t value;
  volatile uint16_t* effective_address=address;
  if(big_endian_selected)
    configure_big_endian;
  value=*effective_address;
  configure_little_endian;
  return value;
}

uint32_t LH(void* address)
{
  int16_t value;
  volatile int16_t* effective_address=address;
  if(big_endian_selected)
    configure_big_endian;
  value=*effective_address;
  configure_little_endian;
  return value;
}

uint32_t LBU(void* address)
{
  uint8_t value;
  volatile uint8_t* effective_address=address;
  if(big_endian_selected)
    configure_big_endian;
  value=*effective_address;
  configure_little_endian;
  return value;
}

uint32_t LB(void* address)
{
  int8_t value;
  volatile int8_t* effective_address=address;
  if(big_endian_selected)
    configure_big_endian;
  value=*effective_address;
  configure_little_endian;
  return value;
}

void SW(uint32_t value, uint32_t* address)
{
  if(big_endian_selected)
    configure_big_endian;
  *address=value;
  configure_little_endian;
}

void SH(uint32_t value, uint16_t* address)
{
  if(big_endian_selected)
    configure_big_endian;
  *address=value;
  configure_little_endian;
}

void SB(uint32_t value, uint8_t* address)
{
  if(big_endian_selected)
    configure_big_endian;
  *address=value;
  configure_little_endian;
}

int main()
{
  int fails=0;
  int size,logsize,offset,is_signed;
  uint32_t stored=0x5678abcd;
  uint32_t result,expected;
  void* address;
  char* instruction_name;

  enable_AEBO;

  for(big_endian_selected=0;big_endian_selected<2;big_endian_selected++)
    {
    sc_printf("\nTesting %s endian data access\n\n",big_endian_selected?"big":"little");
    for(logsize=0;logsize<3;logsize++)
    {
      size=1<<logsize;
      address=&stored;
      for(offset=0;offset<4;offset++)
      {
        for(is_signed=0;is_signed<2 && !(logsize==2 && is_signed);is_signed++)
        {
          expected=simulate_load(size,is_signed,address);
          switch (logsize)
          {
            case 0:
              result= is_signed? LB(address) : LBU(address);
              instruction_name= is_signed? " LB" : "LBU";
              break;
            case 1:
              result= is_signed? LH(address) : LHU(address);
              instruction_name= is_signed? " LH" : "LHU";
              break;
            case 2:
              result= LW(address);
              instruction_name= " LW";
              break;
          }
          fails|=test_instruction(instruction_name,result,expected);
        }
        address++;
      }
    }

    for(logsize=0;logsize<3;logsize++)
    {
      size=1<<logsize;
      for(offset=0;offset<4;offset++)
      {
        result=0x12345678;
        expected=0x12345678;
        simulate_store(size, 0x5678abcd,(void*)&expected+offset);
        switch (logsize) {
          case 0:
            SB(0x5678abcd,(void*)&result+offset);
            instruction_name="SB";
            break;
          case 1:
            SH(0x5678abcd,(void*)&result+offset);
            instruction_name="SH";
            break;
          case 2:
            SW(0x5678abcd,(void*)&result+offset);
            instruction_name="SW";
            break;
        }
        fails|=test_instruction(instruction_name,result,expected);
      }
    }
  }
  return fails;
}
