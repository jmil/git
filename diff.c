static struct ll_diff_driver {
	const char *name;
	struct ll_diff_driver *next;
	const char *cmd;
} *user_diff, **user_diff_tail;

/*
 * Currently there is only "diff.<drivername>.command" variable;
 * because there are "diff.color.<slot>" variables, we are parsing
 * this in a bit convoluted way to allow low level diff driver
 * called "color".
 */
static int parse_lldiff_command(const char *var, const char *ep, const char *value)
{
	const char *name;
	int namelen;
	struct ll_diff_driver *drv;

	name = var + 5;
	namelen = ep - name;
	for (drv = user_diff; drv; drv = drv->next)
		if (!strncmp(drv->name, name, namelen) && !drv->name[namelen])
			break;
	if (!drv) {
		drv = xcalloc(1, sizeof(struct ll_diff_driver));
		drv->name = xmemdupz(name, namelen);
		if (!user_diff_tail)
			user_diff_tail = &user_diff;
		*user_diff_tail = drv;
		user_diff_tail = &(drv->next);
	}

	return git_config_string(&(drv->cmd), var, value);
}

/*
 * 'diff.<what>.funcname' attribute can be specified in the configuration
 * to define a customized regexp to find the beginning of a function to
 * be used for hunk header lines of "diff -p" style output.
 */
struct funcname_pattern_entry {
	char *name;
	char *pattern;
	int cflags;
};
static struct funcname_pattern_list {
	struct funcname_pattern_list *next;
	struct funcname_pattern_entry e;
} *funcname_pattern_list;

