// works in-place, O(n*d), swap needs to be adapted
// could contain a bug! test against sort first!
// len = length of element strings
const int max_val=256;
void radix_sort_msb(unsigned char seq[][], int len, int l, int r, int base = 0) {
	if (r-l <= 20) {
		sort(seq+l,seq+r); // even better: use insertion sort
		return;
	}
	if (base == len || r-l <= 1) return;
	int start[max_val+1] = {0}, pos[max_val+1] = {0};
	start[0] = l;
	rep(i,l,r) start[seq[i][base]+1]++;
	rep(i,1,max_val+1) start[i] += start[i-1], pos[i] = start[i];
	rep(c,0,max_val) {
		rep(i,start[c], start[c+1]) {
			for(;;) {
				int d = seq[i][base];
				if (d==c) break;
				int& j = pos[d];
				swap(seq[i], seq[j]);
				j++;
			}
		}
		radix_sort_msb(seq,len,start[c],start[c+1],base+1);
	}
}
