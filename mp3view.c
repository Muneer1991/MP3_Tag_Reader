/* This file contains codes related to viewing */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3view.h"
#include "types.h"

/* Function Definitions */

/* Validating the files given through CLA */
Status read_and_validate_view_args(char *argv[], ViewInfo *viInfo)
{
    /* Check for .mp3 file and  */
    if(strcmp((strstr(argv[2],".")), ".mp3") == 0)
    {
        /* Store file name in a struct variable */
        viInfo -> view_mp3_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    return e_success;
}

/* Function definition to open files in relevant modes */
Status open_view_files(ViewInfo *viInfo)
{
    /* Open the file in read only mode a assign a file pointer */
    viInfo->fptr_view_mp3 = fopen(viInfo->view_mp3_fname, "r");

    /*If is non-existing print error */
    if (viInfo->fptr_view_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", viInfo->view_mp3_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* Function to check tags */
Status data_view_mp3(const char *data, int size, const char *original)
{
    int count = 0;
    
    /* If tag is matching the buffer then count will be same as size of tag */
    for(int i = 0; i < size; i++)
    {
        if(data[i] == original[i])
        {
            count++;
        }
    }

    if(count != size)
    {
        return e_failure;
    }
    
	/* No failure return e_success */
    return e_success;
}

/* Function to retrieve data from header */
Status get_view_data(int size, ViewInfo *viInfo, char *tag)
{
    /* Buffer to store the data retrieved */
	char buffer[size];
    int i;
    /* Skip flags to point to the data */
	fseek(viInfo -> fptr_view_mp3, 2, SEEK_CUR);

    /* Read the data into the buffer */
	fread(buffer,size, 1, viInfo -> fptr_view_mp3);

    /* Transfer the data character by character */
	for(i = 0; i < size; i++)
    {
        tag[i] = buffer[i];
    }
    /* Store null at end to form it as a string */
    tag[i] = '\0';
    
    return e_success;
}
/* Function to check for ID3 tag */
Status check_view_ID3(FILE *src, ViewInfo *viInfo)
{
    char ID[3];
    
    /* Read the ID3 characters into a buffer */
    fread(ID,3,sizeof(char),src);
    
    /* Pass the buffer to the function to compare */
    if(data_view_mp3(ID,3,"ID3") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for Version */
Status check_view_version(FILE *src)
{
    short version;
    /* Read the 2 bytes of version detail into the variable */
    fread(&version,sizeof(short),1,src);

    /* Since little endian, the variable needs to be shifted to get 03 00 */
	version <<= 8;

    /* If version matching, then success */
	if(version == 0x0300)
         return e_success;
    else
        return e_failure;
}

/* Function to check for TIT2 tag */
Status check_view_TIT2(ViewInfo *viInfo)
{
    /* Skip the next 5 bytes to reach the first tag TIT2 */
    fseek(viInfo -> fptr_view_mp3, 5, SEEK_CUR);
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag TIT2 */
    if(data_view_mp3(tag,4,"TIT2") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TPE1 tag */
Status check_view_TPE1(ViewInfo *viInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag TPE1 */
    if(data_view_mp3(tag,4,"TPE1") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TALB tag */
Status check_view_TALB(ViewInfo *viInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag TALB */
    if(data_view_mp3(tag,4,"TALB") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TYER tag */
Status check_view_TYER(ViewInfo *viInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag TYER */
    if(data_view_mp3(tag,4,"TYER") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TCON tag */
Status check_view_TCON(ViewInfo *viInfo)
{
    char tag[4];
    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag TYER */
    if(data_view_mp3(tag,4,"TCON") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for COMM tag */
Status check_view_COMM(ViewInfo *viInfo)
{
    char tag[4];
    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),viInfo -> fptr_view_mp3);

    /* Check if characters are matching the tag COMM */
    if(data_view_mp3(tag,4,"COMM") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to store the data into a struct variable */
Status get_view_title(ViewInfo *viInfo)
{
    uint title_size = 0;

    /* Read the size of data into a variable */
    fread(&title_size, sizeof(int), 1, viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    title_size >>=  24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Title_view = calloc(title_size, 1);

    /* Store the data into the struct variable through this function call */
    get_view_data(title_size, viInfo, viInfo -> Title_view);

    return e_success;
}

/* Function to store the data into a struct variable */
Status get_view_artist(ViewInfo *viInfo)
{
    uint artist_size = 0;

    /* Read the size of data into a variable */
    fread(&artist_size,sizeof(int),1,viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    artist_size >>= 24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Artist_view = calloc(artist_size,1);

    /* Store the data into the struct variable through this function call */
    get_view_data(artist_size, viInfo, viInfo -> Artist_view);

    return e_success;
}

/* Function to store the data into a struct variable */
Status get_view_album(ViewInfo *viInfo)
{
    uint album_size = 0;

    /* Read the size of data into a variable */
    fread(&album_size,sizeof(int), 1, viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    album_size >>= 24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Album_view = calloc(album_size,1);

    /* Store the data into the struct variable through this function call */
    get_view_data(album_size, viInfo, viInfo -> Album_view);
    
    return e_success;
}

/* Function to store the data into a struct variable */
Status get_view_year(ViewInfo *viInfo)
{
    uint year_size = 0;

    /* Read the size of data into a variable */
    fread(&year_size,sizeof(int), 1, viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    year_size >>= 24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Year_view = calloc(year_size + 1, 1);

    /* Store the data into the struct variable through this function call */
    get_view_data(year_size, viInfo, viInfo -> Year_view);
    
    return e_success;
}

/* Function to store the data into a struct variable */
Status get_view_content(ViewInfo *viInfo)
{
    uint content_size = 0;

    /* Read the size of data into a variable */
    fread(&content_size,sizeof(int), 1, viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    content_size >>= 24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Content_view = calloc(content_size,1);

    /* Store the data into the struct variable through this function call */
    get_view_data(content_size, viInfo, viInfo -> Content_view);
    
    return e_success;
}

/* Function to store the data into a struct variable */
Status get_view_composer(ViewInfo *viInfo)
{
    uint composer_size = 0;

    /* Read the size of data into a variable */
    fread(&composer_size,sizeof(int), 1, viInfo -> fptr_view_mp3);

    /* Little endian reverse the size data, so shift operation */
    composer_size >>= 24;

    /* Allocate memory to struct variable according to the data size */
    viInfo -> Composer_view = calloc(composer_size,1);

    /* Store the data into the struct variable through this function call */
    get_view_data(composer_size, viInfo, viInfo -> Composer_view);

    return e_success;
}
/* Function definition for viewing */
/* All function's working comments mentioned above */
Status do_view(ViewInfo *viInfo)
{
    if(open_view_files(viInfo) == e_success)
    {
        if(check_view_ID3(viInfo->fptr_view_mp3, viInfo) == e_success)
        {
            if(check_view_version(viInfo->fptr_view_mp3) == e_success)
            {
                if(check_view_TIT2(viInfo) == e_success)
                {
                    if(get_view_title(viInfo) == e_success)
                    {
                        printf("%-10s%-4c%s\n","Title",':',&viInfo -> Title_view[1]);
                        if(check_view_TPE1(viInfo) == e_success)
                        {
                            if(get_view_artist(viInfo) == e_success)
                            {
                                printf("%-10s%-4c%s\n","Artist",':',&viInfo -> Artist_view[1]);
                                if(check_view_TALB(viInfo) == e_success)
                                {
                                    if(get_view_album(viInfo) == e_success)
                                    {
                                        printf("%-10s%-4c%s\n","Album",':',&viInfo -> Album_view[1]);
                                        if(check_view_TYER(viInfo) == e_success)
                                        {
                                            if(get_view_year(viInfo) == e_success)
                                            {
                                                printf("%-10s%-4c%s\n","Year",':',&viInfo -> Year_view[1]);
                                                if(check_view_TCON(viInfo) == e_success)
                                                {
                                                    if(get_view_content(viInfo) == e_success)
                                                    {
                                                        printf("%-10s%-4c%s\n","Content",':',&viInfo -> Content_view[1]);
                                                        if(check_view_COMM(viInfo) == e_success)
                                                        {
                                                            if(get_view_composer(viInfo) == e_success)
                                                            {
                                                                printf("%-10s%-4c%s\n","Composer",':',&viInfo -> Composer_view[1]);
                                                            }
                                                            else
                                                            {
                                                                printf("Composer not retrieved!!!\n");
                                                                return e_failure;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            printf("Tag is not COMM!!!\n");
                                                            return e_failure;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        printf("Content not retrieved!!!\n");
                                                        return e_failure;
                                                    }
                                                }
                                                else
                                                {
                                                    printf("Tag is not TYER!!!\n");
                                                    return e_failure;
                                                }
                                            }
                                            else
                                            {
                                                printf("Year not retrieved!!!\n");
                                                return e_failure;
                                            }
                                        }
                                        else
                                        {
                                            printf("Tag is not TYER!!!\n");
                                            return e_failure;
                                        }
                                    }
                                    else
                                    {
                                        printf("Album not retrieved!!!\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Tag is not TALB!!!\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Artist not retrieved!!!\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Tag is not TPE1!!!\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Title not retrieved!!!\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Tag is not TIT2!!!\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Version is not 03 00\n");
                return e_failure;
            }
        }
        else
        {
            printf("Tag is not ID3!!!\n");
            return e_failure;
        }
    }
    else
    {
        printf("File open failed!!!\n");
        return e_failure;
    }
    return e_success;
}