#include "string-list.h"
#include "dir.h"
"git apply [--stat] [--numstat] [--summary] [--check] [--index] [--cached] [--apply] [--no-add] [--index-info] [--allow-binary-replacement] [--reverse] [--reject] [--verbose] [-z] [-pNUM] [-CNUM] [--whitespace=<nowarn|warn|fix|error|error-all>] <patch>...";
static const char *root;
static int root_len;
	unsigned int recount:1;
/*
 * A line in a file, len-bytes long (includes the terminating LF,
 * except for an incomplete line at the end if the file ends with
 * one), and its contents hashes to 'hash'.
 */
struct line {
	size_t len;
	unsigned hash : 24;
	unsigned flag : 8;
#define LINE_COMMON     1
};

/*
 * This represents a "file", which is an array of "lines".
 */
struct image {
	char *buf;
	size_t len;
	size_t nr;
	size_t alloc;
	struct line *line_allocated;
	struct line *line;
};

/*
 * Records filenames that have been touched, in order to handle
 * the case where more than one patches touch the same file.
 */

static struct string_list fn_table;

static uint32_t hash_line(const char *cp, size_t len)
{
	size_t i;
	uint32_t h;
	for (i = 0, h = 0; i < len; i++) {
		if (!isspace(cp[i])) {
			h = h * 3 + (cp[i] & 0xff);
		}
	}
	return h;
}

static void add_line_info(struct image *img, const char *bol, size_t len, unsigned flag)
{
	ALLOC_GROW(img->line_allocated, img->nr + 1, img->alloc);
	img->line_allocated[img->nr].len = len;
	img->line_allocated[img->nr].hash = hash_line(bol, len);
	img->line_allocated[img->nr].flag = flag;
	img->nr++;
}

static void prepare_image(struct image *image, char *buf, size_t len,
			  int prepare_linetable)
{
	const char *cp, *ep;

	memset(image, 0, sizeof(*image));
	image->buf = buf;
	image->len = len;

	if (!prepare_linetable)
		return;

	ep = image->buf + image->len;
	cp = image->buf;
	while (cp < ep) {
		const char *next;
		for (next = cp; next < ep && *next != '\n'; next++)
			;
		if (next < ep)
			next++;
		add_line_info(image, cp, next - cp, 0);
		cp = next;
	}
	image->line = image->line_allocated;
}

static void clear_image(struct image *image)
{
	free(image->buf);
	image->buf = NULL;
	image->len = 0;
}

		die("git apply: read returned %s", strerror(errno));
		struct strbuf name = STRBUF_INIT;
				if (root)
					strbuf_insert(&name, 0, root, root_len);
	if (root) {
		char *ret = xmalloc(root_len + len + 1);
		strcpy(ret, root);
		memcpy(ret + root_len, start, len);
		ret[root_len + len] = '\0';
		return ret;
	}

 * Get the name etc info from the ---/+++ lines of a traditional patch header
			die("git apply: bad git-diff - expected /dev/null, got %s on line %d", name, linenr);
			die("git apply: bad git-diff - inconsistent %s filename on line %d", oldnew, linenr);
			die("git apply: bad git-diff - expected /dev/null on line %d", linenr);
		struct strbuf first = STRBUF_INIT;
		struct strbuf sp = STRBUF_INIT;
			struct strbuf sp = STRBUF_INIT;
	if (patch->def_name && root) {
		char *s = xmalloc(root_len + strlen(patch->def_name) + 1);
		strcpy(s, root);
		strcpy(s + root_len, patch->def_name);
		free(patch->def_name);
		patch->def_name = s;
	}
