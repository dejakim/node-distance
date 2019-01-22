const { levenshtein } = require('./build/Release/distance'); // native c++

module.exports.levenshtein = function(s1, s2) {
	return levenshtein(s1, s2);
};

module.exports.distance = function(s1, s2) {
	var src = s1, dst = s2;
	// Let dst is shorter than src
	if (src.length < dst.length) { src = s2, dst = s1; }
	if (dst.length < 1) { return src.length; }
	// Init buffers
	var d1 = Array(dst.length + 1).fill();
	var d0 = d1.map(function(x,i) {return i});
	// Calculation
	for (var i = 0; i < src.length; i++) {
		d1[0] = i + 1;
		c = src[i];
		for (var j = 0; j < dst.length; j++) {
			if (c === dst[j]) {
				d1[j+1] = d0[j];
			} else {
				d1[j+1] = Math.min(d1[j], d0[j], d0[j+1]) + 1;
			}
		}
		// Swap buffers
		var t = d0; d0 = d1; d1 = t;
	}
	return d0[d0.length - 1];
}
