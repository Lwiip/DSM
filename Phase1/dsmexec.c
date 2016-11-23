#include <unistd.h>
#include <signal.h>

#include "common_impl.h"


/* variables globales */
int DSM_NODE_NUM = 0;

/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL;

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;

void usage(void) {
	fprintf(stdout, "Usage : dsmexec machine_file executable arg1 arg2 ...\n");
	fflush(stdout);
	exit(EXIT_FAILURE);
}

void sigchld_handler(int sig) {
	int pid;

	if ((pid = wait(NULL)) == -1) /* suppression du fils zombi */
	{
		 perror("wait handler ");
		 errno = 0;
		 return;
	}
	printf("Prise en compte du fils : %d\n", pid);
}

int count_line(FILE * FP) {
	int nb_line = 0;
	int c;

	while ((c = getc(FP)) != EOF) { //passage par tout les caractères du fichier
		if (c == '\n') {
			nb_line++;
		}
	}
	fseek(FP, 0, SEEK_SET);
	return nb_line;
}

int main(int argc, char *argv[]){
	if (argc < 3) {
		usage();
	} else {
		pid_t pid;
		int num_procs = 0;

		/* Mise en place d'un traitant pour recuperer les fils zombies*/
		struct sigaction custom_sigchild;
		memset(&custom_sigchild, 0, sizeof(struct sigaction));
		custom_sigchild.sa_handler = sigchld_handler;
		sigaction(SIGCHLD, &custom_sigchild , NULL);

		/* lecture du fichier de machines */
		/* 1- on recupere le nombre de processus a lancer */

		FILE * FP = fopen("machine_file", "r");
		if (!FP) {
			printf("Impossible d'ouvrir le fichier\n");
			exit(-1);
		}



		DSM_NODE_NUM = count_line(FP);

		/* 2- on recupere les noms des machines : le nom de */
		/* la machine est un des elements d'identification */
		dsm_proc_t tab_dsm_proc[DSM_NODE_NUM];
		init_tab_dsm_proc(tab_dsm_proc, DSM_NODE_NUM);

		int i = 0;
		int read;
		size_t len = 0;
		while ((read = getline(&(tab_dsm_proc[i].connect_info.name_machine),
				&len, FP)) != -1) {
			if (errno != 0) {
				perror("Erreur lors de la lecture des noms de machines");
				errno = 0;
			}
			tab_dsm_proc[i].connect_info.rank = i;
			i++;
		}

		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		/* creation de la socket d'ecoute */
		/* + ecoute effective */

		/* creation des fils */
		for (i = 0; i < num_procs; i++) {

			/* creation du tube pour rediriger stdout */

			/* creation du tube pour rediriger stderr */

			pid = fork();
			if (pid == -1)
				ERROR_EXIT("fork");

			if (pid == 0) { /* fils */

				/* redirection stdout */

				/* redirection stderr */

				/* Creation du tableau d'arguments pour le ssh */

				/* jump to new prog : */
				/* execvp("ssh",newargv); */

			} else if (pid > 0) { /* pere */
				/* fermeture des extremites des tubes non utiles */
				num_procs_creat++;
			}
		}

		for (i = 0; i < num_procs; i++) {

			/* on accepte les connexions des processus dsm */

			/*  On recupere le nom de la machine distante */
			/* 1- d'abord la taille de la chaine */
			/* 2- puis la chaine elle-meme */

			/* On recupere le pid du processus distant  */

			/* On recupere le numero de port de la socket */
			/* d'ecoute des processus distants */
		}

		/* envoi du nombre de processus aux processus dsm*/

		/* envoi des rangs aux processus dsm */

		/* envoi des infos de connexion aux processus */

		/* gestion des E/S : on recupere les caracteres */
		/* sur les tubes de redirection de stdout/stderr */
		/* while(1)
		 {
		 je recupere les infos sur les tubes de redirection
		 jusqu'à ce qu'ils soient inactifs (ie fermes par les
		 processus dsm ecrivains de l'autre cote ...)

		 };
		 */

		/* on attend les processus fils */

		/* on ferme les descripteurs proprement */

		/* on ferme la socket d'ecoute */
	}
	exit(EXIT_SUCCESS);
}