static void recount_diff(char *line, int size, struct fragment *fragment)
{
	int oldlines = 0, newlines = 0, ret = 0;

	if (size < 1) {
		warning("recount: ignore empty hunk");
		return;
	}

	for (;;) {
		int len = linelen(line, size);
		size -= len;
		line += len;

		if (size < 1)
			break;

		switch (*line) {
		case ' ': case '\n':
			newlines++;
			/* fall through */
		case '-':
			oldlines++;
			continue;
		case '+':
			newlines++;
			continue;
		case '\\':
			continue;
		case '@':
			ret = size < 3 || prefixcmp(line, "@@ ");
			break;
		case 'd':
			ret = size < 5 || prefixcmp(line, "diff ");
			break;
		default:
			ret = -1;
			break;
		}
		if (ret) {
			warning("recount: unexpected line: %.*s",
				(int)linelen(line, size), line);
			return;
		}
		break;
	}
	fragment->oldlines = oldlines;
	fragment->newlines = newlines;
}

	unsigned result = ws_check(line + 1, len - 1, ws_rule);
			patch_input_file, linenr, err, len - 2, line + 1);
	if (offset > 0 && patch->recount)
		recount_diff(line + offset, size - offset, fragment);
	struct strbuf qname = STRBUF_INIT;
		if (strbuf_readlink(buf, path, st->st_size) < 0)
			return error("unable to read symlink %s", path);
		convert_to_git(path, buf->buf, buf->len, buf, 0);
