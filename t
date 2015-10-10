	    location = symtabindex++;
D(	printf("hidden definition '%s' at %p\n",id,(void *)LOC2INT(location)); )
	    location->name = (char *) malloc(strlen(id) + 1);
	    strcpy(location->name, id);
	    location->u.body = NULL; /* may be assigned later */
	}
#ifdef NO_HELP_LOCAL_SYMBOLS
	location->is_local = 1;
#endif
	location->next = display[display_enter];
	display[display_enter] = location;
