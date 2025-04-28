#ifndef CURP_STRATEGIES
#define CURP_STRATEGIES

static const char vowels[] = { 'A', 'E', 'I', 'O', 'U' };
static const char consonants[] = {
	'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N',
	'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'
};

#define CHARACTER(t, min, max) (char)theft_random_choice(t, max - min) + min
#define NUMBER(t, min, max) theft_random_choice(t, max - min) + min
#define BOOLEAN(t) theft_random_choice(t, 2)
#define UPPER_CHARACTER(t) CHARACTER(t, 'A', 'Z')
#define DIGIT_CHARACTER(t) CHARACTER(t, '0', '9')
#define SAMPLED_FROM(t, a) a[theft_random_choice(t, sizeof(a))]

#define VOWEL(t) SAMPLED_FROM(t, vowels)
#define CONSONANT(t) SAMPLED_FROM(t, consonants)

#endif
