#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //validate stego image
    //check the argv[2] is .bmp file or not
    char *cmp;
    if(cmp=strstr(argv[2],".bmp"))
    {
        if(strcmp(cmp,".bmp")==0)
        {
        //assign argv[2] to decInfo->stego_image_fname
        decInfo->stego_image_fname = argv[2];
        }
        else
        {
            return e_failure;
        }
        
    }
    else
    {
        //if it is not '.bmp' then print error message and return e_failure
        printf("ERROR : stego image extension must be .bmp file\n");
        return e_failure;
    }
    // validate output image
    // check argv[3] is present or not 
    if(argv[3]!=NULL)
    {
        //assign argv[3] without extension to decInfo->output_fname
        decInfo->output_fname = strtok(argv[3],".");
    }
    //default name
    else
    {
        //assign argv[3] to decInfo->output_fname
        printf("INFO: Output File not mentioned. Creating output as default without extension\n");
        decInfo->output_fname = "output";

    }
    return e_success;

    
}
/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ### Decoding Procedure Started ###\n");
    // check open_files_for_decode(decInfo) return value is e_success or not
    if(open_files_for_decode(decInfo)==e_success)
    {

        //goto step2
        //check skip_bmp_header(decInfo->fptr_strgo_image) return value is e_success or not
        if(skip_bmp_header(decInfo->fptr_stego_image)==e_success)
        {

            //goto step3
            // check decode_magic_string(decInfo->magic_string,decInfo) return value is e_success or not
            if(decode_magic_string(decInfo->magic_string,decInfo)==e_success)
            {
                //goto step4
                // check secret_file_extn_size(decInfo) return value is e_success or not
                if(decode_secret_file_extn_size(decInfo)==e_success)
                {
                    //goto step5
                    // check decode_secret_file_extn(decInfo->extn_output_file,decInfo) return value is e_success or not
                    if(decode_secret_file_extn(decInfo->extn_output_file,decInfo)==e_success)
                    {
                        //goto step6
                        //check decode_secret_file_size(decInfo) return value is e_success or not
                        if(decode_secret_file_size(decInfo)==e_success)
                        {
                            //goto step7
                            //check decode_secret_file_data(decInfo) and return value is e_success or not
                            if(decode_secret_file_data(decInfo)==e_success)
                            {
                                //return e_success
                                return e_success;
                            }
                            else
                            {
                                //print error message and return e_failure
                                printf("ERROR: Decode secret file data is failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            //print error message and return e_failure
                            printf("ERROR: Decode secret file size is failed\n");
                            return e_failure;

                        }
                    }
                    else
                    {
                        //print error message and return e_failure
                        printf("ERROR: Decode secret file extension is failed\n");
                        return e_failure;
                    }

                }
                else
                {
                    //print error message and return e_failure
                    printf("ERROR: Decode secret file extension size is failed\n");
                    return e_failure;
                }

            }
            else
            {
                //print error message and return e_failure
                printf("ERROR: Decode magic string is failed\n");
                return e_failure;
            }
        }
        else
        {
            //print error message and return e_failure
            printf("ERROR: Skip bmp header is failed\n");
            return e_failure;
        }
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
    }
    else
    {
        //print error message and return e_failure
        printf("ERROR: Unable to open files\n");
        return e_failure;
    }
    
    return e_success;
}

/* Get File pointers for i/p and o/p files */
Status open_files_for_decode(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    //open stego.bmp file
    printf("INFO: Opened steged_img.bmp\n");
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}
/* skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image)
{
    printf("INFO: Skip BMP Header\n");
    //seek to 54th byte
    fseek(fptr_stego_image, 54, SEEK_SET);
    printf("INFO: Done\n");
    return e_success;

}
/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    //declare check_magic_string
    char check_magic_string[10];
    printf("INFO: Decoding Magic String Signature\n");
    //check decode_image_to_data return value is e_success or not
    
    if(decode_image_to_data(magic_string,2,decInfo->fptr_stego_image)==e_success)
    {
        printf("enter magic string : \n");
        //read user input magic string 
        scanf("%s",check_magic_string);
        check_magic_string[2]='\0';
        //compare user entered magic string with decoded magic string
        if(strcmp(decInfo->magic_string, check_magic_string)==0)
        {
            //if true print success message and return e_success
            printf("INFO: entered magic string matches\n");
            printf("INFO: Done\n");
            return e_success;
        }
        else
        {
            // No success return e_failure
            printf("ERROR: Magic string not matches\n");
            return e_failure;
        }
        // No failure return e_success
        return e_success;
    }
    else
    {
        // No failure return e_success
        printf("ERROR: Decode image to data is failed\n");
        return e_failure;
    }
}
/* Decode function, which does the real encoding */
Status decode_image_to_data(char *data, int size, FILE *fptr_stego_image)
{
    //declare image_buffer
    char image_buffer[8];
    //loop for 8 times
    for(int i=0;i<size;i++)
    {
        //read 8 bytes from fptr_strgo_image
        fread(image_buffer,8,1,fptr_stego_image);
        //perform decode_lsb_to_byte
        data[i] = decode_lsb_to_byte(image_buffer);
    }
    data[size] = '\0';
    return e_success;
}
/* Decode a LSB into bytes of image data array */
char decode_lsb_to_byte(char *image_buffer)
{
    //initalize ch with zero
    char ch = 0;
    //declare i
    int i;
    //loop for 8 times
    for(i=0;i<8;i++)
    {
        ch = (ch | ((image_buffer[i] & 1) << (7-i)));
    }
    return ch;  //return ch value to decode_image _to_data
}
/* Decode a LSB into int of image data array */
int decode_lsb_to_int(char *image_buffer)
{
    //initalize num with zero
    int num = 0;
    //declare i
    int i;
    //loop for 32 times
    for(i=0;i<32;i++)
    {
        num = (num | ((image_buffer[i] & 1) << (31 - i)));
    }
    return num; //return num value
}
/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Extension size\n");
    //declare image_buffer
    char image_buffer[32];
    //read 32 bytes from stego_image
    fread(image_buffer,32,1,decInfo->fptr_stego_image);
    //perform decode_lsb_to_int
    decInfo->extn_size =  decode_lsb_to_int(image_buffer);
    printf("INFO: Done\n");
    return e_success;
}
/* Decode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    printf("INFO: Decoding Outout file(secret file) Extension\n");
    //call decode_image_to_data
    decode_image_to_data(file_extn,decInfo->extn_size,decInfo->fptr_stego_image);
    file_extn[decInfo->extn_size] = '\0';
    //declare file_name
    char file_name[100];
    //copy filename from decInfo->output_fname to file_name
    strcpy(file_name,decInfo->output_fname);
    //concatenate file_name and file_extn
    strcat(file_name,file_extn);
    //open file_name in write 'w' mode
    printf("INFO: Opened Output.txt\n");
    decInfo->fptr_output = fopen(file_name, "w");
    printf("INFO: Done. Opened all required files\n");
    return e_success;
}
/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding File Size\n");
    //declare image_buffer
    char image_buffer[32];
    //read 32 bytes from fptr_stego_image
    fread(image_buffer,32,1,decInfo->fptr_stego_image);
    //assign decoded size to decInfo->size_output_file
    decInfo->size_output_file = decode_lsb_to_int(image_buffer);
    printf("INFO: Done\n");
    return e_success;
}
/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding File Data\n");
    //declare ch
    char ch;
    //run loop for decInfo->size_output_file times
    for(int i = 0; i < decInfo->size_output_file; i++)
    {
        //declare image_buffer
        char image_buffer[8];
        //read 8 bytes from fptr_stego_image
        fread(image_buffer, 8, 1, decInfo->fptr_stego_image);
        //decode each char byte 
        ch = decode_lsb_to_byte(image_buffer);
        //store into output file which is decInfo->fptr_output
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    printf("INFO: Done\n");
    return e_success;

}