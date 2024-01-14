#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		// •¶Žš—ñargv‚Ìi”Ô‚ð•\Ž¦
		printf(argv[i]);
		// ‰üs
		printf("\n");
	}

	system("pause");
	return 0;
}