static void update_pre_post_images(struct image *preimage,
				   struct image *postimage,
				   char *buf,
				   size_t len)
	int i, ctx;
	char *new, *old, *fixed;
	struct image fixed_preimage;
	/*
	 * Update the preimage with whitespace fixes.  Note that we
	 * are not losing preimage->buf -- apply_one_fragment() will
	 * free "oldlines".
	 */
	prepare_image(&fixed_preimage, buf, len, 1);
	assert(fixed_preimage.nr == preimage->nr);
	for (i = 0; i < preimage->nr; i++)
		fixed_preimage.line[i].flag = preimage->line[i].flag;
	free(preimage->line_allocated);
	*preimage = fixed_preimage;
	/*
	 * Adjust the common context lines in postimage, in place.
	 * This is possible because whitespace fixing does not make
	 * the string grow.
	 */
	new = old = postimage->buf;
	fixed = preimage->buf;
	for (i = ctx = 0; i < postimage->nr; i++) {
		size_t len = postimage->line[i].len;
		if (!(postimage->line[i].flag & LINE_COMMON)) {
			/* an added line -- no counterparts in preimage */
			memmove(new, old, len);
			old += len;
			new += len;
			continue;
		}

		/* a common context -- skip it in the original postimage */
		old += len;

		/* and find the corresponding one in the fixed preimage */
		while (ctx < preimage->nr &&
		       !(preimage->line[ctx].flag & LINE_COMMON)) {
			fixed += preimage->line[ctx].len;
			ctx++;
		if (preimage->nr <= ctx)
			die("oops");

		/* and copy it in, while fixing the line length */
		len = preimage->line[ctx].len;
		memcpy(new, fixed, len);
		new += len;
		fixed += len;
		postimage->line[i].len = len;
		ctx++;
	/* Fix the length of the whole thing */
	postimage->len = new - postimage->buf;
}

static int match_fragment(struct image *img,
			  struct image *preimage,
			  struct image *postimage,
			  unsigned long try,
			  int try_lno,
			  unsigned ws_rule,
			  int match_beginning, int match_end)
{
	int i;
	char *fixed_buf, *buf, *orig, *target;

	if (preimage->nr + try_lno > img->nr)
		return 0;

	if (match_beginning && try_lno)
		return 0;

	if (match_end && preimage->nr + try_lno != img->nr)
		return 0;

	/* Quick hash check */
	for (i = 0; i < preimage->nr; i++)
		if (preimage->line[i].hash != img->line[try_lno + i].hash)
			return 0;

	/*
	 * Do we have an exact match?  If we were told to match
	 * at the end, size must be exactly at try+fragsize,
	 * otherwise try+fragsize must be still within the preimage,
	 * and either case, the old piece should match the preimage
	 * exactly.
	 */
	if ((match_end
	     ? (try + preimage->len == img->len)
	     : (try + preimage->len <= img->len)) &&
	    !memcmp(img->buf + try, preimage->buf, preimage->len))
		return 1;

	if (ws_error_action != correct_ws_error)
		return 0;

	/*
	 * The hunk does not apply byte-by-byte, but the hash says
	 * it might with whitespace fuzz.
	 */
	fixed_buf = xmalloc(preimage->len + 1);
	buf = fixed_buf;
	orig = preimage->buf;
	target = img->buf + try;
	for (i = 0; i < preimage->nr; i++) {
		size_t fixlen; /* length after fixing the preimage */
		size_t oldlen = preimage->line[i].len;
		size_t tgtlen = img->line[try_lno + i].len;
		size_t tgtfixlen; /* length after fixing the target line */
		char tgtfixbuf[1024], *tgtfix;
		int match;

		/* Try fixing the line in the preimage */
		fixlen = ws_fix_copy(buf, orig, oldlen, ws_rule, NULL);

		/* Try fixing the line in the target */
		if (sizeof(tgtfixbuf) > tgtlen)
			tgtfix = tgtfixbuf;
		else
			tgtfix = xmalloc(tgtlen);
		tgtfixlen = ws_fix_copy(tgtfix, target, tgtlen, ws_rule, NULL);

		/*
		 * If they match, either the preimage was based on
		 * a version before our tree fixed whitespace breakage,
		 * or we are lacking a whitespace-fix patch the tree
		 * the preimage was based on already had (i.e. target
		 * has whitespace breakage, the preimage doesn't).
		 * In either case, we are fixing the whitespace breakages
		 * so we might as well take the fix together with their
		 * real change.
		 */
		match = (tgtfixlen == fixlen && !memcmp(tgtfix, buf, fixlen));

		if (tgtfix != tgtfixbuf)
			free(tgtfix);
		if (!match)
			goto unmatch_exit;

		orig += oldlen;
		buf += fixlen;
		target += tgtlen;
	}

	/*
	 * Yes, the preimage is based on an older version that still
	 * has whitespace breakages unfixed, and fixing them makes the
	 * hunk match.  Update the context lines in the postimage.
	 */
	update_pre_post_images(preimage, postimage,
			       fixed_buf, buf - fixed_buf);
	return 1;

 unmatch_exit:
	free(fixed_buf);
	return 0;
}

static int find_pos(struct image *img,
		    struct image *preimage,
		    struct image *postimage,
		    int line,
		    unsigned ws_rule,
		    int match_beginning, int match_end)
{
	int i;
	unsigned long backwards, forwards, try;
	int backwards_lno, forwards_lno, try_lno;

	if (preimage->nr > img->nr)
		return -1;

	/*
	 * If match_begining or match_end is specified, there is no
	 * point starting from a wrong line that will never match and
	 * wander around and wait for a match at the specified end.
	 */
	if (match_beginning)
		line = 0;
	else if (match_end)
		line = img->nr - preimage->nr;

	if (line > img->nr)
		line = img->nr;

	try = 0;
	for (i = 0; i < line; i++)
		try += img->line[i].len;
	backwards = try;
	backwards_lno = line;
	forwards = try;
	forwards_lno = line;
	try_lno = line;

		if (match_fragment(img, preimage, postimage,
				   try, try_lno, ws_rule,
				   match_beginning, match_end))
			return try_lno;

	again:
		if (backwards_lno == 0 && forwards_lno == img->nr)
			break;
			if (backwards_lno == 0) {
				i++;
				goto again;
			backwards_lno--;
			backwards -= img->line[backwards_lno].len;
			try_lno = backwards_lno;
			if (forwards_lno == img->nr) {
				i++;
				goto again;
			forwards += img->line[forwards_lno].len;
			forwards_lno++;
			try_lno = forwards_lno;
static void remove_first_line(struct image *img)
	img->buf += img->line[0].len;
	img->len -= img->line[0].len;
	img->line++;
	img->nr--;
static void remove_last_line(struct image *img)
	img->len -= img->line[--img->nr].len;
static void update_image(struct image *img,
			 int applied_pos,
			 struct image *preimage,
			 struct image *postimage)
	 * remove the copy of preimage at offset in img
	 * and replace it with postimage
	int i, nr;
	size_t remove_count, insert_count, applied_at = 0;
	char *result;
	for (i = 0; i < applied_pos; i++)
		applied_at += img->line[i].len;

	remove_count = 0;
	for (i = 0; i < preimage->nr; i++)
		remove_count += img->line[applied_pos + i].len;
	insert_count = postimage->len;

	/* Adjust the contents */
	result = xmalloc(img->len + insert_count - remove_count + 1);
	memcpy(result, img->buf, applied_at);
	memcpy(result + applied_at, postimage->buf, postimage->len);
	memcpy(result + applied_at + postimage->len,
	       img->buf + (applied_at + remove_count),
	       img->len - (applied_at + remove_count));
	free(img->buf);
	img->buf = result;
	img->len += insert_count - remove_count;
	result[img->len] = '\0';

	/* Adjust the line table */
	nr = img->nr + postimage->nr - preimage->nr;
	if (preimage->nr < postimage->nr) {
		 * NOTE: this knows that we never call remove_first_line()
		 * on anything other than pre/post image.
		img->line = xrealloc(img->line, nr * sizeof(*img->line));
		img->line_allocated = img->line;
	if (preimage->nr != postimage->nr)
		memmove(img->line + applied_pos + postimage->nr,
			img->line + applied_pos + preimage->nr,
			(img->nr - (applied_pos + preimage->nr)) *
			sizeof(*img->line));
	memcpy(img->line + applied_pos,
	       postimage->line,
	       postimage->nr * sizeof(*img->line));
	img->nr = nr;
static int apply_one_fragment(struct image *img, struct fragment *frag,
	int size = frag->size;
	char *old, *new, *oldlines, *newlines;
	int pos, applied_pos;
	struct image preimage;
	struct image postimage;
	memset(&preimage, 0, sizeof(preimage));
	memset(&postimage, 0, sizeof(postimage));
	oldlines = xmalloc(size);
	newlines = xmalloc(size);

	old = oldlines;
	new = newlines;
		int plen, added;
		plen = len - 1;
			*old++ = '\n';
			*new++ = '\n';
			add_line_info(&preimage, "\n", 1, LINE_COMMON);
			add_line_info(&postimage, "\n", 1, LINE_COMMON);
			memcpy(old, patch + 1, plen);
			add_line_info(&preimage, old, plen,
				      (first == ' ' ? LINE_COMMON : 0));
			old += plen;
			/* --no-add does not add new lines */
			if (first == '+' && no_add)
				break;

			if (first != '+' ||
			    !whitespace_error ||
			    ws_error_action != correct_ws_error) {
				memcpy(new, patch + 1, plen);
				added = plen;
			}
			else {
				added = ws_fix_copy(new, patch + 1, plen, ws_rule, &applied_after_fixing_ws);
			add_line_info(&postimage, new, added,
				      (first == '+' ? 0 : LINE_COMMON));
			new += added;
			if (first == '+' &&
			    added == 1 && new[-1] == '\n')
				added_blank_line = 1;
	    old > oldlines && old[-1] == '\n' &&
	    new > newlines && new[-1] == '\n') {
		old--;
		new--;
	 * but we need to be careful.  -U0 that inserts before the second
	 * line also has this pattern.
	match_beginning = (!frag->oldpos ||
			   (frag->oldpos == 1 && !unidiff_zero));
	pos = frag->newpos ? (frag->newpos - 1) : 0;
	preimage.buf = oldlines;
	preimage.len = old - oldlines;
	postimage.buf = newlines;
	postimage.len = new - newlines;
	preimage.line = preimage.line_allocated;
	postimage.line = postimage.line_allocated;


		applied_pos = find_pos(img, &preimage, &postimage, pos,
				       ws_rule, match_beginning, match_end);

		if (applied_pos >= 0)

			remove_first_line(&preimage);
			remove_first_line(&postimage);
			remove_last_line(&preimage);
			remove_last_line(&postimage);
	if (applied_pos >= 0) {
		if (ws_error_action == correct_ws_error &&
		    new_blank_lines_at_end &&
		    postimage.nr + applied_pos == img->nr) {
			/*
			 * If the patch application adds blank lines
			 * at the end, and if the patch applies at the
			 * end of the image, remove those added blank
			 * lines.
			 */
			while (new_blank_lines_at_end--)
				remove_last_line(&postimage);
		}
		/*
		 * Warn if it was necessary to reduce the number
		 * of context lines.
		 */
		if ((leading != frag->leading) ||
		    (trailing != frag->trailing))
			fprintf(stderr, "Context reduced to (%ld/%ld)"
				" to apply fragment at %d\n",
				leading, trailing, applied_pos+1);
		update_image(img, applied_pos, &preimage, &postimage);
	} else {
		if (apply_verbosely)
			error("while searching for:\n%.*s",
			      (int)(old - oldlines), oldlines);
	}

	free(oldlines);
	free(newlines);
	free(preimage.line_allocated);
	free(postimage.line_allocated);

	return (applied_pos < 0);
static int apply_binary_fragment(struct image *img, struct patch *patch)
		dst = patch_delta(img->buf, img->len, fragment->patch,
		clear_image(img);
		img->buf = dst;
		img->len = len;
		clear_image(img);
		img->len = fragment->size;
		img->buf = xmalloc(img->len+1);
		memcpy(img->buf, fragment->patch, img->len);
		img->buf[img->len] = '\0';
static int apply_binary(struct image *img, struct patch *patch)
		hash_sha1_file(img->buf, img->len, blob_type, sha1);
		if (img->len)
		clear_image(img);
		clear_image(img);
		img->buf = result;
		img->len = size;
		if (apply_binary_fragment(img, patch))
		hash_sha1_file(img->buf, img->len, blob_type, sha1);
static int apply_fragments(struct image *img, struct patch *patch)
		return apply_binary(img, patch);
		if (apply_one_fragment(img, frag, inaccurate_eof, ws_rule)) {
	if (S_ISGITLINK(ce->ce_mode)) {
static struct patch *in_fn_table(const char *name)
	struct string_list_item *item;
	if (name == NULL)
		return NULL;

	item = string_list_lookup(name, &fn_table);
	if (item != NULL)
		return (struct patch *)item->util;

	return NULL;
}

static void add_to_fn_table(struct patch *patch)
{
	struct string_list_item *item;

	/*
	 * Always add new_name unless patch is a deletion
	 * This should cover the cases for normal diffs,
	 * file creations and copies
	 */
	if (patch->new_name != NULL) {
		item = string_list_insert(patch->new_name, &fn_table);
		item->util = patch;
	}

	/*
	 * store a failure on rename/deletion cases because
	 * later chunks shouldn't patch old names
	 */
	if ((patch->new_name == NULL) || (patch->is_rename)) {
		item = string_list_insert(patch->old_name, &fn_table);
		item->util = (struct patch *) -1;
	}
}

static int apply_data(struct patch *patch, struct stat *st, struct cache_entry *ce)
{
	struct strbuf buf = STRBUF_INIT;
	struct image image;
	size_t len;
	char *img;
	struct patch *tpatch;

	if (!(patch->is_copy || patch->is_rename) &&
	    ((tpatch = in_fn_table(patch->old_name)) != NULL)) {
		if (tpatch == (struct patch *) -1) {
			return error("patch %s has been renamed/deleted",
				patch->old_name);
		}
		/* We have a patched copy in memory use that */
		strbuf_add(&buf, tpatch->result, tpatch->resultsize);
	} else if (cached) {
	img = strbuf_detach(&buf, &len);
	prepare_image(&image, img, len, !patch->is_binary);

	if (apply_fragments(&image, patch) < 0)
	patch->result = image.buf;
	patch->resultsize = image.len;
	add_to_fn_table(patch);
	free(image.line_allocated);
		if (has_symlink_leading_path(strlen(new_name), new_name))
	if (S_ISGITLINK(ce->ce_mode)) {
static int check_preimage(struct patch *patch, struct cache_entry **ce, struct stat *st)
	struct patch *tpatch = NULL;
	int stat_ret = 0;
	unsigned st_mode = 0;
	if (!old_name)
		return 0;
	assert(patch->is_new <= 0);

	if (!(patch->is_copy || patch->is_rename) &&
	    (tpatch = in_fn_table(old_name)) != NULL) {
		if (tpatch == (struct patch *) -1) {
			return error("%s: has been deleted/renamed", old_name);
		}
		st_mode = tpatch->new_mode;
	} else if (!cached) {
		stat_ret = lstat(old_name, st);
		if (stat_ret && errno != ENOENT)
			return error("%s: %s", old_name, strerror(errno));
	}
	if (check_index && !tpatch) {
		int pos = cache_name_pos(old_name, strlen(old_name));
		if (pos < 0) {
			if (patch->is_new < 0)
				goto is_new;
			return error("%s: does not exist in index", old_name);
		}
		*ce = active_cache[pos];
		if (stat_ret < 0) {
			struct checkout costate;
			/* checkout */
			costate.base_dir = "";
			costate.base_dir_len = 0;
			costate.force = 0;
			costate.quiet = 0;
			costate.not_new = 0;
			costate.refresh_cache = 1;
			if (checkout_entry(*ce, &costate, NULL) ||
			    lstat(old_name, st))
				return -1;
		}
		if (!cached && verify_index_match(*ce, st))
			return error("%s: does not match index", old_name);
		if (cached)
			st_mode = (*ce)->ce_mode;
	} else if (stat_ret < 0) {
			goto is_new;
		return error("%s: %s", old_name, strerror(errno));
	}

	if (!cached)
		st_mode = ce_mode_from_stat(*ce, st->st_mode);

	if (patch->is_new < 0)
		patch->is_new = 0;
	if (!patch->old_mode)
		patch->old_mode = st_mode;
	if ((st_mode ^ patch->old_mode) & S_IFMT)
		return error("%s: wrong type", old_name);
	if (st_mode != patch->old_mode)
		fprintf(stderr, "warning: %s has type %o, expected %o\n",
			old_name, st_mode, patch->old_mode);
	return 0;

 is_new:
	patch->is_new = 1;
	patch->is_delete = 0;
	patch->old_name = NULL;
	return 0;
}

static int check_patch(struct patch *patch)
{
	struct stat st;
	const char *old_name = patch->old_name;
	const char *new_name = patch->new_name;
	const char *name = old_name ? old_name : new_name;
	struct cache_entry *ce = NULL;
	int ok_if_exists;
	int status;

	patch->rejected = 1; /* we will drop this after we succeed */

	status = check_preimage(patch, &ce, &st);
	if (status)
		return status;
	old_name = patch->old_name;

	if (in_fn_table(new_name) == (struct patch *) -1)
	while (patch) {
		err |= check_patch(patch);
		patch = patch->next;
		if (!ce)
			die("make_cache_entry failed for path '%s'", name);
			remove_path(patch->old_name);
	ce->ce_flags = namelen;
	struct strbuf nbuf = STRBUF_INIT;
			char newpath[PATH_MAX];
			mksnpath(newpath, sizeof(newpath), "%s~%u", path, nr);
static struct string_list limit_by_name;
static int has_include;
static void add_name_limit(const char *name, int exclude)
{
	struct string_list_item *it;

	it = string_list_append(name, &limit_by_name);
	it->util = exclude ? NULL : (void *) 1;
}
	int i;

	/* Paths outside are not touched regardless of "--include" */

	/* See if it matches any of exclude/include rule */
	for (i = 0; i < limit_by_name.nr; i++) {
		struct string_list_item *it = &limit_by_name.items[i];
		if (!fnmatch(it->string, pathname, 0))
			return (it->util != NULL);
	}

	/*
	 * If we had any include, a path that does not match any rule is
	 * not used.  Otherwise, we saw bunch of exclude rules (or none)
	 * and such a path is used.
	 */
	return !has_include;

#define INACCURATE_EOF	(1<<0)
#define RECOUNT		(1<<1)

static int apply_patch(int fd, const char *filename, int options)
	struct strbuf buf = STRBUF_INIT;
	/* FIXME - memory leak when using multiple patch files as inputs */
	memset(&fn_table, 0, sizeof(struct string_list));
		patch->inaccurate_eof = !!(options & INACCURATE_EOF);
		patch->recount =  !!(options & RECOUNT);
static int git_apply_config(const char *var, const char *value, void *cb)
	if (!strcmp(var, "apply.whitespace"))
		return git_config_string(&apply_default_whitespace, var, value);
	return git_default_config(var, value, cb);
	int options = 0;
	int is_not_gitdir;
	git_config(git_apply_config, NULL);
			errs |= apply_patch(0, "<stdin>", options);
			add_name_limit(arg + 10, 1);
			continue;
		}
		if (!prefixcmp(arg, "--include=")) {
			add_name_limit(arg + 10, 0);
			has_include = 1;
			options |= INACCURATE_EOF;
			continue;
		}
		if (!strcmp(arg, "--recount")) {
			options |= RECOUNT;
			continue;
		}
		if (!prefixcmp(arg, "--directory=")) {
			arg += strlen("--directory=");
			root_len = strlen(arg);
			if (root_len && arg[root_len - 1] != '/') {
				char *new_root;
				root = new_root = xmalloc(root_len + 2);
				strcpy(new_root, arg);
				strcpy(new_root + root_len++, "/");
			} else
				root = arg;
		errs |= apply_patch(fd, arg, options);
		errs |= apply_patch(0, "<stdin>", options);