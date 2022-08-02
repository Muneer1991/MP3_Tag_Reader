/* This file contains the function prototypes and struct necessary for editing */

#include <stdio.h>
#ifndef EDIT_H
#define EDIT_H

#include "types.h" // Contains user defined types

/* Structure to store information required for editing */

typedef struct _EditInfo
{
    /* Edit mp3 info */
    char *edit_mp3_fname;
    char option[3];        //Array to store options -t/-a/-A/-y/-m/-c
    FILE *fptr_edit_mp3;
    char *Title_edit;
    char *Artist_edit;
    char *Album_edit;
    char *Year_edit;
    char *Content_edit;
    char *Composer_edit;

} EditInfo;

/* Editing function prototype */
/* Check operation type */

/* Read and validate arguments for editing */
Status read_and_validate_edit_args(char *argv[], EditInfo *edInfo);

/* Function to edit details */
Status do_edit(EditInfo *edInfo,char *argv[]);

/* Function to open files in read and write (r+) mode */
Status open_edit_files(EditInfo *edInfo);

/* Function to check for ID3 tag */
Status check_edit_ID3(FILE *src, EditInfo *edInfo);

/* Function to check tags */
Status data_edit_mp3(const char *data, int size, const char *original);

/* Function to verify version is 3 */
Status check_edit_version(FILE *src);

/* Function to verify tag is TIT2 */
Status check_edit_TIT2(EditInfo *edInfo);

/* Function to verify tag is TPE1 */
Status check_edit_TPE1(EditInfo *edInfo);

/* Function to verify tag is TALB */
Status check_edit_TALB(EditInfo *edInfo);

/* Function to verify tag is TYER */
Status check_edit_TYER(EditInfo *edInfo);

/* Function to verify tag is TCON */
Status check_edit_TCON(EditInfo *edInfo);

/* Function to verify tag is COMM */
Status check_edit_COMM(EditInfo *edInfo);

/* Function to edit title */
Status get_edit_title(EditInfo *edInfo,char *argv[]);

/* Function to edit artist */
Status get_edit_artist(EditInfo *edInfo,char *argv[]);

/* Function to edit album */
Status get_edit_album(EditInfo *edInfo,char *argv[]);

/* Function to edit year */
Status get_edit_year(EditInfo *edInfo,char *argv[]);

/* Function to edit content */
Status get_edit_content(EditInfo *edInfo,char *argv[]);

/* Function to edit composer */
Status get_edit_composer(EditInfo *edInfo,char *argv[]);

#endif

