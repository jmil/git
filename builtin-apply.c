"git-apply [--stat] [--numstat] [--summary] [--check] [--index] [--cached] [--apply] [--no-add] [--index-info] [--allow-binary-replacement] [--reverse] [--reject] [--verbose] [-z] [-pNUM] [-CNUM] [--whitespace=<nowarn|warn|fix|error|error-all>] <patch>...";
		die("git-apply: read returned %s", strerror(errno));
 * Get the name etc info from the --/+++ lines of a traditional patch header
			die("git-apply: bad git-diff - expected /dev/null, got %s on line %d", name, linenr);
			die("git-apply: bad git-diff - inconsistent %s filename on line %d", oldnew, linenr);
			die("git-apply: bad git-diff - expected /dev/null on line %d", linenr);
	unsigned result = check_and_emit_line(line + 1, len - 1, ws_rule,
	    NULL, NULL, NULL, NULL);
		     patch_input_file, linenr, err, len - 2, line + 1);
	if (!unidiff_zero || context) {
		/* If the user says the patch is not generated with
		 * --unified=0, or if we have seen context lines,
		 * then not having oldlines means the patch is creation,
		 * and not having newlines means the patch is deletion.
		 */
		if (patch->is_new < 0 && !oldlines) {
			patch->is_new = 1;
			patch->old_name = NULL;
		}
		if (patch->is_delete < 0 && !newlines) {
			patch->is_delete = 1;
			patch->new_name = NULL;
		}
	}
		convert_to_git(path, buf->buf, buf->len, buf);
