/**
 * cipher.h
 *
 * Function declarations and default values.
 * NO NEED TO MODIFY THIS FILE.
 */

#define DEFAULT_SHIFT 3		// Default key for shift encryption
#define SIZE_OF_ALPHABET 26	// The number of letters in English alphabet

/**
 * Keys for substitution cipher
 */
char *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *SUBSTITUTION_KEY = "GBYXOLNJCQMKHVDFZTWASPEIUR";

/**
 * The frequencies of English letters A through Z.
 *
 * Eg., freq[0] is the frequency of letter 'A' in English.
 */
double freq[] = {
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
	0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
	0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
	0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

/**
 * Function declarations
 */
void shift_enc(unsigned char *input, int key, unsigned char *output);
void shift_dec(unsigned char *input, int key, unsigned char *output);
void shift_bf(unsigned char *input, const char *outFilename);
void subst_enc(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output);
void subst_dec(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output);
void analyzeFreq(unsigned char *input, const char *outFilename);
