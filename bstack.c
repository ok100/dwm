static void
bstack(Monitor *m) {
	int w, h, mh, mx, tx, ty, tw;
	unsigned int i, n;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;
	if(n > m->nmaster) {
		mh = m->nmaster ? m->mfact * m->wh : 0;
		tw = m->ww / (n - m->nmaster);
		ty = m->wy + mh;
	} 
	else {
		mh = m->wh;
		tw = m->ww;
		ty = m->wy;
	}
	for(i = mx = 0, tx = m->wx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if(i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			if(n == 1)
				resize(c, m->wx - c->bw, m->wy, m->ww, m->wh, False);
			else
				resize(c, m->wx + mx - c->bw, m->wy, w, mh - c->bw, False);
			mx += WIDTH(c) - c->bw;
		} 
		else {
			h = m->wh - mh;
			resize(c, tx, ty, tw - (2 * c->bw), h - (2 * c->bw), False);
			if(tw != m->ww)
				tx += WIDTH(c);
		}
	}
}
