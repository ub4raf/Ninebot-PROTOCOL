// GNU, blablabla
// ub4raf
// HARDWARE DEPENDET!
// used w Tiva(Stellaris) ARM core and ROM_-based HAL.


void send_controls (uint8_t throttle_des, uint8_t brake_des, uint8_t question)
    {
    NinebotPack send_pack;

                uint32_t length,counter;
                uint8_t data[32] = { 0x06, 0x2A , 0x29 , 0x00 , 0x00 , 0x07 , 0x01 };
                data[1]=throttle_des;
                data[2]=brake_des;
                if(question)
                    ninebot_create_pack(0x21,0x20,0x64,0,0x7,data,&send_pack);
                else
                    ninebot_create_pack(0x21,0x20,0x65,0,0x5,data,&send_pack);
                length=ninebot_serialyze(&send_pack,data);
                counter=0;
                while(counter<length)
                    {
                    ROM_UARTCharPut/*NonBlocking*/(UART5_BASE,data[counter]);
                    counter++;
                    }
    }
    
inline void string_left(uint8_t *data, uint32_t *index, uint8_t shift)
        {
            uint32_t result_buf;
            for(result_buf=0;
                    result_buf<*index-shift;
                    result_buf++)
              data[result_buf]=data[result_buf+shift];
            *index-=shift;
         }

uint32_t NB_UART_Process (void)
{
    uint32_t msg_counter=0;
    uint32_t result_buf;
    uint32_t counter;

#ifdef DEBUG_UART
                    result_buf=0;
                    UARTprintf("\nProcess buffer:");
                    while((result_buf)<dataUART_NB_index)
                          UARTprintf(" %0x",dataUART_NB_buffer[result_buf++]);
                    UARTprintf("\n");
#endif

    while(dataUART_NB_index>8)
        {
            result_buf=ninebot_parse(dataUART_NB_buffer,dataUART_NB_index, &pack);
                    switch(result_buf)
                        {
                           case 0:
                              string_left(dataUART_NB_buffer,&dataUART_NB_index,(pack.len+9));
#ifdef DEBUG_UART
                                counter=0;
                                while((counter)<pack.len)
                                      UARTprintf(" %0x",pack.data[counter++]);
                                UARTprintf("\n");
#endif
                                //5A A5 06 20 21 64 00 05 64 00 00 1B 00 D0 FE
                                //                                1B=speed
                            if(     (pack.direction==0x20)&&
                                    (pack.RW==0x21)&&
                                    //(pack.len==0x06)&&
                                    (pack.command==0x64)&&
                                    (pack.parameter==0x00)
                                    //&&(pack.data[0]==0x05)
                                    )
                                {
                                state= pack.data[0];    // charging, moove, stop etc
                                led_bat=pack.data[1]; // % of battery
                                lamp=pack.data[2];
                                beep=pack.data[3];
                                speed = pack.data[4];
                                new_2164=0xFF;
                                }
                            else
                                if(     (pack.direction==0x21)&&
                                        (pack.RW==0x20)&&
(  ((pack.command==0x65)&&(pack.len==0x05))  ||  ((pack.command==0x64)&&(pack.len==0x07)) )&&
                                        (pack.parameter==0x00)
                                        //&&(pack.data[0]==0x04)
                                        )
                                    {
                                    throttle = pack.data[1];
                                    brakes   = pack.data[2];
                                    new_2065=0xFF;
                                    }
                                else
                                    if(     (pack.direction==0x20)&&
                                            (pack.RW==0x21)&&
                                            (pack.command==0x01)//&&
                                            //(pack.len==0x00)&&
                                           //(pack.parameter==0x00)
                                     )
                                       {
                                        //  5A A5 00 20 21 01 00 BD FF ++++++++++
                                        //   esc to head, read register 00
                                        //  5A A5 0D 21 20 04 00 6A 6F 68 6E 73 63 6F 6F 74 65 72 00 00 FF FA
                                        //   head to esc, command 04!!! arg=00, "johnscooter", 00 00 hash
                                        //answer here
                                        UARTprintf(" johnscooter\n");
                                                    
                                                    NinebotPack send_pack;
                                                    uint8_t length;
    uint8_t data[32] = { 0x6A , 0x6F , 0x68 , 0x6E , 0x73 , 0x63 , 0x6F , 0x6F , 0x74 , 0x65 , 0x72 , 0x00 , 0x00 };
                                                    ninebot_create_pack(0x21,0x20,0x04,0x00,0x0D,data,&send_pack);
                                                    length=ninebot_serialyze(&send_pack,data);
                                                    counter=0;
                                                    while(counter<length)
                                                        {
                                                        ROM_UARTCharPut/*NonBlocking*/(UART5_BASE,data[counter]);
                                                        //TODO: NonBlocking tx
                                                        counter++;
                                                        }


                                       }
                                else
                                    //if(  //   (pack.direction==0x25)&&
                                         //   (pack.RW==0x20)//&&
                                      //      (pack.command==0x31)//&&
                                 //(  ((pack.command==0x65)&&(pack.len==0x05))  ||  ((pack.command==0x64)&&(pack.len==0x07)) )&&
                                 //           (pack.parameter==0x00)
                                            //&&(pack.data[0]==0x04)
                                     //)
                                       {
                                        //0x25 0x31
//#ifdef DEBUG_UART
                                UARTprintf("Unknown: %x from %x, %x:",pack.direction,pack.RW, pack.command);
                                result_buf=0;
                                while((result_buf)<pack.len)
                                      UARTprintf(" %0x",pack.data[result_buf++]);
                                UARTprintf("\n");
//#endif
                                       }
                                    //else
                                    {


                                    };
                            msg_counter++;
                            //return 0;
                            break;



                               //dataUART_NB_index--;
                           //move 1 or 2 times
                            case 1://not enough data?
                            case 4://Incomplete message
                            //return ;
                            case 5:
                            goto endofloop;


                            case 2:
                            case 3:


                            case 6:
                            default:
                                string_left(dataUART_NB_buffer,&dataUART_NB_index,1);
                        }

#ifdef DEBUG_UART
                    result_buf=0;
                    while((result_buf)<dataUART_NB_index)
                          UARTprintf(" %0x",dataUART_NB_buffer[result_buf++]);
                    UARTprintf("\n");
#endif

        }
    endofloop:

    return msg_counter;

}