static int find_offset(const char *buf, unsigned long size,
		       const char *fragment, unsigned long fragsize,
		       int line, int *lines)
	unsigned long start, backwards, forwards;
	if (fragsize > size)
	start = 0;
	if (line > 1) {
		unsigned long offset = 0;
		i = line-1;
		while (offset + fragsize <= size) {
			if (buf[offset++] == '\n') {
				start = offset;
				if (!--i)
					break;
			}
		}
	}
	/* Exact line number? */
	if ((start + fragsize <= size) &&
	    !memcmp(buf + start, fragment, fragsize))
		return start;
	backwards = start;
	forwards = start;
		unsigned long try;
		int n;
		/* "backward" */
			if (!backwards) {
				if (forwards + fragsize > size)
					break;
				continue;
			do {
				--backwards;
			} while (backwards && buf[backwards-1] != '\n');
			while (forwards + fragsize <= size) {
				if (buf[forwards++] == '\n')
					break;
		if (try + fragsize > size)
			continue;
		if (memcmp(buf + try, fragment, fragsize))
			continue;
		n = (i >> 1)+1;
		if (i & 1)
			n = -n;
		*lines = n;
		return try;

	/*
	 * We should start searching forward and backward.
	 */
static void remove_first_line(const char **rbuf, int *rsize)
	const char *buf = *rbuf;
	int size = *rsize;
	unsigned long offset;
	offset = 0;
	while (offset <= size) {
		if (buf[offset++] == '\n')
			break;
	}
	*rsize = size - offset;
	*rbuf = buf + offset;
static void remove_last_line(const char **rbuf, int *rsize)
	const char *buf = *rbuf;
	int size = *rsize;
	unsigned long offset;
	offset = size - 1;
	while (offset > 0) {
		if (buf[--offset] == '\n')
			break;
	}
	*rsize = offset + 1;
static int apply_line(char *output, const char *patch, int plen,
		      unsigned ws_rule)
	 * plen is number of bytes to be copied from patch,
	 * starting at patch+1 (patch[0] is '+').  Typically
	 * patch[plen] is '\n', unless this is the incomplete
	 * last line.
	int i;
	int add_nl_to_tail = 0;
	int fixed = 0;
	int last_tab_in_indent = 0;
	int last_space_in_indent = 0;
	int need_fix_leading_space = 0;
	char *buf;

	if ((ws_error_action != correct_ws_error) || !whitespace_error ||
	    *patch != '+') {
		memcpy(output, patch + 1, plen);
		return plen;
	}

	/*
	 * Strip trailing whitespace
	 */
	if ((ws_rule & WS_TRAILING_SPACE) &&
	    (1 < plen && isspace(patch[plen-1]))) {
		if (patch[plen] == '\n')
			add_nl_to_tail = 1;
		plen--;
		while (0 < plen && isspace(patch[plen]))
			plen--;
		fixed = 1;
	}

	/*
	 * Check leading whitespaces (indent)
	 */
	for (i = 1; i < plen; i++) {
		char ch = patch[i];
		if (ch == '\t') {
			last_tab_in_indent = i;
			if ((ws_rule & WS_SPACE_BEFORE_TAB) &&
			    0 < last_space_in_indent)
			    need_fix_leading_space = 1;
		} else if (ch == ' ') {
			last_space_in_indent = i;
			if ((ws_rule & WS_INDENT_WITH_NON_TAB) &&
			    8 <= i - last_tab_in_indent)
				need_fix_leading_space = 1;
		}
		else
			break;
	}

	buf = output;
	if (need_fix_leading_space) {
		int consecutive_spaces = 0;
		int last = last_tab_in_indent + 1;

		if (ws_rule & WS_INDENT_WITH_NON_TAB) {
			/* have "last" point at one past the indent */
			if (last_tab_in_indent < last_space_in_indent)
				last = last_space_in_indent + 1;
			else
				last = last_tab_in_indent + 1;
		}
		 * between patch[1..last], strip the funny spaces,
		 * updating them to tab as needed.
		for (i = 1; i < last; i++, plen--) {
			char ch = patch[i];
			if (ch != ' ') {
				consecutive_spaces = 0;
				*output++ = ch;
			} else {
				consecutive_spaces++;
				if (consecutive_spaces == 8) {
					*output++ = '\t';
					consecutive_spaces = 0;
				}
			}
		}
		while (0 < consecutive_spaces--)
			*output++ = ' ';
		fixed = 1;
		i = last;
	else
		i = 1;

	memcpy(output, patch + i, plen);
	if (add_nl_to_tail)
		output[plen++] = '\n';
	if (fixed)
		applied_after_fixing_ws++;
	return output + plen - buf;
static int apply_one_fragment(struct strbuf *buf, struct fragment *frag,
	int offset, size = frag->size;
	char *old = xmalloc(size);
	char *new = xmalloc(size);
	const char *oldlines, *newlines;
	int oldsize = 0, newsize = 0;
	int pos, lines;
		int plen;
		plen = len-1;
			old[oldsize++] = '\n';
			new[newsize++] = '\n';
			memcpy(old + oldsize, patch + 1, plen);
			oldsize += plen;
			if (first != '+' || !no_add) {
				int added = apply_line(new + newsize, patch,
						       plen, ws_rule);
				newsize += added;
				if (first == '+' &&
				    added == 1 && new[newsize-1] == '\n')
					added_blank_line = 1;

	    oldsize > 0 && old[oldsize - 1] == '\n' &&
	    newsize > 0 && new[newsize - 1] == '\n') {
		oldsize--;
		newsize--;
	oldlines = old;
	newlines = new;
	match_beginning = frag->oldpos <= 1;
	lines = 0;
	pos = frag->newpos;
		offset = find_offset(buf->buf, buf->len,
				     oldlines, oldsize, pos, &lines);
		if (match_end && offset + oldsize != buf->len)
			offset = -1;
		if (match_beginning && offset)
			offset = -1;
		if (offset >= 0) {
			if (ws_error_action == correct_ws_error &&
			    (buf->len - oldsize - offset == 0)) /* end of file? */
				newsize -= new_blank_lines_at_end;

			/* Warn if it was necessary to reduce the number
			 * of context lines.
			 */
			if ((leading != frag->leading) ||
			    (trailing != frag->trailing))
				fprintf(stderr, "Context reduced to (%ld/%ld)"
					" to apply fragment at %d\n",
					leading, trailing, pos + lines);

			strbuf_splice(buf, offset, oldsize, newlines, newsize);
			offset = 0;
		}
			remove_first_line(&oldlines, &oldsize);
			remove_first_line(&newlines, &newsize);
			remove_last_line(&oldlines, &oldsize);
			remove_last_line(&newlines, &newsize);
	if (offset && apply_verbosely)
		error("while searching for:\n%.*s", oldsize, oldlines);
	free(old);
	free(new);
	return offset;
static int apply_binary_fragment(struct strbuf *buf, struct patch *patch)
		dst = patch_delta(buf->buf, buf->len, fragment->patch,
		/* XXX patch_delta NUL-terminates */
		strbuf_attach(buf, dst, len, len + 1);
		strbuf_reset(buf);
		strbuf_add(buf, fragment->patch, fragment->size);
static int apply_binary(struct strbuf *buf, struct patch *patch)
		hash_sha1_file(buf->buf, buf->len, blob_type, sha1);
		if (buf->len)
		strbuf_release(buf);
		/* XXX read_sha1_file NUL-terminates */
		strbuf_attach(buf, result, size, size + 1);
		if (apply_binary_fragment(buf, patch))
		hash_sha1_file(buf->buf, buf->len, blob_type, sha1);
static int apply_fragments(struct strbuf *buf, struct patch *patch)
		return apply_binary(buf, patch);
		if (apply_one_fragment(buf, frag, inaccurate_eof, ws_rule)) {
	if (S_ISGITLINK(ntohl(ce->ce_mode))) {
	if (cached) {
	if (apply_fragments(&buf, patch) < 0)
	patch->result = strbuf_detach(&buf, &patch->resultsize);
		if (has_symlink_leading_path(new_name, NULL))
	if (S_ISGITLINK(ntohl(ce->ce_mode))) {
static int check_patch(struct patch *patch, struct patch *prev_patch)
	struct stat st;
	const char *new_name = patch->new_name;
	const char *name = old_name ? old_name : new_name;
	struct cache_entry *ce = NULL;
	int ok_if_exists;

	patch->rejected = 1; /* we will drop this after we succeed */
	if (old_name) {
		int stat_ret = 0;
		unsigned st_mode = 0;

		if (!cached)
			stat_ret = lstat(old_name, &st);
		if (check_index) {
			int pos = cache_name_pos(old_name, strlen(old_name));
			if (pos < 0)
				return error("%s: does not exist in index",
					     old_name);
			ce = active_cache[pos];
			if (stat_ret < 0) {
				struct checkout costate;
				if (errno != ENOENT)
					return error("%s: %s", old_name,
						     strerror(errno));
				/* checkout */
				costate.base_dir = "";
				costate.base_dir_len = 0;
				costate.force = 0;
				costate.quiet = 0;
				costate.not_new = 0;
				costate.refresh_cache = 1;
				if (checkout_entry(ce,
						   &costate,
						   NULL) ||
				    lstat(old_name, &st))
					return -1;
			}
			if (!cached && verify_index_match(ce, &st))
				return error("%s: does not match index",
					     old_name);
			if (cached)
				st_mode = ntohl(ce->ce_mode);
		} else if (stat_ret < 0)
		if (!cached)
			st_mode = ntohl(ce_mode_from_stat(ce, st.st_mode));
		if (patch->is_new < 0)
			patch->is_new = 0;
		if (!patch->old_mode)
			patch->old_mode = st_mode;
		if ((st_mode ^ patch->old_mode) & S_IFMT)
			return error("%s: wrong type", old_name);
		if (st_mode != patch->old_mode)
			fprintf(stderr, "warning: %s has type %o, expected %o\n",
				old_name, st_mode, patch->old_mode);
	}

	if (new_name && prev_patch && 0 < prev_patch->is_delete &&
	    !strcmp(prev_patch->old_name, new_name))
	struct patch *prev_patch = NULL;
	for (prev_patch = NULL; patch ; patch = patch->next) {
		err |= check_patch(patch, prev_patch);
		prev_patch = patch;
			char *name = xstrdup(patch->old_name);
			char *end = strrchr(name, '/');
			while (end) {
				*end = 0;
				if (rmdir(name))
					break;
				end = strrchr(name, '/');
			}
			free(name);
	ce->ce_flags = htons(namelen);
			const char *newpath;
			newpath = mkpath("%s~%u", path, nr);
static int apply_patch(int fd, const char *filename, int inaccurate_eof)
		patch->inaccurate_eof = inaccurate_eof;
static int git_apply_config(const char *var, const char *value)
	if (!strcmp(var, "apply.whitespace")) {
		if (!value)
			return config_error_nonbool(var);
		apply_default_whitespace = xstrdup(value);
		return 0;
	}
	return git_default_config(var, value);
	int inaccurate_eof = 0;
	int is_not_gitdir = 0;
	git_config(git_apply_config);
			errs |= apply_patch(0, "<stdin>", inaccurate_eof);
			inaccurate_eof = 1;
		errs |= apply_patch(fd, arg, inaccurate_eof);
		errs |= apply_patch(0, "<stdin>", inaccurate_eof);