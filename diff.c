#include "userdiff.h"
static int diff_rename_limit_default = 200;
static int diff_suppress_blank_empty;
int diff_use_color_default = -1;
static int diff_mnemonic_prefix;
static void diff_filespec_load_driver(struct diff_filespec *one);
static char *run_textconv(const char *, struct diff_filespec *, size_t *);

int git_diff_ui_config(const char *var, const char *value, void *cb)
	if (!strcmp(var, "diff.mnemonicprefix")) {
		diff_mnemonic_prefix = git_config_bool(var, value);
	if (!strcmp(var, "diff.external"))
		return git_config_string(&external_diff_cmd_cfg, var, value);
	return git_diff_basic_config(var, value, cb);
int git_diff_basic_config(const char *var, const char *value, void *cb)
	if (!strcmp(var, "diff.renamelimit")) {
		diff_rename_limit_default = git_config_int(var, value);
		return 0;
	}

	switch (userdiff_config(var, value)) {
		case 0: break;
		case -1: return -1;
		default: return 0;
	}

	/* like GNU diff's --suppress-blank-empty option  */
	if (!strcmp(var, "diff.suppress-blank-empty")) {
		diff_suppress_blank_empty = git_config_bool(var, value);
		return 0;
	return git_color_default_config(var, value, cb);
	struct strbuf res = STRBUF_INIT;
static void print_line_count(FILE *file, int count)
		fprintf(file, "0,0");
		fprintf(file, "1");
		fprintf(file, "1,%d", count);
static void copy_file_with_prefix(FILE *file,
				  int prefix, const char *data, int size,
			fputs(set, file);
			putc(prefix, file);
			fputs(reset, file);
		putc(ch, file);
		fprintf(file, "%s\n\\ No newline at end of file\n", reset);
			      const char *textconv_one,
			      const char *textconv_two,
	const char *a_prefix, *b_prefix;
	const char *data_one, *data_two;
	size_t size_one, size_two;

	if (diff_mnemonic_prefix && DIFF_OPT_TST(o, REVERSE_DIFF)) {
		a_prefix = o->b_prefix;
		b_prefix = o->a_prefix;
	} else {
		a_prefix = o->a_prefix;
		b_prefix = o->b_prefix;
	}
	quote_two_c_style(&a_name, a_prefix, name_a, 0);
	quote_two_c_style(&b_name, b_prefix, name_b, 0);
	if (textconv_one) {
		data_one = run_textconv(textconv_one, one, &size_one);
		if (!data_one)
			die("unable to read files to diff");
	}
	else {
		data_one = one->data;
		size_one = one->size;
	}
	if (textconv_two) {
		data_two = run_textconv(textconv_two, two, &size_two);
		if (!data_two)
			die("unable to read files to diff");
	}
	else {
		data_two = two->data;
		size_two = two->size;
	}

	lc_a = count_lines(data_one, size_one);
	lc_b = count_lines(data_two, size_two);
	fprintf(o->file,
		"%s--- %s%s%s\n%s+++ %s%s%s\n%s@@ -",
		metainfo, a_name.buf, name_a_tab, reset,
		metainfo, b_name.buf, name_b_tab, reset, fraginfo);
	print_line_count(o->file, lc_a);
	fprintf(o->file, " +");
	print_line_count(o->file, lc_b);
	fprintf(o->file, " @@%s\n", reset);
		copy_file_with_prefix(o->file, '-', data_one, size_one, old, reset);
		copy_file_with_prefix(o->file, '+', data_two, size_two, new, reset);

	FILE *file;
static void print_word(FILE *file, struct diff_words_buffer *buffer, int len, int color,
	fputs(diff_get_color(1, color), file);
	fwrite(ptr, len, 1, file);
	fputs(diff_get_color(1, DIFF_RESET), file);
			putc('\n', file);
			putc('\n', diff_words->file);
			print_word(diff_words->file,
				   &diff_words->minus, len, DIFF_FILE_OLD, 1);
			print_word(diff_words->file,
				   &diff_words->plus, len, DIFF_FILE_NEW, 0);
			print_word(diff_words->file,
				   &diff_words->plus, len, DIFF_PLAIN, 0);
	memset(&xpp, 0, sizeof(xpp));
	xdi_diff_outf(&minus, &plus, fn_out_diff_words_aux, diff_words,
		      &xpp, &xecfg, &ecb);
		putc('\n', diff_words->file);
	FILE *file;
		free (ecbdata->diff_words->minus.text.ptr);
		free (ecbdata->diff_words->plus.text.ptr);
static void emit_line(FILE *file, const char *set, const char *reset, const char *line, int len)
	int has_trailing_newline, has_trailing_carriage_return;

	has_trailing_newline = (len > 0 && line[len-1] == '\n');
	if (has_trailing_newline)
		len--;
	has_trailing_carriage_return = (len > 0 && line[len-1] == '\r');
	if (has_trailing_carriage_return)
		len--;

	fputs(set, file);
	fwrite(line, len, 1, file);
	fputs(reset, file);
	if (has_trailing_carriage_return)
		fputc('\r', file);
	if (has_trailing_newline)
		fputc('\n', file);
		emit_line(ecbdata->file, set, reset, line, len);
		emit_line(ecbdata->file, set, reset, line, ecbdata->nparents);
		ws_check_emit(line + ecbdata->nparents,
			      len - ecbdata->nparents, ecbdata->ws_rule,
			      ecbdata->file, set, reset, ws);
		fprintf(ecbdata->file, "%s--- %s%s%s\n",
			meta, ecbdata->label_path[0], reset, name_a_tab);
		fprintf(ecbdata->file, "%s+++ %s%s%s\n",
			meta, ecbdata->label_path[1], reset, name_b_tab);
	if (diff_suppress_blank_empty
	    && len == 2 && line[0] == ' ' && line[1] == '\n') {
		line[0] = '\n';
		len = 1;
	}

		emit_line(ecbdata->file,
			  diff_get_color(ecbdata->color_diff, DIFF_FRAGINFO),
			putc('\n', ecbdata->file);
		emit_line(ecbdata->file, reset, reset, line, len);
		emit_line(ecbdata->file, plain, reset, line, len);
		emit_line(ecbdata->file,
			  diff_get_color(ecbdata->color_diff, color),
	struct strbuf name = STRBUF_INIT;
static void show_name(FILE *file,
		      const char *prefix, const char *name, int len,
	fprintf(file, " %s%s%-*s%s |", set, prefix, len, name, reset);
static void show_graph(FILE *file, char ch, int cnt, const char *set, const char *reset)
	fprintf(file, "%s", set);
		putc(ch, file);
	fprintf(file, "%s", reset);
		struct strbuf buf = STRBUF_INIT;
	if (width < 25)
		width = 25;
	if (name_width < 10)
		name_width = 10;
	else if (width < name_width + 15)
		name_width = width - 15;
			show_name(options->file, prefix, name, len, reset, set);
			fprintf(options->file, "  Bin ");
			fprintf(options->file, "%s%d%s", del_c, deleted, reset);
			fprintf(options->file, " -> ");
			fprintf(options->file, "%s%d%s", add_c, added, reset);
			fprintf(options->file, " bytes");
			fprintf(options->file, "\n");
			show_name(options->file, prefix, name, len, reset, set);
			fprintf(options->file, "  Unmerged\n");
		show_name(options->file, prefix, name, len, reset, set);
		fprintf(options->file, "%5d%s", added + deleted,
				added + deleted ? " " : "");
		show_graph(options->file, '+', add, add_c, reset);
		show_graph(options->file, '-', del, del_c, reset);
		fprintf(options->file, "\n");
	}
	fprintf(options->file,
	       "%s %d files changed, %d insertions(+), %d deletions(-)%s\n",
static void show_shortstats(struct diffstat_t* data, struct diff_options *options)
	fprintf(options->file, " %d files changed, %d insertions(+), %d deletions(-)\n",
			fprintf(options->file, "-\t-\t");
			fprintf(options->file,
				"%d\t%d\t", file->added, file->deleted);
				write_name_quoted(file->name, options->file,
				fputs(file->print_name, options->file);
				putc(options->line_termination, options->file);
				putc('\0', options->file);
				write_name_quoted(file->from_name, options->file, '\0');
			write_name_quoted(file->name, options->file, '\0');
struct dirstat_file {
	const char *name;
	unsigned long changed;
};

struct dirstat_dir {
	struct dirstat_file *files;
	int alloc, nr, percent, cumulative;
};

static long gather_dirstat(FILE *file, struct dirstat_dir *dir, unsigned long changed, const char *base, int baselen)
{
	unsigned long this_dir = 0;
	unsigned int sources = 0;

	while (dir->nr) {
		struct dirstat_file *f = dir->files;
		int namelen = strlen(f->name);
		unsigned long this;
		char *slash;

		if (namelen < baselen)
			break;
		if (memcmp(f->name, base, baselen))
			break;
		slash = strchr(f->name + baselen, '/');
		if (slash) {
			int newbaselen = slash + 1 - f->name;
			this = gather_dirstat(file, dir, changed, f->name, newbaselen);
			sources++;
		} else {
			this = f->changed;
			dir->files++;
			dir->nr--;
			sources += 2;
		}
		this_dir += this;
	}

	/*
	 * We don't report dirstat's for
	 *  - the top level
	 *  - or cases where everything came from a single directory
	 *    under this directory (sources == 1).
	 */
	if (baselen && sources != 1) {
		int permille = this_dir * 1000 / changed;
		if (permille) {
			int percent = permille / 10;
			if (percent >= dir->percent) {
				fprintf(file, "%4d.%01d%% %.*s\n", percent, permille % 10, baselen, base);
				if (!dir->cumulative)
					return 0;
			}
		}
	}
	return this_dir;
}

static int dirstat_compare(const void *_a, const void *_b)
{
	const struct dirstat_file *a = _a;
	const struct dirstat_file *b = _b;
	return strcmp(a->name, b->name);
}

static void show_dirstat(struct diff_options *options)
{
	int i;
	unsigned long changed;
	struct dirstat_dir dir;
	struct diff_queue_struct *q = &diff_queued_diff;

	dir.files = NULL;
	dir.alloc = 0;
	dir.nr = 0;
	dir.percent = options->dirstat_percent;
	dir.cumulative = DIFF_OPT_TST(options, DIRSTAT_CUMULATIVE);

	changed = 0;
	for (i = 0; i < q->nr; i++) {
		struct diff_filepair *p = q->queue[i];
		const char *name;
		unsigned long copied, added, damage;

		name = p->one->path ? p->one->path : p->two->path;

		if (DIFF_FILE_VALID(p->one) && DIFF_FILE_VALID(p->two)) {
			diff_populate_filespec(p->one, 0);
			diff_populate_filespec(p->two, 0);
			diffcore_count_changes(p->one, p->two, NULL, NULL, 0,
					       &copied, &added);
			diff_free_filespec_data(p->one);
			diff_free_filespec_data(p->two);
		} else if (DIFF_FILE_VALID(p->one)) {
			diff_populate_filespec(p->one, 1);
			copied = added = 0;
			diff_free_filespec_data(p->one);
		} else if (DIFF_FILE_VALID(p->two)) {
			diff_populate_filespec(p->two, 1);
			copied = 0;
			added = p->two->size;
			diff_free_filespec_data(p->two);
		} else
			continue;

		/*
		 * Original minus copied is the removed material,
		 * added is the new material.  They are both damages
		 * made to the preimage. In --dirstat-by-file mode, count
		 * damaged files, not damaged lines. This is done by
		 * counting only a single damaged line per file.
		 */
		damage = (p->one->size - copied) + added;
		if (DIFF_OPT_TST(options, DIRSTAT_BY_FILE) && damage > 0)
			damage = 1;

		ALLOC_GROW(dir.files, dir.nr + 1, dir.alloc);
		dir.files[dir.nr].name = name;
		dir.files[dir.nr].changed = damage;
		changed += damage;
		dir.nr++;
	}

	/* This can happen even with many files, if everything was renames */
	if (!changed)
		return;

	/* Show all directories with more than x% of the changes */
	qsort(dir.files, dir.nr, sizeof(dir.files[0]), dirstat_compare);
	gather_dirstat(options->file, &dir, changed, "", 0);
}

	int lineno;
	struct diff_options *o;
	int trailing_blanks_start;
static int is_conflict_marker(const char *line, unsigned long len)
{
	char firstchar;
	int cnt;

	if (len < 8)
		return 0;
	firstchar = line[0];
	switch (firstchar) {
	case '=': case '>': case '<':
		break;
	default:
		return 0;
	}
	for (cnt = 1; cnt < 7; cnt++)
		if (line[cnt] != firstchar)
			return 0;
	/* line[0] thru line[6] are same as firstchar */
	if (firstchar == '=') {
		/* divider between ours and theirs? */
		if (len != 8 || line[7] != '\n')
			return 0;
	} else if (len < 8 || !isspace(line[7])) {
		/* not divider before ours nor after theirs */
		return 0;
	}
	return 1;
}

	int color_diff = DIFF_OPT_TST(data->o, COLOR_DIFF);
	const char *ws = diff_get_color(color_diff, DIFF_WHITESPACE);
	const char *reset = diff_get_color(color_diff, DIFF_RESET);
	const char *set = diff_get_color(color_diff, DIFF_FILE_NEW);
		unsigned bad;
		if (!ws_blank_line(line + 1, len - 1, data->ws_rule))
			data->trailing_blanks_start = 0;
		else if (!data->trailing_blanks_start)
			data->trailing_blanks_start = data->lineno;
		if (is_conflict_marker(line + 1, len - 1)) {
			data->status |= 1;
			fprintf(data->o->file,
				"%s:%d: leftover conflict marker\n",
				data->filename, data->lineno);
		}
		bad = ws_check(line + 1, len - 1, data->ws_rule);
		if (!bad)
		data->status |= bad;
		err = whitespace_error_string(bad);
		fprintf(data->o->file, "%s:%d: %s.\n",
			data->filename, data->lineno, err);
		emit_line(data->o->file, set, reset, line, 1);
		ws_check_emit(line + 1, len - 1, data->ws_rule,
			      data->o->file, set, reset, ws);
	} else if (line[0] == ' ') {
		data->trailing_blanks_start = 0;
	} else if (line[0] == '@') {
		data->trailing_blanks_start = 0;
static void emit_binary_diff_body(FILE *file, mmfile_t *one, mmfile_t *two)
		fprintf(file, "delta %lu\n", orig_size);
		fprintf(file, "literal %lu\n", two->size);
		fputs(line, file);
		fputc('\n', file);
	fprintf(file, "\n");
static void emit_binary_diff(FILE *file, mmfile_t *one, mmfile_t *two)
	fprintf(file, "GIT binary patch\n");
	emit_binary_diff_body(file, one, two);
	emit_binary_diff_body(file, two, one);
static void diff_filespec_load_driver(struct diff_filespec *one)
	if (!one->driver)
		one->driver = userdiff_find_by_path(one->path);
	if (!one->driver)
		one->driver = userdiff_find_by_name("default");
int diff_filespec_is_binary(struct diff_filespec *one)
	if (one->is_binary == -1) {
		diff_filespec_load_driver(one);
		if (one->driver->binary != -1)
			one->is_binary = one->driver->binary;
		else {
			if (!one->data && DIFF_FILE_VALID(one))
				diff_populate_filespec(one, 0);
			if (one->data)
				one->is_binary = buffer_is_binary(one->data,
						one->size);
			if (one->is_binary == -1)
				one->is_binary = 0;
		}
	return one->is_binary;
static const struct userdiff_funcname *diff_funcname_pattern(struct diff_filespec *one)
	diff_filespec_load_driver(one);
	return one->driver->funcname.pattern ? &one->driver->funcname : NULL;
void diff_set_mnemonic_prefix(struct diff_options *options, const char *a, const char *b)
	if (!options->a_prefix)
		options->a_prefix = a;
	if (!options->b_prefix)
		options->b_prefix = b;
static const char *get_textconv(struct diff_filespec *one)
	if (!DIFF_FILE_VALID(one))
		return NULL;
	if (!S_ISREG(one->mode))
		return NULL;
	diff_filespec_load_driver(one);
	return one->driver->textconv;
	const char *a_prefix, *b_prefix;
	const char *textconv_one = NULL, *textconv_two = NULL;

	if (DIFF_OPT_TST(o, ALLOW_TEXTCONV)) {
		textconv_one = get_textconv(one);
		textconv_two = get_textconv(two);
	}

	diff_set_mnemonic_prefix(o, "a/", "b/");
	if (DIFF_OPT_TST(o, REVERSE_DIFF)) {
		a_prefix = o->b_prefix;
		b_prefix = o->a_prefix;
	} else {
		a_prefix = o->a_prefix;
		b_prefix = o->b_prefix;
	}

	/* Never use a non-valid filename anywhere if at all possible */
	name_a = DIFF_FILE_VALID(one) ? name_a : name_b;
	name_b = DIFF_FILE_VALID(two) ? name_b : name_a;
	a_one = quote_two(a_prefix, name_a + (*name_a == '/'));
	b_two = quote_two(b_prefix, name_b + (*name_b == '/'));
	fprintf(o->file, "%sdiff --git %s %s%s\n", set, a_one, b_two, reset);
		fprintf(o->file, "%snew file mode %06o%s\n", set, two->mode, reset);
			fprintf(o->file, "%s%s%s\n", set, xfrm_msg, reset);
		fprintf(o->file, "%sdeleted file mode %06o%s\n", set, one->mode, reset);
			fprintf(o->file, "%s%s%s\n", set, xfrm_msg, reset);
			fprintf(o->file, "%sold mode %06o%s\n", set, one->mode, reset);
			fprintf(o->file, "%snew mode %06o%s\n", set, two->mode, reset);
			fprintf(o->file, "%s%s%s\n", set, xfrm_msg, reset);
		if (complete_rewrite &&
		    (textconv_one || !diff_filespec_is_binary(one)) &&
		    (textconv_two || !diff_filespec_is_binary(two))) {
			emit_rewrite_diff(name_a, name_b, one, two,
						textconv_one, textconv_two, o);
	    ( (diff_filespec_is_binary(one) && !textconv_one) ||
	      (diff_filespec_is_binary(two) && !textconv_two) )) {
			emit_binary_diff(o->file, &mf1, &mf2);
			fprintf(o->file, "Binary files %s and %s differ\n",
				lbl[0], lbl[1]);
		const struct userdiff_funcname *pe;

		if (textconv_one) {
			size_t size;
			mf1.ptr = run_textconv(textconv_one, one, &size);
			if (!mf1.ptr)
				die("unable to read files to diff");
			mf1.size = size;
		}
		if (textconv_two) {
			size_t size;
			mf2.ptr = run_textconv(textconv_two, two, &size);
			if (!mf2.ptr)
				die("unable to read files to diff");
			mf2.size = size;
		}
		pe = diff_funcname_pattern(one);
		if (!pe)
			pe = diff_funcname_pattern(two);
		memset(&xpp, 0, sizeof(xpp));
		ecbdata.file = o->file;
		if (pe)
			xdiff_set_find_func(&xecfg, pe->pattern, pe->cflags);
		if (DIFF_OPT_TST(o, COLOR_DIFF_WORDS)) {
			ecbdata.diff_words->file = o->file;
		}
		xdi_diff_outf(&mf1, &mf2, fn_out_consume, &ecbdata,
			      &xpp, &xecfg, &ecb);
		if (textconv_one)
			free(mf1.ptr);
		if (textconv_two)
			free(mf2.ptr);
		memset(&xpp, 0, sizeof(xpp));
		xdi_diff_outf(&mf1, &mf2, diffstat_consume, diffstat,
			      &xpp, &xecfg, &ecb);
			      const char *attr_path,
			      struct diff_filespec *one,
			      struct diff_filespec *two,
			      struct diff_options *o)
	data.o = o;
	data.ws_rule = whitespace_rule(attr_path);
	/*
	 * All the other codepaths check both sides, but not checking
	 * the "old" side here is deliberate.  We are checking the newly
	 * introduced changes, and as long as the "new" side is text, we
	 * can and should check what it introduces.
	 */
		memset(&xpp, 0, sizeof(xpp));
		xecfg.ctxlen = 1; /* at least one context line */
		xdi_diff_outf(&mf1, &mf2, checkdiff_consume, &data,
			      &xpp, &xecfg, &ecb);

		if ((data.ws_rule & WS_TRAILING_SPACE) &&
		    data.trailing_blanks_start) {
			fprintf(o->file, "%s:%d: ends with blank lines.\n",
				data.filename, data.trailing_blanks_start);
			data.status = 1; /* report errors */
		}
	spec->is_binary = -1;

	/*
	 * This is not the sha1 we are looking for, or
	 * unreusable because it is not a regular file.
	 */
	if (hashcmp(sha1, ce->sha1) || !S_ISREG(ce->ce_mode))

	/*
	 * If ce matches the file in the work tree, we can reuse it.
	if (ce_uptodate(ce) ||
	    (!lstat(name, &st) && !ce_match_stat(ce, &st, 0)))
		return 1;

	return 0;
	struct strbuf buf = STRBUF_INIT;
		struct strbuf buf = STRBUF_INIT;
			struct strbuf sb = STRBUF_INIT;

			if (strbuf_readlink(&sb, s->path, s->size))
			s->data = strbuf_detach(&sb, &s->size);
			s->should_free = 1;
		if (size_only)
			return 0;
		if (convert_to_git(s->path, s->data, s->size, &buf, safe_crlf)) {
			ret = readlink(name, buf, sizeof(buf));
			if (ret == sizeof(buf))
				die("symlink too long: %s", name);
			prep_temp_blob(temp, buf, ret,
			 const char *attr_path,
		struct userdiff_driver *drv = userdiff_find_by_path(attr_path);
		if (drv && drv->external)
			pgm = drv->external;
		fprintf(o->file, "* Unmerged path %s\n", name);
static void strip_prefix(int prefix_length, const char **namep, const char **otherp)
{
	/* Strip the prefix but do not molest /dev/null and absolute paths */
	if (*namep && **namep != '/')
		*namep += prefix_length;
	if (*otherp && **otherp != '/')
		*otherp += prefix_length;
}

	const char *attr_path;
	name  = p->one->path;
	other = (strcmp(name, p->two->path) ? p->two->path : NULL);
	attr_path = name;
	if (o->prefix_length)
		strip_prefix(o->prefix_length, &name, &other);
		run_diff_cmd(pgm, name, NULL, attr_path,
			     NULL, NULL, NULL, o, 0);
		run_diff_cmd(NULL, name, other, attr_path,
			     one, null, xfrm_msg, o, 0);
		run_diff_cmd(NULL, name, other, attr_path,
			     null, two, xfrm_msg, o, 0);
		run_diff_cmd(pgm, name, other, attr_path,
			     one, two, xfrm_msg, o, complete_rewrite);
	if (o->prefix_length)
		strip_prefix(o->prefix_length, &name, &other);

	const char *attr_path;
	attr_path = other ? other : name;

	if (o->prefix_length)
		strip_prefix(o->prefix_length, &name, &other);
	builtin_checkdiff(name, other, attr_path, p->one, p->two, o);

	options->file = stdout;

	options->dirstat_percent = 3;
	DIFF_OPT_CLR(options, DIRSTAT_CUMULATIVE);
	if (diff_use_color_default > 0)
	if (!diff_mnemonic_prefix) {
		options->a_prefix = "a/";
		options->b_prefix = "b/";
	}
	if (!DIFF_OPT_TST(options, RELATIVE_NAME))
		options->prefix = NULL;
	if (options->prefix)
		options->prefix_length = strlen(options->prefix);
	else
		options->prefix_length = 0;

					    DIFF_FORMAT_DIRSTAT |
				      DIFF_FORMAT_DIRSTAT |
	else if (opt_arg(arg, 'X', "dirstat", &options->dirstat_percent))
		options->output_format |= DIFF_FORMAT_DIRSTAT;
	else if (!strcmp(arg, "--cumulative")) {
		options->output_format |= DIFF_FORMAT_DIRSTAT;
		DIFF_OPT_SET(options, DIRSTAT_CUMULATIVE);
	} else if (opt_arg(arg, 0, "dirstat-by-file",
			   &options->dirstat_percent)) {
		options->output_format |= DIFF_FORMAT_DIRSTAT;
		DIFF_OPT_SET(options, DIRSTAT_BY_FILE);
	}
	else if (!strcmp(arg, "--relative"))
		DIFF_OPT_SET(options, RELATIVE_NAME);
	else if (!prefixcmp(arg, "--relative=")) {
		DIFF_OPT_SET(options, RELATIVE_NAME);
		options->prefix = arg + 11;
	}
	else if (!strcmp(arg, "--textconv"))
		DIFF_OPT_SET(options, ALLOW_TEXTCONV);
	else if (!strcmp(arg, "--no-textconv"))
		DIFF_OPT_CLR(options, ALLOW_TEXTCONV);
	else if (!strcmp(arg, "--ignore-submodules"))
		DIFF_OPT_SET(options, IGNORE_SUBMODULES);
	else if (!prefixcmp(arg, "--output=")) {
		options->file = fopen(arg + strlen("--output="), "w");
		options->close_file = 1;
	} else
		fprintf(opt->file, ":%06o %06o %s ", p->one->mode, p->two->mode,
			diff_unique_abbrev(p->one->sha1, opt->abbrev));
		fprintf(opt->file, "%s ", diff_unique_abbrev(p->two->sha1, opt->abbrev));
		fprintf(opt->file, "%c%03d%c", p->status, similarity_index(p),
			inter_name_termination);
		fprintf(opt->file, "%c%c", p->status, inter_name_termination);
	if (p->status == DIFF_STATUS_COPIED ||
	    p->status == DIFF_STATUS_RENAMED) {
		const char *name_a, *name_b;
		name_a = p->one->path;
		name_b = p->two->path;
		strip_prefix(opt->prefix_length, &name_a, &name_b);
		write_name_quoted(name_a, opt->file, inter_name_termination);
		write_name_quoted(name_b, opt->file, line_termination);
		const char *name_a, *name_b;
		name_a = p->one->mode ? p->one->path : p->two->path;
		name_b = NULL;
		strip_prefix(opt->prefix_length, &name_a, &name_b);
		write_name_quoted(name_a, opt->file, line_termination);
	else if (fmt & DIFF_FORMAT_NAME) {
		const char *name_a, *name_b;
		name_a = p->two->path;
		name_b = NULL;
		strip_prefix(opt->prefix_length, &name_a, &name_b);
		write_name_quoted(name_a, opt->file, opt->line_termination);
	}
static void show_file_mode_name(FILE *file, const char *newdelete, struct diff_filespec *fs)
		fprintf(file, " %s mode %06o ", newdelete, fs->mode);
		fprintf(file, " %s ", newdelete);
	write_name_quoted(fs->path, file, '\n');
static void show_mode_change(FILE *file, struct diff_filepair *p, int show_name)
		fprintf(file, " mode change %06o => %06o%c", p->one->mode, p->two->mode,
			write_name_quoted(p->two->path, file, '\n');
static void show_rename_copy(FILE *file, const char *renamecopy, struct diff_filepair *p)
	fprintf(file, " %s %s (%d%%)\n", renamecopy, names, similarity_index(p));
	show_mode_change(file, p, 0);
static void diff_summary(FILE *file, struct diff_filepair *p)
		show_file_mode_name(file, "delete", p->one);
		show_file_mode_name(file, "create", p->two);
		show_rename_copy(file, "copy", p);
		show_rename_copy(file, "rename", p);
			fputs(" rewrite ", file);
			write_name_quoted(p->two->path, file, ' ');
			fprintf(file, "(%d%%)\n", similarity_index(p));
		show_mode_change(file, p, !p->score);
	git_SHA_CTX *ctx;
	git_SHA1_Update(data->ctx, line, new_len);
	git_SHA_CTX ctx;
	git_SHA1_Init(&ctx);
		memset(&xpp, 0, sizeof(xpp));
		git_SHA1_Update(&ctx, buffer, len1);
		xdi_diff_outf(&mf1, &mf2, patch_id_consume, &data,
			      &xpp, &xecfg, &ecb);
	git_SHA1_Final(sha1, &ctx);
			show_shortstats(&diffstat, options);
	if (output_format & DIFF_FORMAT_DIRSTAT)
		show_dirstat(options);
			diff_summary(options->file, q->queue[i]);
			putc(options->line_termination, options->file);
				fputs(options->stat_sep, options->file);
	if (options->close_file)
		fclose(options->file);
	if (S_ISGITLINK(one->mode))
		return 0;
	if (options->skip_stat_unmatch)
		    const char *concatpath)
	if (DIFF_OPT_TST(options, IGNORE_SUBMODULES) && S_ISGITLINK(mode))
		return;

	if (options->prefix &&
	    strncmp(concatpath, options->prefix, options->prefix_length))
		return;

		 const char *concatpath)
	if (DIFF_OPT_TST(options, IGNORE_SUBMODULES) && S_ISGITLINK(old_mode)
			&& S_ISGITLINK(new_mode))
		return;


	if (options->prefix &&
	    strncmp(concatpath, options->prefix, options->prefix_length))
		return;


	if (options->prefix &&
	    strncmp(path, options->prefix, options->prefix_length))
		return;


static char *run_textconv(const char *pgm, struct diff_filespec *spec,
		size_t *outsize)
{
	struct diff_tempfile temp;
	const char *argv[3];
	const char **arg = argv;
	struct child_process child;
	struct strbuf buf = STRBUF_INIT;

	prepare_temp_file(spec->path, &temp, spec);
	*arg++ = pgm;
	*arg++ = temp.name;
	*arg = NULL;

	memset(&child, 0, sizeof(child));
	child.argv = argv;
	child.out = -1;
	if (start_command(&child) != 0 ||
	    strbuf_read(&buf, child.out, 0) < 0 ||
	    finish_command(&child) != 0) {
		if (temp.name == temp.tmp_path)
			unlink(temp.name);
		error("error running textconv command '%s'", pgm);
		return NULL;
	}
	if (temp.name == temp.tmp_path)
		unlink(temp.name);

	return strbuf_detach(&buf, outsize);
}