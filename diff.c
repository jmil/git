static int diff_rename_limit_default = 100;
static int diff_use_color_default;
static struct funcname_pattern {
	struct funcname_pattern *next;
static int parse_funcname_pattern(const char *var, const char *ep, const char *value)
	struct funcname_pattern *pp;
		if (!strncmp(pp->name, name, namelen) && !pp->name[namelen])
		pp->name = xmemdupz(name, namelen);
	if (pp->pattern)
		free(pp->pattern);
	pp->pattern = xstrdup(value);
int git_diff_ui_config(const char *var, const char *value)
	if (!strcmp(var, "diff.renamelimit")) {
		diff_rename_limit_default = git_config_int(var, value);
		return 0;
	}
	if (!strcmp(var, "diff.external")) {
		if (!value)
			return config_error_nonbool(var);
		external_diff_cmd_cfg = xstrdup(value);
		return 0;
	}
	return git_diff_basic_config(var, value);
int git_diff_basic_config(const char *var, const char *value)
				return parse_funcname_pattern(var, ep, value);
	return git_default_config(var, value);
static void print_line_count(int count)
		printf("0,0");
		printf("1");
		printf("1,%d", count);
static void copy_file_with_prefix(int prefix, const char *data, int size,
			fputs(set, stdout);
			putchar(prefix);
			fputs(reset, stdout);
		putchar(ch);
		printf("%s\n\\ No newline at end of file\n", reset);
	printf("%s--- %s%s%s\n%s+++ %s%s%s\n%s@@ -",
	       metainfo, a_name.buf, name_a_tab, reset,
	       metainfo, b_name.buf, name_b_tab, reset, fraginfo);
	print_line_count(lc_a);
	printf(" +");
	print_line_count(lc_b);
	printf(" @@%s\n", reset);
		copy_file_with_prefix('-', one->data, one->size, old, reset);
		copy_file_with_prefix('+', two->data, two->size, new, reset);
static void print_word(struct diff_words_buffer *buffer, int len, int color,
	fputs(diff_get_color(1, color), stdout);
	fwrite(ptr, len, 1, stdout);
	fputs(diff_get_color(1, DIFF_RESET), stdout);
			putchar('\n');
			putchar('\n');
			print_word(&diff_words->minus, len, DIFF_FILE_OLD, 1);
			print_word(&diff_words->plus, len, DIFF_FILE_NEW, 0);
			print_word(&diff_words->plus, len, DIFF_PLAIN, 0);
		putchar('\n');
		if (ecbdata->diff_words->minus.text.ptr)
			free (ecbdata->diff_words->minus.text.ptr);
		if (ecbdata->diff_words->plus.text.ptr)
			free (ecbdata->diff_words->plus.text.ptr);
static void emit_line(const char *set, const char *reset, const char *line, int len)
	fputs(set, stdout);
	fwrite(line, len, 1, stdout);
	fputs(reset, stdout);
		emit_line(set, reset, line, len);
		emit_line(set, reset, line, ecbdata->nparents);
		(void)check_and_emit_line(line + ecbdata->nparents,
		    len - ecbdata->nparents, ecbdata->ws_rule,
		    stdout, set, reset, ws);
		printf("%s--- %s%s%s\n",
		       meta, ecbdata->label_path[0], reset, name_a_tab);
		printf("%s+++ %s%s%s\n",
		       meta, ecbdata->label_path[1], reset, name_b_tab);
		emit_line(diff_get_color(ecbdata->color_diff, DIFF_FRAGINFO),
			putchar('\n');
		emit_line(reset, reset, line, len);
		emit_line(plain, reset, line, len);
		emit_line(diff_get_color(ecbdata->color_diff, color),
static void show_name(const char *prefix, const char *name, int len,
	printf(" %s%s%-*s%s |", set, prefix, len, name, reset);
static void show_graph(char ch, int cnt, const char *set, const char *reset)
	printf("%s", set);
		putchar(ch);
	printf("%s", reset);
	if (width < name_width + 15) {
		if (name_width <= 25)
			width = name_width + 15;
		else
			name_width = width - 15;
	}
			show_name(prefix, name, len, reset, set);
			printf("  Bin ");
			printf("%s%d%s", del_c, deleted, reset);
			printf(" -> ");
			printf("%s%d%s", add_c, added, reset);
			printf(" bytes");
			printf("\n");
			show_name(prefix, name, len, reset, set);
			printf("  Unmerged\n");
		show_name(prefix, name, len, reset, set);
		printf("%5d ", added + deleted);
		show_graph('+', add, add_c, reset);
		show_graph('-', del, del_c, reset);
		putchar('\n');
	}
	printf("%s %d files changed, %d insertions(+), %d deletions(-)%s\n",
static void show_shortstats(struct diffstat_t* data)
	printf(" %d files changed, %d insertions(+), %d deletions(-)\n",
			printf("-\t-\t");
			printf("%d\t%d\t", file->added, file->deleted);
				write_name_quoted(file->name, stdout,
				fputs(file->print_name, stdout);
				putchar(options->line_termination);
				putchar('\0');
				write_name_quoted(file->from_name, stdout, '\0');
			write_name_quoted(file->name, stdout, '\0');
	int lineno, color_diff;
	const char *ws = diff_get_color(data->color_diff, DIFF_WHITESPACE);
	const char *reset = diff_get_color(data->color_diff, DIFF_RESET);
	const char *set = diff_get_color(data->color_diff, DIFF_FILE_NEW);
		data->status = check_and_emit_line(line + 1, len - 1,
		    data->ws_rule, NULL, NULL, NULL, NULL);
		if (!data->status)
		err = whitespace_error_string(data->status);
		printf("%s:%d: %s.\n", data->filename, data->lineno, err);
		emit_line(set, reset, line, 1);
		(void)check_and_emit_line(line + 1, len - 1, data->ws_rule,
		    stdout, set, reset, ws);
	} else if (line[0] == ' ')
	else if (line[0] == '@') {
static void emit_binary_diff_body(mmfile_t *one, mmfile_t *two)
		printf("delta %lu\n", orig_size);
		printf("literal %lu\n", two->size);
		puts(line);
	printf("\n");
static void emit_binary_diff(mmfile_t *one, mmfile_t *two)
	printf("GIT binary patch\n");
	emit_binary_diff_body(one, two);
	emit_binary_diff_body(two, one);
static const char *funcname_pattern(const char *ident)
	struct funcname_pattern *pp;
		if (!strcmp(ident, pp->name))
			return pp->pattern;
static struct builtin_funcname_pattern {
	const char *name;
	const char *pattern;
} builtin_funcname_pattern[] = {
	{ "java", "!^[ 	]*\\(catch\\|do\\|for\\|if\\|instanceof\\|"
			"new\\|return\\|switch\\|throw\\|while\\)\n"
			"^[ 	]*\\(\\([ 	]*"
			"[A-Za-z_][A-Za-z_0-9]*\\)\\{2,\\}"
			"[ 	]*([^;]*$\\)" },
	{ "tex", "^\\(\\\\\\(sub\\)*section{.*\\)$" },
static const char *diff_funcname_pattern(struct diff_filespec *one)
	const char *ident, *pattern;
	pattern = funcname_pattern(ident);
	if (pattern)
		return pattern;
			return builtin_funcname_pattern[i].pattern;
	printf("%sdiff --git %s %s%s\n", set, a_one, b_two, reset);
		printf("%snew file mode %06o%s\n", set, two->mode, reset);
			printf("%s%s%s\n", set, xfrm_msg, reset);
		printf("%sdeleted file mode %06o%s\n", set, one->mode, reset);
			printf("%s%s%s\n", set, xfrm_msg, reset);
			printf("%sold mode %06o%s\n", set, one->mode, reset);
			printf("%snew mode %06o%s\n", set, two->mode, reset);
			printf("%s%s%s\n", set, xfrm_msg, reset);
			emit_binary_diff(&mf1, &mf2);
			printf("Binary files %s and %s differ\n",
			       lbl[0], lbl[1]);
		const char *funcname_pattern;
		funcname_pattern = diff_funcname_pattern(one);
		if (!funcname_pattern)
			funcname_pattern = diff_funcname_pattern(two);
		if (funcname_pattern)
			xdiff_set_find_func(&xecfg, funcname_pattern);
		if (DIFF_OPT_TST(o, COLOR_DIFF_WORDS))
			     struct diff_filespec *one,
			     struct diff_filespec *two, struct diff_options *o)
	data.color_diff = DIFF_OPT_TST(o, COLOR_DIFF);
	data.ws_rule = whitespace_rule(data.filename);
	if ((lstat(name, &st) < 0) ||
	    !S_ISREG(st.st_mode) || /* careful! */
	    ce_match_stat(ce, &st, 0) ||
	    hashcmp(sha1, ce->sha1))
	/* we return 1 only when we can stat, it is a regular file,
	 * stat information matches, and sha1 recorded in the cache
	 * matches.  I.e. we know the file in the work tree really is
	 * the same as the <name, sha1> pair.
	return 1;
		if (convert_to_git(s->path, s->data, s->size, &buf)) {
		const char *cmd = external_diff_attr(name);
		printf("* Unmerged path %s\n", name);
		run_diff_cmd(pgm, p->one->path, NULL, NULL, NULL, NULL, o, 0);
	name  = p->one->path;
	other = (strcmp(name, p->two->path) ? p->two->path : NULL);
		run_diff_cmd(NULL, name, other, one, null, xfrm_msg, o, 0);
		run_diff_cmd(NULL, name, other, null, two, xfrm_msg, o, 0);
		run_diff_cmd(pgm, name, other, one, two, xfrm_msg, o,
			     complete_rewrite);
	builtin_checkdiff(name, other, p->one, p->two, o);
	options->msg_sep = "";
	if (diff_use_color_default)
	/*
	 * If we postprocess in diffcore, we cannot simply return
	 * upon the first hit.  We need to run diff as usual.
	 */
	if (options->pickaxe || options->filter)
		DIFF_OPT_CLR(options, QUIET);

	else
	if (!abbrev)
		return sha1_to_hex(sha1);
		printf(":%06o %06o %s ", p->one->mode, p->two->mode,
		       diff_unique_abbrev(p->one->sha1, opt->abbrev));
		printf("%s ", diff_unique_abbrev(p->two->sha1, opt->abbrev));
		printf("%c%03d%c", p->status, similarity_index(p),
			   inter_name_termination);
		printf("%c%c", p->status, inter_name_termination);
	if (p->status == DIFF_STATUS_COPIED || p->status == DIFF_STATUS_RENAMED) {
		write_name_quoted(p->one->path, stdout, inter_name_termination);
		write_name_quoted(p->two->path, stdout, line_termination);
		const char *path = p->one->mode ? p->one->path : p->two->path;
		write_name_quoted(path, stdout, line_termination);
	else if (fmt & DIFF_FORMAT_NAME)
		write_name_quoted(p->two->path, stdout, opt->line_termination);
static void show_file_mode_name(const char *newdelete, struct diff_filespec *fs)
		printf(" %s mode %06o ", newdelete, fs->mode);
		printf(" %s ", newdelete);
	write_name_quoted(fs->path, stdout, '\n');
static void show_mode_change(struct diff_filepair *p, int show_name)
		printf(" mode change %06o => %06o%c", p->one->mode, p->two->mode,
			write_name_quoted(p->two->path, stdout, '\n');
static void show_rename_copy(const char *renamecopy, struct diff_filepair *p)
	printf(" %s %s (%d%%)\n", renamecopy, names, similarity_index(p));
	show_mode_change(p, 0);
static void diff_summary(struct diff_filepair *p)
		show_file_mode_name("delete", p->one);
		show_file_mode_name("create", p->two);
		show_rename_copy("copy", p);
		show_rename_copy("rename", p);
			fputs(" rewrite ", stdout);
			write_name_quoted(p->two->path, stdout, ' ');
			printf("(%d%%)\n", similarity_index(p));
		show_mode_change(p, !p->score);
			show_shortstats(&diffstat);
			diff_summary(q->queue[i]);
				fputs(options->stat_sep, stdout);
			} else {
				putchar(options->line_termination);
	if (S_ISGITLINK(one->mode)) {
		diff_fill_sha1_info(one);
		diff_fill_sha1_info(two);
		return !hashcmp(one->sha1, two->sha1);
	}
	if (DIFF_OPT_TST(options, QUIET))
		return;

	if (options->skip_stat_unmatch && !DIFF_OPT_TST(options, FIND_COPIES_HARDER))
		    const char *base, const char *path)
	char concatpath[PATH_MAX];
	if (!path) path = "";
	sprintf(concatpath, "%s%s", base, path);
		 const char *base, const char *path)
	char concatpath[PATH_MAX];
	if (!path) path = "";
	sprintf(concatpath, "%s%s", base, path);