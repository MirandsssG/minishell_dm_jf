/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:25:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/13 18:03:30 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(char const *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	free_words(char **words, size_t count)
{
	if (count == 0)
	{
		free(words);
		return ;
	}
	free(words[count - 1]);
	free_words(words, count - 1);
}

static char	*split_word(char const **s, char c)
{
	const char	*ptr_c;
	char		*word;

	while (**s && **s == c)
		(*s)++;
	ptr_c = ft_strchr(*s, c);
	if (ptr_c != NULL)
	{
		word = ft_substr(*s, 0, ptr_c - *s);
		*s = ptr_c;
		while (**s && **s == c)
			(*s)++;
	}
	else
	{
		word = ft_substr(*s, 0, ft_strlen(*s));
		*s += ft_strlen(*s);
	}
	return (word);
}

static char	**combine_w(char **words, char const *s, char c, size_t word_c)
{
	size_t	i;

	i = 0;
	while (i < word_c)
	{
		words[i] = split_word(&s, c);
		if (!words[i])
		{
			free_words(words, i);
			return (NULL);
		}
		i++;
	}
	words[i] = NULL;
	return (words);
}

char	**ft_split(char const *s, char c)
{
	size_t	word_c;
	char	**words;

	if (!s)
		return (NULL);
	word_c = word_count(s, c);
	words = malloc(sizeof(char *) * (word_c + 1));
	if (!words)
		return (NULL);
	if (!combine_w(words, s, c, word_c))
		return (NULL);
	return (words);
}
