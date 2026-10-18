#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("INFO: width = %u ", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Get file size */
uint get_file_size(FILE *fptr)
{
    //logic to find secret file size
    printf("INFO: Checking for Secret.txt size\n");
    fseek(fptr,0,SEEK_END);
    long size = ftell(fptr); //assign to long size
    printf("INFO: Done. Not Empty\n");
    return (uint)size; //return size
    
}

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    printf("INFO: Opening SkeltonCode/beautiful.bmp files\n");
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    printf("INFO: Opened secret.txt\n");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    printf("INFO: Opened steged_img.bmp\n");
    printf("INFO: Done\n");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    printf("INFO: Checking for Skelton code/beautiful.bmp capacity to handle secret.txt\n");
    //call get image size for bmp and assign to encInfo->image_capacity
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    //call get file size for bmp and assign to encInfo->size_secret_file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    //validate image capacity must be greater than secret file size
    if(encInfo->image_capacity>(16+32+32+32+(encInfo->size_secret_file*8))) 
    {
        //print success message if true and return e_success
        //printf("capacity is checked successfully\n"); 
        printf("INFO: Done. Found OK\n");
        return e_success;
    }
    else
    {
        //print failure message and return e_failure
        printf("Error: Not enough capacity\n");  
        return e_failure;
    }
   
}


/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
   
    //validate source image
    //check the argv[2] is .bmp file or not
    char *cmp;
    if(cmp = strstr(argv[2],".bmp"))
    {
        if(strcmp(cmp,".bmp")==0)
        {
        //assign argv[2] to encInfo->src_image_fname
        encInfo->src_image_fname = argv[2];
        
        }
        else
        {
            return e_failure;
        }
        
    }
    else
    {
        //if it is not '.bmp' then print error message and return e_failure
        printf("ERROR: It is not '.bmp' file\n");
        return e_failure;
    }
    // validate secret file
    //check the argv[3] is . file or not
    if(strstr(argv[3],"."))
    {
        //if true assign argv[3] to encInfo->secret_fname
        encInfo->secret_fname = argv[3];
    }
    else
    {
        //if it is not '.' then print error message and return e_failure
        printf("ERROR: It is not '.' file\n" );
        return e_failure;
    }
    // validate stego image
    // check argv[4] is present or not 
    if(argv[4]!=NULL)
    {
        if(cmp = strstr(argv[4],".bmp"))
        {
            if(strcmp(cmp,".bmp")==0)
            {
            //if true assign argv[4] to encInfo->stego_image_fname
            encInfo->stego_image_fname = argv[4];
            }
        }
        else
        {
            //if it is not .bmp file then print error message and  return e_failure 
            printf("ERROR: It is not '.bmp' file \n");
            return e_failure;
        }
    }
    //default name
    else
    {
        //assign stego.bmp file to encInfo->stego_image_fname
        printf("INFO: Output File not mentioned. Creating stego.bmp as default\n");
        encInfo->stego_image_fname = "stego.bmp";

    }
    //return e_success
    return e_success;

    
}

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
 
    // check open_files(encInfo) return value is e_success or not
    printf("INFO: Openning required files\n");
    if(open_files(encInfo)==e_success) //open all files
    {
        //goto step2
        //check check_capacity(encInfo) return value is e_success or not
        printf("INFO: ## Encoding Procedure Started ##\n");
        
        if(check_capacity(encInfo)==e_success) 
        {
            //goto step3
            //check copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) return value is e_success or not
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)  
            {
                //goto step4
                //check encode_magic_string return value is e_success or not
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)  
                {
                    //goto step5
                    char *exten = strstr(encInfo->secret_fname,"."); //get extension 
                    char *extension = exten;
                    int extension_size = strlen(extension); //get string length of extension

                    //check encode_secret_file_extn_size return value is e_success or not
                    if(encode_secret_file_extn_size(extension_size,encInfo)==e_success)
                    {
                        //goto step6
                        //check encode_secret_file_extn return value is e_success or not
                        if(encode_secret_file_extn(extension,encInfo)==e_success)
                        {
                            //goto step7
                            //check encode_secret_file_size return value is e_success or not 
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                //goto step8
                                //check encode_secret_file_data return value is e_success or not
                                if(encode_secret_file_data(encInfo)==e_success) 
                                {
                                    //goto step9
                                    //copy_remaining_data return value is e_success or not
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success) 
                                    {
                                        //return e_success
                                        return e_success;
                                    }
                                    else
                                    {
                                        //print error message and return e_failure
                                        printf("ERROR: copy remaining data is failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    //print error message and return e_failure
                                    printf("ERROR: encode secret file data is failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                //print error message and return e_failure
                                printf("ERROR: encode secret file size is failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            //print error message and return e_failure
                            printf("ERROR: encode secret file extension is failed\n");
                            return e_failure;
                        }

                    }
                    else
                    {
                        //print error message and return e_failure
                        printf("ERROR: encode secret file extension size is failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    //print error message and return e_failure
                    printf("ERROR: encode magic string is failed\n");
                    return e_failure;
                }
            }
            else
            {
                //print error message and return e_failure
                printf("ERROR: copy bmp header is failed\n");
                return e_failure;
            }
        }
        else
        {
            //print error message and return e_failure
            printf("ERROR: check capacity is failed\n");
            return e_failure;
        }
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
    }
    else
    {
        //print error message and return e_failure
        printf("ERROR: Unable to open file\n");
        return e_failure;
    }
}
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("INFO: Copying Image Header\n");
    //rewind file pointer 
    rewind(fptr_src_image);
    rewind(fptr_dest_image);
    //declare bufferdata
    char bufferdata[54];
    //copy source header data to destination header data 
    fread(bufferdata,54,1,fptr_src_image);
    fwrite(bufferdata,54,1,fptr_dest_image);
    printf("INFO: Done\n");

    return e_success;
}
/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Magic String Signature\n");
    //encode magic string and store magic string
    //check encode_data_to_image return e_success or not
    if(encode_data_to_image(MAGIC_STRING, 2,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        //if true print success message and return e_success
        printf("INFO: Done\n");
        return e_success;
    }
    else
    {
        //if encode data to image is failure return e_failure
        return e_failure;
    }

}
/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //declare buffer
    char buffer[8];
    //loop for size times
    for(int i=0;i<size;i++)
    {
        //Read 8 bytes of data from source file 
        fread(buffer,8,1,fptr_src_image);
        //one by one char from data and encode byte to lsb
        //check encode_byte_to_lsb returns e_failure or not
        if(encode_byte_to_lsb(data[i],buffer)==e_failure)
        {
            //if encode_byte_to_lsb is failure then retun e_failure
            return e_failure;
        }
        // write 8 bytes of data buffer in to stego file
        fwrite(buffer,8,1,fptr_stego_image);
          
    }
    printf("INFO: Encode Byte To LSB is Successful\n");
    printf("INFO: Encode Date To Image is Successful\n");
    //return e_success
    return e_success;

}

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
   
    int mask = 1,i;
    //loop for 8 times
    for(i=0;i<8;i++)
    {
        // clear lsb bit of image buffer
        image_buffer[i] = image_buffer[i]&(~mask);
    }
    for(i=7;i>=0;i--)
    {
        //get 1 bit from data shift to lsb side
        //merge the image buffer and 1 bit data
        image_buffer[7-i] = (image_buffer[7-i]|(1&((unsigned)data>>i)));
    }
    return e_success;
    

}
/* Encode a int into LSB of image data array */
Status encode_int_to_lsb(int data, char *image_buffer)
{
    

    int mask = 1,i;
    //loop for 32 times
    for(i=0;i<32;i++)
    {
        // clear lsb bit of image buffer
        image_buffer[i] = image_buffer[i]&(~mask);  
    }
    for(i=31;i>=0;i--)
    {
        //get 1 bit from data shift to lsb side and merge the image buffer and 1 bit data
        image_buffer[31-i] = (image_buffer[31-i]|(1&(data>>i)));
    }
   
    //return e_success
    return e_success;

}

