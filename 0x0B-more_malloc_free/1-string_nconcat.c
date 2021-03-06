#include <stdlib.h>

/**
 * _strlen - string length
 * @str: string
 *
 * Return: string length
 */

int _strlen(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		;

	return (i);

}


/**
 * string_nconcat - concats string
 * @s1: first string
 * @s2: second string
 * @n: how many to concat
 *
 * Return: pointer to string or NULL
 */

char *string_nconcat(char *s1, char *s2, unsigned int n)
{
	char *ar, *empty;
	unsigned int i, j, len1, len2;

	empty = "";

	if (s1 == NULL)
		s1 = empty;

	if (s2 == NULL)
		s2 = empty;

	len1 = _strlen(s1);
	len2 = _strlen(s2);

	if (n >= len2)
		n = len2;

	ar = malloc(len1 + n + 1);

	if (ar == NULL)
		return (NULL);

	for (i = 0; s1[i] != '\0'; i++)
		ar[i] = s1[i];

	if (len2 < n)
	{
		for (j = 0; s2[j] != '\0'; i++, j++)
			ar[i] = s2[j];
	}
	else
	{
		for (j = 0; j < n; i++, j++)
			ar[i] = s2[j];
	}

	ar[i] = '\0';

	return (ar);

}
