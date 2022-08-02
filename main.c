/**************Documentation**************
Name          : Muneer Mohammad Ali
Date          : 19/05/2022
Description   : MP3 Tag Reader Project
Sample Input  : Viewing : ./a.out -v sample.mp3
				Editing : ./a.out -e -y 1991 sample.mp3
Sample Output : Viewing : All details of sample.mp3
				Editing : Edited details
******************************************/
#include <stdio.h>
#include <string.h>
#include "mp3view.h"
#include "edit.h"
#include "types.h"

int main(int argc, char *argv[])
{
    /* Checking if view option selected*/
    if(check_operation_type(argc,argv) == e_view)
    {
        //Declaring a struct variable
        ViewInfo viInfo;
        
        printf("----------------Selected View Details----------------\n");
        printf("-----------------------------------------------------\n");
        printf("----------MP3 TAG READER AND EDITOR FOR ID3----------\n");
        printf("-----------------------------------------------------\n\n");

        /* Validate all arguments passed */
        if(read_and_validate_view_args(argv, &viInfo) == e_success)
        {
            /* If passed, start retrieving details */
            if(do_view(&viInfo) == e_success)
            {
                printf("\n-----------------------------------------------------\n");
                printf("-----------DETAILS DISPLAYED SUCCESSFULLY------------\n\n");
            }
            else
            {
                printf("Details not retrieved!!!\n");
            }
        }
        else
        {
            printf("Reading and validating inputs failed!!!\n");
        }
    }

    /* Checking if edit option selected*/    
    else if(check_operation_type(argc,argv) == e_edit)
    {
        //Declaring a struct variable
        EditInfo edInfo;
        printf("----------------Selected Edit Details----------------\n");
        printf("----------MP3 TAG READER AND EDITOR FOR ID3----------\n");

        /* Validate all arguments passed */
        if(read_and_validate_edit_args(argv, &edInfo) == e_success)
        {
            if(do_edit(&edInfo,argv) == e_success);
            else
            {
                printf("Editing failed!!!\n");
            }
        }
        else
        {
            printf("Reading and validating inputs failed!!!\n");
        }
    }

    /* Check if --help option passed */
    else if(check_operation_type(argc,argv) == e_help)
    {
        /* Help menu */
        printf("----------------------HELP MENU----------------------\n\n");
        printf("%-4s%-4s%-4s%s","1.","-v","->","to view mp3 file contents\n");
        printf("%-4s%-4s%-4s%s","2.","-e","->","to edit mp3 file contents\n");
        printf("%-12s%s\n","","2.1 -t -> to edit song title");
        printf("%-12s%s\n","","2.2 -a -> to edit artist name");
        printf("%-12s%s\n","","2.3 -A -> to edit Album name");
        printf("%-12s%s\n","","2.4 -y -> to edit year");
        printf("%-12s%s\n","","2.5 -m -> to edit content");
        printf("%-12s%s\n","","2.6 -c -> to edit comment");
        printf("\n-----------------------------------------------------\n");
    }

    /* Condition to check invalid options */
    else if(check_operation_type(argc,argv) == e_unsupported)
    {
        /* Usage menu */
        printf("-----------------------------------------------------\n\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE :\n");
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("\n-----------------------------------------------------\n");
    }
        
    return 0;
}

/* function to check options */
OperationType check_operation_type(int argc, char *argv[])
{
    /* Condition for view option */
    if(argc == 3 && strcmp(argv[1],"-v") == 0)
    {
        return e_view;
    }

    /* Condition for edit option */
    else if(argc == 5 && strcmp(argv[1],"-e") == 0)
    {
        return e_edit;
    }

    /* Condition for help option */
    else if(argc == 2 && strcmp(argv[1],"--help") == 0)
    {
        return e_help;
    }

    /* No conditions passed the return unsupported to indicate invalid arguments */
    return e_unsupported;
        
}
