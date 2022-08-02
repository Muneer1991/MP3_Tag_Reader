/* This file contains the function prototypes and struct necessary for viewing */

/* This file contains all required prototypes for viewing */
#include <stdio.h>
#ifndef VIEW_H
#define VIEW_H

#include "types.h" // Contains user defined types

/* Structure to store information required for viewing mp3 file */
typedef struct _ViewInfo
{
    char *view_mp3_fname;
    FILE *fptr_view_mp3;
    char *Title_view;
    char *Artist_view;
    char *Album_view;
    char *Year_view;
    char *Content_view;
    char *Composer_view;
} ViewInfo;


/* Viewing function prototype */

/* Check operation type */
OperationType check_operation_type(int argc, char *argv[]);

/* Read and validate arguments for viewing */
Status read_and_validate_view_args(char *argv[], ViewInfo *viInfo);

/* Function to view details */
Status do_view(ViewInfo *viInfo);

/* Function to open files in read mode */
Status open_view_files(ViewInfo *viInfo);

/* Function to check for ID3 tag */
Status check_view_ID3(FILE *src, ViewInfo *viInfo);

/* Function to check tags */
Status data_view_mp3(const char *data, int size, const char *original);

/* Function to retrieve data from header */
Status get_view_data(int size, ViewInfo *viInfo, char *tag);

/* Function to check version is 3 */
Status check_view_version(FILE *src);

/* Function to check TIT2 tag */
Status check_view_TIT2(ViewInfo *viInfo);

/* Function to check TPE1 tag */
Status check_view_TPE1(ViewInfo *viInfo);

/* Function to check TALB tag */
Status check_view_TALB(ViewInfo *viInfo);

/* Function to check TYER tag */
Status check_view_TYER(ViewInfo *viInfo);

/* Function to check TCON tag */
Status check_view_TCON(ViewInfo *viInfo);

/* Function to check COMM tag */
Status check_view_COMM(ViewInfo *viInfo);

/* Function to retrieve and store title */
Status get_view_title(ViewInfo *viInfo);

/* Function to retrieve and store artist */
Status get_view_artist(ViewInfo *viInfo);

/* Function to retrieve and store album */
Status get_view_album(ViewInfo *viInfo);

/* Function to retrieve and store year */
Status get_view_year(ViewInfo *viInfo);

/* Function to retrieve and store content */
Status get_view_content(ViewInfo *viInfo);

/* Function to retrieve and store composer */
Status get_view_composer(ViewInfo *viInfo);

#endif

