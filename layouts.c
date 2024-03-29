// TODO: FIX GAPS IN GRID LAYOUT
void
grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - m->gappx) / (rows ? rows : 1);
	cw = (m->ww - m->gappx) / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - m->gappx * (rows + 1) - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - m->gappx * (cols + 1) - cw * cols : 0;
		resize(c, cx + m->gappx * (i / rows + 1), cy + m->gappx * (i % rows + 1), cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void
gaplessgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if(n == 0)
		return;

	/* grid dimensions */
	for(cols = 0; cols <= n/2; cols++)
		if(cols*cols >= n)
			break;
	if(n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? (m->ww - m->gappx) / cols : m->ww - m->gappx;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? (m->wh - m->gappx) / rows : m->wh - m->gappx;
		cx = m->wx + cn*cw;
		cy = m->wy + rn*ch;
		resize(c, cx + m->gappx, cy + m->gappx, cw - 2 * c->bw - m->gappx, ch - 2 * c->bw - m->gappx, False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}
