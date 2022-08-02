/* This file contains codes related to editing */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"
#include "types.h"

/* Function Definitions */

/* Validating the files given through CLA */
Status read_and_validate_edit_args(char *argv[], EditInfo *edInfo)
{
    /* Validate options giving with edit option */
    if(strcmp(argv[2],"-t") == 0 || strcmp(argv[2],"-a") == 0 || strcmp(argv[2],"-A") == 0 || strcmp(argv[2],"-y") == 0 || strcmp(argv[2],"-m") == 0 || strcmp(argv[2],"-c") == 0)
    {
        strcpy(edInfo -> option,argv[2]);
    }
    else
    {
        return e_failure;
    }

    /* Validate if .mp3 file is provided */
    if(strcmp((strstr(argv[4],".")), ".mp3") == 0)
    {
        edInfo -> edit_mp3_fname = argv[4];
    }
    else
    {
        printf(".mp3 file not passed\n");
        return e_failure;
    }

    return e_success;
}

/* Function definition to open files in relevant modes */
Status open_edit_files(EditInfo *edInfo)
{
    /* Open the the file in read and write mode and define a file pointer */
    edInfo->fptr_edit_mp3 = fopen(edInfo->edit_mp3_fname, "r+");

    /* Error message if passed .mp3 file does not exist */
    if (edInfo->fptr_edit_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", edInfo->edit_mp3_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* Function to check tags */
Status data_edit_mp3(const char *data, int size, const char *original)
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

/* Function to check for ID3 tag */
Status check_edit_ID3(FILE *src, EditInfo *edInfo)
{
    char ID[3], ch;

    /* Read the ID3 characters into a buffer */
    fread(ID,3,sizeof(char),src);

    /* Pass the buffer to the function to compare */
    data_edit_mp3(ID,3,"ID3");
    return e_success;
}

Status check_edit_version(FILE *src)
{
    short version;

    /* Function to check for Version */
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
Status check_edit_TIT2(EditInfo *edInfo)
{
    /* Skip the next 5 bytes to reach the first tag TIT2 */
    fseek(edInfo -> fptr_edit_mp3, 5, SEEK_CUR);
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);

    /* Check if characters are matching the tag TIT2 */
    if(data_edit_mp3(tag,4,"TIT2") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TPE1 tag */
Status check_edit_TPE1(EditInfo *edInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);
    /* Check if characters are matching the tag TPE1 */
    if(data_edit_mp3(tag,4,"TPE1") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TALB tag */
Status check_edit_TALB(EditInfo *edInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);
    
    /* Check if characters are matching the tag TALB */
    if(data_edit_mp3(tag,4,"TALB") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TYER tag */
Status check_edit_TYER(EditInfo *edInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);
    /* Check if characters are matching the tag TYER */
    if(data_edit_mp3(tag,4,"TYER") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for TCON tag */
Status check_edit_TCON(EditInfo *edInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);
    /* Check if characters are matching the tag TYER */
    if(data_edit_mp3(tag,4,"TCON") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to check for COMM tag */
Status check_edit_COMM(EditInfo *edInfo)
{
    char tag[4];

    /* Read tag characters into the buffer */
    fread(tag,4,sizeof(char),edInfo -> fptr_edit_mp3);
    /* Check if characters are matching the tag COMM */
    if(data_edit_mp3(tag,4,"COMM") == e_success)
    {
        return e_success;    
    }
    return e_failure;
}

/* Function to edit title */
Status get_edit_title(EditInfo *edInfo,char *argv[])
{
    uint title_size = 0;

    /* Read the size of data into a variable */
    fread(&title_size, sizeof(int), 1, edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    title_size >>=  24;

    /* if option entered is -t, then edit title */
    if(strcmp(edInfo -> option, "-t") == 0)
    {
        /* Allocate memory for struct pointer variable */
        edInfo -> Title_edit = calloc(title_size, 1);

        /* First element should be a NULL */
        edInfo -> Title_edit[0] = '\0';

        /* Copy the data from 2nd index*/
        strcpy(&edInfo -> Title_edit[1], argv[3]);

        /* Write the new title in the title area in header */
	    fwrite(edInfo -> Title_edit, title_size, 1, edInfo -> fptr_edit_mp3);

        /* Print statements */
        printf("-------------------CHANGE THE TITLE-------------------\n");
        printf("TITLE: %s\n",&edInfo -> Title_edit[1]);
        printf("--------------TITLE CHANGED SUCCESSFULLY--------------\n\n");
    }
        
    /* if option not correct then skip the data */
    else
        fseek(edInfo -> fptr_edit_mp3, title_size, SEEK_CUR);
    
    return e_success;
}

/* Function to edit artist */
Status get_edit_artist(EditInfo *edInfo,char *argv[])
{
    uint artist_size = 0;

    /* Read the size of data into a variable */
    fread(&artist_size,sizeof(int),1,edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    artist_size >>=  24;

    /* if option entered is -a, then edit artist */
    if(strcmp(edInfo -> option, "-a") == 0)
    {
        edInfo -> Artist_edit = calloc(artist_size, 1);
        edInfo -> Artist_edit[0] = '\0';
        strcpy(&edInfo -> Artist_edit[1], argv[3]);
    
	    fwrite(edInfo -> Artist_edit,artist_size, 1, edInfo -> fptr_edit_mp3);
        printf("-------------------CHANGE THE ARTIST-------------------\n");
        printf("ARTIST: %s\n",&edInfo -> Artist_edit[1]);
        printf("--------------ARTIST CHANGED SUCCESSFULLY--------------\n\n");
    }
    else
        fseek(edInfo -> fptr_edit_mp3, artist_size, SEEK_CUR);

    return e_success;
}

/* Function to edit album */
Status get_edit_album(EditInfo *edInfo,char *argv[])
{
    uint album_size = 0;

    /* Read the size of data into a variable */
    fread(&album_size,sizeof(int), 1, edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    album_size >>=  24;

    /* if option entered is -A, then edit album */
    if(strcmp(edInfo -> option, "-A") == 0)
    {
        edInfo -> Album_edit = calloc(album_size, 1);
        edInfo -> Album_edit[0] = '\0';
        strcpy(&edInfo -> Album_edit[1], argv[3]);
    
	    fwrite(edInfo -> Album_edit,album_size, 1, edInfo -> fptr_edit_mp3);
        printf("-------------------CHANGE THE ALBUM-------------------\n");
        printf("ALBUM: %s\n",&edInfo -> Album_edit[1]);
        printf("--------------ALBUM CHANGED SUCCESSFULLY--------------\n\n");
    }
    else
        fseek(edInfo -> fptr_edit_mp3, album_size, SEEK_CUR);
    
    return e_success;
}

/* Function to edit year */
Status get_edit_year(EditInfo *edInfo,char *argv[])
{
    uint year_size = 0;

    /* Read the size of data into a variable */
    fread(&year_size,sizeof(int), 1, edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    year_size >>=  24;

    /* if option entered is -y, then edit year */
    if(strcmp(edInfo -> option, "-y") == 0)
    {
        edInfo -> Year_edit = calloc(year_size, 1);
        edInfo -> Year_edit[0] = '\0';
        strcpy(&edInfo -> Year_edit[1], argv[3]);
    
	    fwrite(edInfo -> Year_edit,year_size, 1, edInfo -> fptr_edit_mp3);
        printf("-------------------CHANGE THE YEAR-------------------\n");
        printf("YEAR: %s\n",&edInfo -> Year_edit[1]);
        printf("--------------YEAR CHANGED SUCCESSFULLY--------------\n\n");
    }
    else
        fseek(edInfo -> fptr_edit_mp3, year_size, SEEK_CUR);
    
    return e_success;
}

/* Function to edit content */
Status get_edit_content(EditInfo *edInfo,char *argv[])
{
    uint content_size = 0;

    /* Read the size of data into a variable */
    fread(&content_size,sizeof(int), 1, edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    content_size >>=  24;

    /* if option entered is -m, then edit content */
    if(strcmp(edInfo -> option, "-m") == 0)
    {
        edInfo -> Content_edit = calloc(content_size, 1);
        edInfo -> Content_edit[0] = '\0';
        strcpy(&edInfo -> Content_edit[1], argv[3]);
    
	    fwrite(edInfo -> Content_edit, content_size, 1, edInfo -> fptr_edit_mp3);
        printf("-------------------CHANGE THE CONTENT-------------------\n");
        printf("CONTENT: %s\n",&edInfo -> Content_edit[1]);
        printf("--------------CONTENT CHANGED SUCCESSFULLY--------------\n\n");
    }
    else
        fseek(edInfo -> fptr_edit_mp3, content_size, SEEK_CUR);
    return e_success;
}

/* Function to edit composer */
Status get_edit_composer(EditInfo *edInfo,char *argv[])
{
    uint composer_size = 0;

    /* Read the size of data into a variable */
    fread(&composer_size,sizeof(int), 1, edInfo -> fptr_edit_mp3);

    /* Skip the 2 byte flag */
    fseek(edInfo -> fptr_edit_mp3, 2, SEEK_CUR);

    /*Store as little endian, so shifting*/
    composer_size >>=  24;

    /* if option entered is -c, then edit composer */
    if(strcmp(edInfo -> option, "-c") == 0)
    {
        edInfo -> Composer_edit = calloc(composer_size, 1);
        edInfo -> Composer_edit[0] = '\0';
        strcpy(&edInfo -> Composer_edit[1], argv[3]);
    
	    fwrite(edInfo -> Composer_edit, composer_size, 1, edInfo -> fptr_edit_mp3);
        printf("-------------------CHANGE THE COMPOSER-------------------\n");
        printf("COMPOSER: %s\n",&edInfo -> Composer_edit[1]);
        printf("--------------COMPOSER CHANGED SUCCESSFULLY--------------\n\n");
    }
    else
        fseek(edInfo -> fptr_edit_mp3, composer_size, SEEK_CUR);

    return e_success;
}

/* Function definition for editing */
/* All function's working comments mentioned above */
Status do_edit(EditInfo *edInfo,char *argv[])
{
    if(open_edit_files(edInfo) == e_success)
    {
        if(check_edit_ID3(edInfo->fptr_edit_mp3, edInfo) == e_success)
        {
            if(check_edit_version(edInfo->fptr_edit_mp3) == e_success)
            {
                if(check_edit_TIT2(edInfo) == e_success)
                {
                    if(get_edit_title(edInfo,argv) == e_success)
                    {
                        if(check_edit_TPE1(edInfo) == e_success)
                        {
                            if(get_edit_artist(edInfo,argv) == e_success)
                            {
                                if(check_edit_TALB(edInfo) == e_success)
                                {
                                    if(get_edit_album(edInfo,argv) == e_success)
                                    {
                                        if(check_edit_TYER(edInfo) == e_success)
                                        {
                                            if(get_edit_year(edInfo,argv) == e_success)
                                            {
                                                if(check_edit_TCON(edInfo) == e_success)
                                                {
                                                    if(get_edit_content(edInfo,argv) == e_success)
                                                    {
                                                        if(check_edit_COMM(edInfo) == e_success)
                                                        {
                                                            if(get_edit_composer(edInfo,argv) == e_success);
                                                        }
                                                        else
                                                        {
                                                            printf("COMPOSER NOT CHANGED!!!");
                                                            return e_failure;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    printf("CONTENT NOT CHANGED!!!");
                                                    return e_failure;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            printf("YEAR NOT CHANGED!!!");
                                            return e_failure;
                                        }
                                    }
                                }
                                else
                                {
                                    printf("ALBUM NOT CHANGED!!!");
                                    return e_failure;
                                }
                            }
                        }
                        else
                        {
                            printf("ARTIST NOT CHANGED!!!\n");
                            return e_failure;
                        }
                    }
                }
                else
                {
                    printf("TITLE NOT CHANGED!!!\n");
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