#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef _UTILS_H_
#define _UTILS_H_

void init_rand(void);
int randperc(void);
char *x_strcpy(char *d, char *s);
Boolean tst_bit(LongInt *f, int b);
Boolean tst_doublebit(DoubleLongInt *f, int b);
void set_bit(LongInt *f, int b);
void set_doublebit(DoubleLongInt *f, int b);
void clr_bit(LongInt *f, int b);
void clr_doublebit(DoubleLongInt *f, int b);
Boolean match(char *p, char *q);
Boolean infile(char *file, char *line);

#ifdef SYS_EQBUG
int strcasecmp(char *s1, char *s2);
int strncasecmp(char *s1, char *s2, register int n);
#endif

#define TABLE_END ((char *)(-1))

int glookup(char *elem, int n, char **table,
			int (*strcmpfun)(char *s1, char *s2, int n));
int tlookup(char *elem, char **table);
int xlookup(char *elem, char **table);

char *lowercase(char *str);
char *uppercase(char *str);
void *xmalloc(int nelem, int elem_size);
void insertch(char *s, char ch, int i);
Boolean tstbits(int w, int m);
char *my_crypt(char *buf, char *pw, int len);
char *mk_string(char *b, char *str, int k, int stopch);

void *resize_array(void *start, int elem_size, int oldlen, int newlen);

/* For the Int-Set package:
 */
typedef struct
{
	int current; /* current element */
	int len;	 /* #elements in the array */
	int maxlen;	 /* length of array */
	int *list;	 /* pointer to the array */
} int_set;

#define SET_END -65536

/* int_set interface:
 */
void init_intset(int_set *p, int len);
void free_intset(int_set *p);
Boolean add_int(int n, int_set *p);
Boolean remove_int(int n, int_set *p);
int find_int(int n, int_set *p);
int find_int_number(int n, int_set *p);
int foreach_int(int_set *p, int (*func)(int));
void remove_current(int_set *p);
int get_set_mem_usage(int_set *p);

#define int_number(I, P) ((P)->list[I])

#define first_int(P) ((P)->len == 0 ? SET_END : ((P)->current = 1, *(P)->list))
#define next_int(P) ((P)->current < (P)->len ? (P)->list[(P)->current++] : SET_END)

#define set_size(P) ((P)->len)
#define is_empty(P) (set_size(P) == 0)

/* int_table interface:
 */
typedef struct TABLE_ENTRY
{
	long int key;
	long int value;
	struct TABLE_ENTRY *next;
} table_entry;

typedef struct
{
	int len;
	table_entry **table;
} int_table;

#define NOT_IN_TABLE -65536L

void init_inttable(int_table *p, int size);
void free_inttable(int_table *p);

Boolean insert_entry(long int key, long int value, int_table *p);
Boolean remove_entry(long int key, int_table *p);
long int lookup_entry(long int key, int_table *p);
long int change_entry(long int key, long int new_value, int_table *p);
int get_table_mem_usage(int_table *p);

#endif

#endif /* Add nothing past this line... */