/* Encode secret file extenstion size*/
Status encode_secret_file_extn_size(int file_exten_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Extension Size\n");

    //declare image_buffer
    char image_buffer[32];
    //read 32 bytes from source file
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    //encode secret file extension size 

    //check encode_int_to_lsb return e_failure or not
    if(encode_int_to_lsb(file_exten_size,image_buffer)==e_failure) 
    {
        //if encode_int_to_lsb is failure then retun e_failure
        return e_failure;
    }
    //write encoded 32 bytes int to stego image
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image); 
    printf("INFO: Encode int To LSB is Successful\n");
    printf("INFO: Done\n");
    return e_success;

}
/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{

    //get extension 
    char *exten = strstr(encInfo->secret_fname,"."); 
    char *extension = exten;
    //get string length of extension 
    int file_exten_size = strlen(extension); 
    printf("INFO: Encoding secret.txt File Extension\n");
    //encode secret file extension
    encode_data_to_image((char*)file_extn,file_exten_size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    printf("INFO: Done\n");
    return e_success;


}
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Size\n");

    //declare image_buffer
    char image_buffer[32];
    //read 32 bytes from fptr_src_image
    fread(image_buffer,32,1,encInfo->fptr_src_image); 
    //check encode_int_to_lsb retun value is e_failure or not
    if(encode_int_to_lsb(file_size,image_buffer)==e_failure) 
    {
        //if failure then return e_failure
        return e_failure; 
    }
    //write encoded bytes to fptr_stego_image
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);
    printf("INFO: Encode int To LSB is Successful\n");
    printf("INFO: Done\n");
    return e_success;


}
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Data\n");
    //declare buffer
    char buffer[100];
    //start of secret file;
    rewind(encInfo->fptr_secret); 
    //read secret file data
    fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret); 
    //encode secret data
    encode_data_to_image(buffer, encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image); 
    printf("INFO: Done\n");
    return e_success;
}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("INFO: Copying Left Over Data\n");
    //declare buffer
    char buffer;
    //read byte to byte
    while(fread(&buffer,1,1,fptr_src)) 
    //write to destination
    fputc(buffer,fptr_dest);  
    printf("INFO: Done\n");
    return e_success;
    
}