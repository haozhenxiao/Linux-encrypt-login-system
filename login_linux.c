#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <sys/types.h>
#include <crypt.h>
#include "pwent.h"

#define TRUE 1
#define FALSE 0
#define LENGTH 16

void sighandler() {
	signal(SIGINT,SIG_IGN);
}

int main(int argc, char *argv[]) {

	mypwent *passwddata; 
	/* see pwent.h */

	char important[LENGTH] = "***IMPORTANT***";

	char user[LENGTH];
	char prompt[] = "password: ";
	char swap_prompt[]="New password: ";
	char again_prompt[]="Again: ";
	char *user_pass;
	char *new_pass;
	char *again_pass;
	int f_login;
	char *en_pass;
        char *envp[] = { NULL };
	char *argvv[] = { "/bin/sh",NULL};

	sighandler();

	while (TRUE) {
		/* check what important variable contains - do not remove, part of buffer overflow test */
		printf("Value of variable 'important' before input of login name: %s\n",
				important);

		printf("login: ");
		fflush(NULL); /* Flush all  output buffers */
		__fpurge(stdin); /* Purge any data in stdin buffer */

		if (fgets(user,16,stdin) == NULL) /* gets() is vulnerable to buffer */
		{
			exit(0); /*  overflow attacks.  */	
		}	

		/* check to see if important variable is intact after input of login name - do not remove */
		printf("Value of variable 'important' after input of login name: %*.*s\n",
				LENGTH - 1, LENGTH - 1, important);

		user_pass = getpass(prompt);
		passwddata = mygetpwnam(user);

		if (passwddata != NULL) {
			en_pass=crypt(user_pass,passwddata->passwd_salt);

			if (!strcmp(en_pass, passwddata->passwd)) {
				if(passwddata->pwage==10){
					printf("You need to swap your password!!! \n");
					do{
						new_pass=getpass(swap_prompt);
						again_pass=getpass(again_prompt);
					}while(strcmp(new_pass,again_pass));
					printf("Password changed!!! \n");
					passwddata->passwd=new_pass;
					passwddata->pwage=0;
				}else{
					printf(" You're in !\n");
					printf("Number of failed login is %d\n", passwddata->pwfailed);
					passwddata->pwfailed=0;
					passwddata->pwage++;
				}
				mysetpwent(user,passwddata);
				setuid(passwddata->uid);
				execve("/bin/sh",argvv,envp);

			}else{
				if(passwddata->pwfailed==3){
					printf("You attempted too many times \n");
					passwddata->pwfailed=0;
					mysetpwent(user,passwddata);
					return 0;
				}
				printf("Wrong password, please try again!!! \n");
				f_login++;
				passwddata->pwfailed=f_login;
				mysetpwent(user,passwddata);
			}
		}else{
			printf("Login Incorrect \n");
		}
		
	}
	return 0;
}

