#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(argc<=2)
    {
        printf("ERROR: Enter correct arguments\n");
        return 0;
    }

    //check operation type
    int ret = check_operation_type(argv);
    

    if(ret == e_encode)
    {
        if(argc>=4)
        {
            //check read_and_validate_encode_args(argv, &encInfo) retun value is e_success or not
            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                printf("INFO: Read And Validated Successfully\n");
                if(do_encoding(&encInfo)==e_success)
                {
                    //encoding completed successfully
                    printf("### Encoding completed successfully ###\n");
                }
                else
                {
                    //error message and terminate the program
                    printf("ERROR: Encoding failed\n");
                    return 1;
                }
            }
            else
            {
                printf("ERROR: error in read and validate encoding\n");
            }
        }
        else
        {
            //error message and terminate the program
            printf("ERROR: enter correct arguments\n");
            return 1;
            
        }
        //encoding
    }
    else if(ret == e_decode)
    {
        //decoding
        if(argc>=3)
        {
            //check read_and_validate_decode_args(argv, &decInfo) retun value is e_success or not
            if(read_and_validate_decode_args(argv,&decInfo)==e_success)
            {
                if(do_decoding(&decInfo)==e_success)
                {
                    //print success message 
                    printf("### Decoding completed successfully ###\n");
                }
                else
                {
                    //print error message and terminate the program
                    printf("ERROR: Decoding failed\n");
                    return 1;
                    
                }
            }
            else
            {
                //print error message and terminate the program
                printf("ERROR: read and validate decode args failed\n");
                return 1;
                
            }
        }
        else
        {
            //print error message
            printf("ERROR: Invalid input, enter correct arguments");
        }
    }
    else if(ret == e_unsupported)
    {
        printf("ERROR: Unsupported operation. Use -e for encode or -d for decode.\n");
        return 1;
    }
}
OperationType check_operation_type(char *argv[])
{
    //check the argv[1] is -e or not
        //return e_encode
    
   
    if(!strcmp(argv[1],"-e"))
    {
        printf("INFO: Operation type is encode\n");
        return e_encode;
    }
    //check the argv[1] is -d or not
        //return e_decode
    else if(!strcmp(argv[1],"-d"))
    {
        printf("INFO: Operation type is decoding\n");
        return e_decode;
    }
    //else
        // return e_unsupported
    else
    {
        printf("INFO: Unsupported Operation\n");
        return e_unsupported;
    }

}




