#include "components.h"

/*
ret:
	 1 - ok
	 0 - end of file
	-1 - parse error
*/
int element_get_next( FILE *fp, element_t *elem ) {
	char buf[32],
	     *p = buf;
	     
	if (token_get_next( fp, buf, 32 ) == 0) return 0;
	if (!token_check( buf, 1 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: [RVIDCL]<uint>.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	/* type */
	switch (*p++) {
		case 'R': elem->type = RES; break;
		case 'I': elem->type = CUR; break;
		case 'V': elem->type = VOL; break;
		case 'C': elem->type = CAP; break;
		case 'L': elem->type = IND; break;
		case 'D': elem->type = DIO; break;
	}
	/* number */
	elem->number = atoi(p);

	/* source */
	if (token_get_next( fp, buf, 32 ) == 0) {
		fprintf(stderr, "Line %d: missing token. Expected: <node+>.\n", 
		        PARSE_LINE);
		return -1;
	}
	if (!token_check( buf, 2 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <node+>.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	elem->source = atoi(buf);

	/* drain */
	if (token_get_next( fp, buf, 32 ) == 0) {
		fprintf(stderr, "Line %d: missing token. Expected: <node->.\n", 
		        PARSE_LINE);
		return -1;
	}
	if (!token_check( buf, 2 )) {
		fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <node->.\n", 
		        PARSE_LINE, buf);
		return -1;
	}
	elem->drain = atoi(buf);

	/* value */
	if (elem->type != DIO) {
		if (token_get_next( fp, buf, 32 ) == 0 ) {
			fprintf(stderr, "Line %d: missing token. Expected: <value>.\n", 
		        PARSE_LINE);
			return -1;
		}
		if (!token_check( buf, 3)) {
			fprintf(stderr, "Line %d: unknown token \"%s\". Expected: <value>.\n", 
		        PARSE_LINE, buf);
			return -1;
		}
	}
	elem->value = atof(buf);

	return 1;
}

void element_print( element_t *elem ) {
	char type;
	switch ( elem->type ) {
		case RES: type = 'R'; break;
		case CUR: type = 'I'; break;
		case VOL: type = 'V'; break;
		case CAP: type = 'C'; break;
		case IND: type = 'L'; break;
		case DIO: type = 'D'; break;
		default:  type = 'U'; break;
	}
	printf("%c%d %d %d %f\n", type, elem->number, elem->source, elem->drain, elem->value);
}