static int parse_funcname_pattern(const char *var, const char *ep, const char *value, int cflags)
{
	const char *name;
	int namelen;
	struct funcname_pattern_list *pp;

	name = var + 5; /* "diff." */
	namelen = ep - name;

	for (pp = funcname_pattern_list; pp; pp = pp->next)
		if (!strncmp(pp->e.name, name, namelen) && !pp->e.name[namelen])
			break;
	if (!pp) {
		pp = xcalloc(1, sizeof(*pp));
		pp->e.name = xmemdupz(name, namelen);
		pp->next = funcname_pattern_list;
		funcname_pattern_list = pp;
	}
	free(pp->e.pattern);
	pp->e.pattern = xstrdup(value);
	pp->e.cflags = cflags;
	return 0;
}

	if (!prefixcmp(var, "diff.")) {
		const char *ep = strrchr(var, '.');

		if (ep != var + 4 && !strcmp(ep, ".command"))
			return parse_lldiff_command(var, ep, value);
	}
	if (!prefixcmp(var, "diff.")) {
		const char *ep = strrchr(var, '.');
		if (ep != var + 4) {
			if (!strcmp(ep, ".funcname")) {
				if (!value)
					return config_error_nonbool(var);
				return parse_funcname_pattern(var, ep, value,
					0);
			} else if (!strcmp(ep, ".xfuncname")) {
				if (!value)
					return config_error_nonbool(var);
				return parse_funcname_pattern(var, ep, value,
					REG_EXTENDED);
			}
		}
	struct strbuf res;
	strbuf_init(&res, 0);
	quote_two_c_style(&a_name, o->a_prefix, name_a, 0);
	quote_two_c_style(&b_name, o->b_prefix, name_b, 0);
	struct xdiff_emit_state xm;
	ecb.outf = xdiff_outf;
	ecb.priv = diff_words;
	diff_words->xm.consume = fn_out_diff_words_aux;
	xdi_diff(&minus, &plus, &xpp, &xecfg, &ecb);

	struct xdiff_emit_state xm;
	struct strbuf name;
	strbuf_init(&name, 0);
	struct xdiff_emit_state xm;

		struct strbuf buf;
		strbuf_init(&buf, 0);
		 * made to the preimage.
	struct xdiff_emit_state xm;
static void setup_diff_attr_check(struct git_attr_check *check)
	static struct git_attr *attr_diff;
	if (!attr_diff) {
		attr_diff = git_attr("diff", 4);
	check[0].attr = attr_diff;
static void diff_filespec_check_attr(struct diff_filespec *one)
	struct git_attr_check attr_diff_check;
	int check_from_data = 0;

	if (one->checked_attr)
		return;

	setup_diff_attr_check(&attr_diff_check);
	one->is_binary = 0;
	one->funcname_pattern_ident = NULL;

	if (!git_checkattr(one->path, 1, &attr_diff_check)) {
		const char *value;

		/* binaryness */
		value = attr_diff_check.value;
		if (ATTR_TRUE(value))
			;
		else if (ATTR_FALSE(value))
			one->is_binary = 1;
		else
			check_from_data = 1;

		/* funcname pattern ident */
		if (ATTR_TRUE(value) || ATTR_FALSE(value) || ATTR_UNSET(value))
			;
		else
			one->funcname_pattern_ident = value;
	}

	if (check_from_data) {
		if (!one->data && DIFF_FILE_VALID(one))
			diff_populate_filespec(one, 0);

		if (one->data)
			one->is_binary = buffer_is_binary(one->data, one->size);
	}
int diff_filespec_is_binary(struct diff_filespec *one)
	diff_filespec_check_attr(one);
	return one->is_binary;
static const struct funcname_pattern_entry *funcname_pattern(const char *ident)
{
	struct funcname_pattern_list *pp;

	for (pp = funcname_pattern_list; pp; pp = pp->next)
		if (!strcmp(ident, pp->e.name))
			return &pp->e;
	return NULL;
}

static const struct funcname_pattern_entry builtin_funcname_pattern[] = {
	{ "java",
	  "!^[ \t]*(catch|do|for|if|instanceof|new|return|switch|throw|while)\n"
	  "^[ \t]*(([ \t]*[A-Za-z_][A-Za-z_0-9]*){2,}[ \t]*\\([^;]*)$",
	  REG_EXTENDED },
	{ "pascal",
	  "^((procedure|function|constructor|destructor|interface|"
		"implementation|initialization|finalization)[ \t]*.*)$"
	  "|"
	  "^(.*=[ \t]*(class|record).*)$",
	  REG_EXTENDED },
	{ "bibtex", "(@[a-zA-Z]{1,}[ \t]*\\{{0,1}[ \t]*[^ \t\"@',\\#}{~%]*).*$",
	  REG_EXTENDED },
	{ "tex",
	  "^(\\\\((sub)*section|chapter|part)\\*{0,1}\\{.*)$",
	  REG_EXTENDED },
	{ "ruby", "^[ \t]*((class|module|def)[ \t].*)$",
	  REG_EXTENDED },
};

static const struct funcname_pattern_entry *diff_funcname_pattern(struct diff_filespec *one)
	const char *ident;
	const struct funcname_pattern_entry *pe;
	int i;

	diff_filespec_check_attr(one);
	ident = one->funcname_pattern_ident;

	if (!ident)
		/*
		 * If the config file has "funcname.default" defined, that
		 * regexp is used; otherwise NULL is returned and xemit uses
		 * the built-in default.
		 */
		return funcname_pattern("default");

	/* Look up custom "funcname.$ident" regexp from config. */
	pe = funcname_pattern(ident);
	if (pe)
		return pe;

	/*
	 * And define built-in fallback patterns here.  Note that
	 * these can be overridden by the user's config settings.
	 */
	for (i = 0; i < ARRAY_SIZE(builtin_funcname_pattern); i++)
		if (!strcmp(ident, builtin_funcname_pattern[i].name))
			return &builtin_funcname_pattern[i];

	return NULL;
	a_one = quote_two(o->a_prefix, name_a + (*name_a == '/'));
	b_two = quote_two(o->b_prefix, name_b + (*name_b == '/'));
	    (diff_filespec_is_binary(one) || diff_filespec_is_binary(two))) {
		const struct funcname_pattern_entry *pe;
		ecb.outf = xdiff_outf;
		ecb.priv = &ecbdata;
		ecbdata.xm.consume = fn_out_consume;
		xdi_diff(&mf1, &mf2, &xpp, &xecfg, &ecb);
		ecb.outf = xdiff_outf;
		ecb.priv = diffstat;
		xdi_diff(&mf1, &mf2, &xpp, &xecfg, &ecb);
	data.xm.consume = checkdiff_consume;
		ecb.outf = xdiff_outf;
		ecb.priv = &data;
		xdi_diff(&mf1, &mf2, &xpp, &xecfg, &ecb);
	struct strbuf buf;
	strbuf_init(&buf, 0);
		struct strbuf buf;
		strbuf_init(&buf, 0);
static const char *external_diff_attr(const char *name)
{
	struct git_attr_check attr_diff_check;

	if (!name)
		return NULL;

	setup_diff_attr_check(&attr_diff_check);
	if (!git_checkattr(name, 1, &attr_diff_check)) {
		const char *value = attr_diff_check.value;
		if (!ATTR_TRUE(value) &&
		    !ATTR_FALSE(value) &&
		    !ATTR_UNSET(value)) {
			struct ll_diff_driver *drv;

			for (drv = user_diff; drv; drv = drv->next)
				if (!strcmp(drv->name, value))
					return drv->cmd;
		}
	}
	return NULL;
}

		const char *cmd = external_diff_attr(attr_path);
		if (cmd)
			pgm = cmd;
	options->a_prefix = "a/";
	options->b_prefix = "b/";
	struct xdiff_emit_state xm;
	SHA_CTX *ctx;
	SHA1_Update(data->ctx, line, new_len);
	SHA_CTX ctx;
	SHA1_Init(&ctx);
	data.xm.consume = patch_id_consume;
		SHA1_Update(&ctx, buffer, len1);
		ecb.outf = xdiff_outf;
		ecb.priv = &data;
		xdi_diff(&mf1, &mf2, &xpp, &xecfg, &ecb);
	SHA1_Final(sha1, &ctx);
		diffstat.xm.consume = diffstat_consume;