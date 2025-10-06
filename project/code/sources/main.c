#include "ft_ssl.h"

int main(int argc, char **argv)
{
	(void)argv;

	if (argc < 2)
		exit_msg(STDERR_FILENO, "usage: ft_ssl command [command opts] [command args]", EXIT_FAILURE);

	// Check et init des datas
	// Selon prog, appel de la fonction de hash
	// formatage et affichage du resultat
	// clean exit

	exit_msg(STDOUT_FILENO, "Tout bon brother", EXIT_SUCCESS);
	return (0);
}
